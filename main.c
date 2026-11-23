#include "header.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename.c>\n", argv[0]);
        return 1;
    }

    char out[256];
    strcpy(out, argv[1]);

    int len = strlen(out);
    if (len >= 2 && out[len - 2] == '.' && out[len - 1] == 'c') {
        out[len - 2] = '\0';
    }
    strcat(out, ".i");

    FILE *fp = fopen(argv[1], "r");
    FILE *fw = fopen("stage1", "w");
    comment_removal(fp, fw);
    fclose(fp);
    fclose(fw);

    fp = fopen("stage1", "r");
    fw = fopen("stage2", "w");
    file_inclusion(fp, fw);
    fclose(fp);
    fclose(fw);

    fp = fopen("stage2", "r");
    fw = fopen(out, "w");
    macro_handler(fp, fw);
    fclose(fp);
    fclose(fw);

    remove("stage1");
    remove("stage2");

    return 0;
}