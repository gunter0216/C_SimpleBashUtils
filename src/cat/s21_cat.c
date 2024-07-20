#include <stdio.h>
#include <stdlib.h>
#include "s21_cat.h"
#include <string.h>

void cat2(int countFiles, FILE *file, char *fileName, int *numStr,
    int flag_b, int flag_e, int flag_E, int flag_n, int flag_s, int flag_t, int flag_T);
void trimString(int **_arrFiles, int *_countFiles, int _argc, char **_argv,
    int *_flag_b, int *_flag_e, int *_flag_E, int *_flag_n, int *_flag_s, int *_flag_t, int *_flag_T);

void cat(int _argc, char **_argv) {
    int *arrFiles = NULL;
    int countFiles;
    int flag_b = 0;
    int flag_e = 0;
    int flag_E = 0;
    int flag_n = 0;
    int flag_s = 0;
    int flag_t = 0;
    int flag_T = 0;
    int numStr = 1;
    trimString(&arrFiles, &countFiles, _argc, _argv,
                &flag_b, &flag_e, &flag_E, &flag_n, &flag_s, &flag_t, &flag_T);
    char *fileName;
    FILE *file;
    for (int i = 0; i < countFiles; i++) {
        fileName = _argv[arrFiles[i]];
        file = fopen(fileName, "r");
        if (file != NULL) {
            cat2(countFiles, file, fileName, &numStr, flag_b, flag_e, flag_E, flag_n, flag_s, flag_t, flag_T);
            fclose(file);
        } else {
            printf("cat: %s: No such file or directory\n", fileName);
        }
    }
    free(arrFiles);
}

void cat2(int countFiles, FILE *file, char *fileName, int *numStr,
            int flag_b, int flag_e, int flag_E, int flag_n, int flag_s, int flag_t, int flag_T) {
    static char symbol;
    char oldSymbol;
    char oldOldSymbol;
#if  __linux__
    static int startStr = 1;
#else
    int startStr = 1;
#endif
    do {
        symbol = fgetc(file);
        if (!feof(file)) {
            if (startStr && ((flag_b && symbol != '\n') || (!flag_b && flag_n))) {
                int x = (*numStr);
                int __temp = 6;
                while (x > 0) {
                    __temp--;
                    x /= 10;
                }
                for (int i = 0; i < __temp; i++)
                    printf(" ");
                printf("%d\t", (*numStr)++);
                startStr = 0;
            }
            if (symbol == '\n') {
                startStr = 1;
                if (flag_E || flag_e)
                    putc('$', stdout);
            }
            if ((flag_T || flag_t) && symbol == '\t') {
                putc('^', stdout);
                putc('I', stdout);
            } else if (!(flag_s && oldOldSymbol == '\n' && oldSymbol == '\n' && symbol == '\n')) {
                int sym = (int)symbol;
                if (sym < 0)
                     sym = 128 - (-sym) + 128;
                if ((sym < 32 || sym >= 127) && (flag_t || flag_e)) {
                    if (sym == 9 || sym == 10)
                        printf("%c", sym);
                    else if (sym < 32)
                        printf("^%c", sym + 64);
                    else if (sym == 127)
                        printf("^?");
                    else if (sym > 127 && sym < 160)
                        printf("M-^%c", sym - 64);
                    else if (sym > 159 && sym < 255)
                        printf("M-%c", sym - 128);
                    else if (sym == 255)
                        printf("M-^%c", sym - 192);
                } else {
                    putc(sym, stdout);
                }
            }
            oldOldSymbol = oldSymbol;
            oldSymbol = symbol;
        }
    } while (!feof(file));
    symbol = ' ';
}

void trimString(int **_arrFiles, int *_countFiles, int _argc, char **_argv,
    int *_flag_b, int *_flag_e, int *_flag_E, int *_flag_n, int *_flag_s, int *_flag_t, int *_flag_T) {
    *_countFiles = 0;
    int *arrFiles = (int*)malloc(sizeof(int));
    for (int i = 1; i < _argc; i++) {
        if (_argv[i][0] == '-') {
            if (_argv[i][1] == '-') {
                if (strcmp(_argv[i], "--number-nonblank") == 0) {
                    *_flag_b = 1;
                } else if (strcmp(_argv[i], "--number") == 0) {
                    *_flag_n = 1;
                } else if (strcmp(_argv[i], "--squeeze-blank") == 0) {
                    *_flag_s = 1;
                }
            } else {
                for (int j = 1; _argv[i][j]; j++) {
                    if (_argv[i][j] == 'b') {
                        *_flag_b = 1;
                    } else if (_argv[i][j] == 'e') {
                        *_flag_e = 1;
                    } else if (_argv[i][j] == 'E') {
                        *_flag_E = 1;
                    } else if (_argv[i][j] == 'n') {
                        *_flag_n = 1;
                    } else if (_argv[i][j] == 's') {
                        *_flag_s = 1;
                    } else if (_argv[i][j] == 't') {
                        *_flag_t = 1;
                    } else if (_argv[i][j] == 'T') {
                        *_flag_T = 1;
                    }
                }
            }
        } else {
            arrFiles = (int*)realloc(arrFiles, ++(*_countFiles) * sizeof(int));
            arrFiles[(*_countFiles) - 1] = i;
        }
    }
    *_arrFiles = arrFiles;
}
