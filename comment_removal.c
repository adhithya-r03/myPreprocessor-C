#include"header.h"

void comment_removal(FILE *fp,FILE *fw){
    int ch;
    while((ch=fgetc(fp))!=-1){
        if(ch == '"'){
            fputc(ch,fw);
            while(((ch=fgetc(fp))!='"') && ch != -1){
                if(ch == '\\'){
                    fputc(ch,fw);
                    ch = fgetc(fp);
                    if(ch!=-1){
                        fputc(ch,fw);
                        continue;
                    }
                    if(ch == -1)
                        continue;
                }
                fputc(ch,fw);
            }
            if(ch == '"'){
                fputc('"',fw);
                continue;
            }
            if( ch == -1)
                break;
        }
        else if(ch=='/'){
            ch = fgetc(fp);
            if(ch == '/'){
                while(((ch = fgetc(fp)) != '\n') && (ch != -1));
                if(ch=='\n'){
                    fputc('\n',fw);
                }
                if(ch == -1){
                    break;
                }
            }
            else if (ch == '*') {
                int closed = 0;
                while (1) {
                    while (((ch = fgetc(fp)) != '*') && (ch != -1)) {
                    }
                    if (ch == -1) {
                        break;
                    }
                    ch = fgetc(fp);
                    if (ch == '/') {
                        closed = 1;
                        break;
                    }
                    if (ch == EOF) {
                        break;
                    }
                }
                if (closed) {
                    continue;
                }
            }
            else{
                fputc('/',fw);
            }
        }
        if(ch != -1)
            fputc(ch,fw);
    }
}