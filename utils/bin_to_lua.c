#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BIN_BUFF_SIZE 1024
#define TXT_BUFF_SIZE BIN_BUFF_SIZE * 4  // 1 bin char with decimal code XYZ (<256) == \ + X + Y + Z  =  4 txt characters max

#define EOF_CH          26


#define NEWLINE         10
#define CARRIAGE_RETURN 13
#define DBL_QUOTES      34
#define BACKSLASH       92

#define NL              "\\n"
#define CR              "\\r"
#define DBL_Q           "\\\""
#define BSLH            "\\\\"


int main (int argc, char *argv[]){
    if (argc < 2) {
        printf("Binary file expected\n");
        return 1;

    } else if (argc != 3) {
        printf("Invalid usage! Usage: 1st arg = <binary file>, 2nd arg = <output file name>\n");
        return 2;
    }



    FILE* fbinary = fopen(argv[1], "rb");
    FILE* flua    = fopen(argv[2], "w" );

    unsigned char bin_buf[BIN_BUFF_SIZE], txt_buf[TXT_BUFF_SIZE];
    unsigned char chr, *escaped_chr;

    int i, chrs_cnt, chunk_size;



    fprintf(flua, "return \"");

    while (chunk_size = fread(bin_buf, 1, BIN_BUFF_SIZE, fbinary)) {
        chrs_cnt = 0;
        for (i=0; i < chunk_size; i++){
            chr = bin_buf[i];

            // apply C-escape style for some type of characters 
            if (
                chr >= 128     ||   // - extended ASCII characters
                chr == EOF_CH       // - Lua interpreter have troubles with EOF character
            ) {
                chrs_cnt += sprintf(txt_buf + chrs_cnt, "\\%d", chr);
                continue;
            }
            
            // escape character with extra backslash or write it as is
            switch (chr){
                case NEWLINE:           escaped_chr = NL;    break;
                case CARRIAGE_RETURN:   escaped_chr = CR;    break;
                case BACKSLASH:         escaped_chr = BSLH;  break;
                case DBL_QUOTES:        escaped_chr = DBL_Q; break;
                default: // write it as is
                    *(txt_buf + chrs_cnt++) = chr;
                    continue;   // go for the next character in binary buffer
            }

            chrs_cnt += sprintf(txt_buf + chrs_cnt, escaped_chr);
        }
        fwrite(txt_buf, 1, chrs_cnt, flua);
    }

    fprintf(flua, "\"");


    int retcode;

    if (feof(fbinary)){
        retcode = 0;
    
    } else if (ferror(fbinary)) {
        puts("I/O error occurred");
        retcode = 3;
        
    } else {
        puts("wtf!? ferror(fbinary)  &&  feof(fbinary)  == false   (!!)");
        retcode = 4;
    }
        
    fclose(fbinary);
    fclose(flua);
    
    return retcode;
}