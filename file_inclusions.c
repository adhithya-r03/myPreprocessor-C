#include "header.h"

#define INCLUDE_DIR "C:/CTOOLS/mingw32/i686-w64-mingw32/include"

void file_inclusion(FILE *fp, FILE *fw) {
    int ch;
    int i, j;
    char s[20], d[200], path[300];
    char c[] = "include";
    FILE *fr;

    while ((ch = fgetc(fp)) != EOF) {
        if (ch == ' ' || ch == '\t') {
            fputc(ch, fw);
        } else if (ch == '#') {
            i = 0;
            while ((ch = fgetc(fp)) == ' ' || ch == '\t');
            if (ch == EOF) {
                break;
            }
            ungetc(ch, fp);

            while (((ch = fgetc(fp)) != ' ') && ch != EOF && ch != '\n' &&
                   ch != '\t' && ch != '<' && ch != '"') {
                if (i < (int)sizeof(s) - 1) {
                    s[i++] = (char)ch;
                }
            }
            s[i] = '\0';

            if (strcmp(s, c) == 0) {
                if (ch == '"' || ch == '<') {
                    ungetc(ch, fp);
                }
                while ((ch = fgetc(fp)) == ' ' || ch == '\t');

                fr = NULL;
                d[0] = '\0';

                if (ch == '"') {
                    j = 0;
                    while (((ch = fgetc(fp)) != '"') && ch != EOF && ch != '\n') {
                        if (j < (int)sizeof(d) - 1) {
                            d[j++] = (char)ch;
                        }
                    }
                    d[j] = '\0';

                    if (ch == '"') {
                        fr = fopen(d, "r");
                    }
                } else if (ch == '<') {
                    j = 0;
                    while (((ch = fgetc(fp)) != '>') && ch != EOF && ch != '\n') {
                        if (j < (int)sizeof(d) - 1) {
                            d[j++] = (char)ch;
                        }
                    }
                    d[j] = '\0';

                    if (ch == '>') {
                        snprintf(path, sizeof(path), "%s/%s", INCLUDE_DIR, d);
                        fr = fopen(path, "r");
                    }
                }

                if (fr == NULL) {
                    printf("No header file found: %s\n", d);
                } else {
                    int rch;
                    while ((rch = fgetc(fr)) != EOF) {
                        fputc(rch, fw);
                    }
                    fclose(fr);
                }

                while (ch != '\n' && ch != EOF) {
                    ch = fgetc(fp);
                }
            } else {
                fputc('#', fw);
                fputs(s, fw);
                if (ch != EOF) {
                    fputc(ch, fw);
                }
                while (ch != '\n' && ch != EOF) {
                    ch = fgetc(fp);
                    if (ch != EOF) {
                        fputc(ch, fw);
                    }
                }
            }
        } else {
            fputc(ch, fw);
            while (((ch = fgetc(fp)) != '\n') && ch != EOF) {
                fputc(ch, fw);
            }
            if (ch == '\n') {
                fputc(ch, fw);
            } else if (ch == EOF) {
                break;
            }
        }
    }
}