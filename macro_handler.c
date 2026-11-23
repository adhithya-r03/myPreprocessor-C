#include "header.h"
#include <stdio.h>
#include <string.h>

#define MAX_MACROS 100
#define MAX_PARAMS 10
#define MAX_LEN 100

typedef struct macro {
    char name[MAX_LEN];
    char val[MAX_LEN];
    int is_func;
    int argc;
    char params[MAX_PARAMS][MAX_LEN];
} mac;

static mac macro[MAX_MACROS];
static int mc = 0;

static int is_start(int c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

static int is_part(int c) {
    return is_start(c) || (c >= '0' && c <= '9');
}

static void read_define(FILE *fp) {
    int ch, k = 0;
    mac *m = &macro[mc];
    m->is_func = 0;
    m->argc = 0;

    while ((ch = fgetc(fp)) == ' ' || ch == '\t');
    if (ch == EOF || ch == '\n') return;

    /* Read macro name (stops at space or '(') */
    while (ch != EOF && ch != '\n' && ch != ' ' && ch != '\t' && ch != '(') {
        if (k < MAX_LEN - 1) m->name[k++] = (char)ch;
        ch = fgetc(fp);
    }
    m->name[k] = '\0';

    /* Function-like macro if '(' immediately follows name */
    if (ch == '(') {
        m->is_func = 1;
        while (ch != EOF && ch != '\n' && ch != ')') {
            k = 0;
            ch = fgetc(fp);
            while (ch == ' ' || ch == '\t') ch = fgetc(fp);

            while (ch != EOF && ch != '\n' && ch != ',' && ch != ')' &&
                   ch != ' ' && ch != '\t') {
                if (k < MAX_LEN - 1) m->params[m->argc][k++] = (char)ch;
                ch = fgetc(fp);
            }
            m->params[m->argc][k] = '\0';
            if (m->params[m->argc][0] != '\0' && m->argc < MAX_PARAMS) {
                m->argc++;
            }
            while (ch != EOF && ch != '\n' && ch != ',' && ch != ')') {
                ch = fgetc(fp);
            }
        }
        if (ch == ')') ch = fgetc(fp);
    }

    while (ch == ' ' || ch == '\t') ch = fgetc(fp);

    /* Read macro value until newline */
    k = 0;
    while (ch != EOF && ch != '\n') {
        if (k < MAX_LEN - 1) m->val[k++] = (char)ch;
        ch = fgetc(fp);
    }
    while (k > 0 && (m->val[k - 1] == ' ' || m->val[k - 1] == '\t' || m->val[k - 1] == '\r')) {
        k--;
    }
    m->val[k] = '\0';

    if (m->name[0] != '\0' && mc < MAX_MACROS) {
        mc++;
    }
}

static void expand_func_macro(mac *m, FILE *fp, FILE *fw, const char *orig) {
    int ch, depth = 1, a = 0, k = 0;
    char args[MAX_PARAMS][MAX_LEN] = {{0}};

    while ((ch = fgetc(fp)) == ' ' || ch == '\t');
    if (ch != '(') {
        if (ch != EOF) ungetc(ch, fp);
        fputs(orig, fw);
        return;
    }

    /* Collect arguments up to matching ')' */
    while ((ch = fgetc(fp)) != EOF && depth > 0) {
        if (ch == '(') {
            depth++;
            if (a < MAX_PARAMS && k < MAX_LEN - 1) args[a][k++] = (char)ch;
        } else if (ch == ')') {
            depth--;
            if (depth == 0) {
                if (a < MAX_PARAMS) args[a][k] = '\0';
                break;
            }
            if (a < MAX_PARAMS && k < MAX_LEN - 1) args[a][k++] = (char)ch;
        } else if (ch == ',' && depth == 1) {
            if (a < MAX_PARAMS) args[a][k] = '\0';
            a++;
            k = 0;
        } else {
            if (a < MAX_PARAMS && k < MAX_LEN - 1) args[a][k++] = (char)ch;
        }
    }

    /* Replace matching parameter names inside macro body */
    int i = 0;
    while (m->val[i] != '\0') {
        if (is_start((unsigned char)m->val[i])) {
            char tok[MAX_LEN];
            int t = 0, p;
            while (m->val[i] != '\0' && is_part((unsigned char)m->val[i])) {
                if (t < MAX_LEN - 1) tok[t++] = m->val[i];
                i++;
            }
            tok[t] = '\0';

            for (p = 0; p < m->argc; p++) {
                if (strcmp(tok, m->params[p]) == 0) break;
            }
            if (p < m->argc) {
                fputs(args[p], fw);
            } else {
                fputs(tok, fw);
            }
        } else {
            fputc(m->val[i++], fw);
        }
    }
}

void macro_handler(FILE *fp, FILE *fw) {
    int ch, i, m_idx;
    char dir[MAX_LEN], word[MAX_LEN];
    int in_str = 0;

    if (!fp || !fw) return;

    while ((ch = fgetc(fp)) != EOF) {
        if (ch == '"') {
            in_str = !in_str;
            fputc(ch, fw);
        } else if (in_str) {
            fputc(ch, fw);
        } else if (ch == ' ' || ch == '\t') {
            fputc(ch, fw);
        } else if (ch == '#') {
            i = 0;
            while ((ch = fgetc(fp)) == ' ' || ch == '\t');
            if (ch == EOF) break;
            ungetc(ch, fp);

            while ((ch = fgetc(fp)) != EOF && ch != '\n' && ch != ' ' && ch != '\t') {
                if (i < MAX_LEN - 1) dir[i++] = (char)ch;
            }
            dir[i] = '\0';

            if (strcmp(dir, "define") == 0) {
                if (ch != EOF && ch != '\n') ungetc(ch, fp);
                read_define(fp);
            } else {
                fputc('#', fw);
                fputs(dir, fw);
                if (ch != EOF) fputc(ch, fw);
                while (ch != '\n' && ch != EOF) {
                    ch = fgetc(fp);
                    if (ch != EOF) fputc(ch, fw);
                }
            }
        } else if (is_start(ch)) {
            int w = 0;
            word[w++] = (char)ch;
            while ((ch = fgetc(fp)) != EOF) {
                if (is_part(ch)) {
                    if (w < MAX_LEN - 1) word[w++] = (char)ch;
                } else {
                    ungetc(ch, fp);
                    break;
                }
            }
            word[w] = '\0';

            for (m_idx = 0; m_idx < mc; m_idx++) {
                if (strcmp(word, macro[m_idx].name) == 0) break;
            }

            if (m_idx < mc) {
                if (macro[m_idx].is_func) {
                    expand_func_macro(&macro[m_idx], fp, fw, word);
                } else {
                    fputs(macro[m_idx].val, fw);
                }
            } else {
                fputs(word, fw);
            }
        } else {
            fputc(ch, fw);
        }
    }
}