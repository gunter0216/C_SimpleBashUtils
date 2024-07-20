#include <stdio.h>
#include <stdlib.h>
#include "s21_grep.h"
#include <string.h>
#include <pcre.h>
#include <regex.h>

int s21_strstr(char *str1, char *str2, int reg);
char *readStringFromFile(char **_string, FILE *file, char *symbol, int *_size, int *line_feed);
int findRegularString(char *string, char *findString);
int findRegularStringFromFile(char *string, char *findString);
void grep2(char **_argv, int *arrFind, int countFind, int countFiles, FILE *file, char *fileName,
            int flag_e, int flag_i, int flag_v, int flag_c,
            int flag_l, int flag_n, int flag_h, int flag_o, int flag_f, char *regular);
void trimString(int **_arrFind, int *_countFind, int **_arrFiles, int *_countFiles, int _argc, char **_argv,
                int *_flag_e, int *_flag_i, int *_flag_v, int *_flag_c, int *_flag_l,
                int *_flag_n, int *_flag_s, int *_flag_h, int *_flag_o, int *_flag_f,
                char **regular, int *error);


void grep(int _argc, char **_argv) {
    int *arrFind;
    int *arrFiles;
    int countFiles;
    int countFind;
    int flag_e = 0;
    int flag_i = 0;
    int flag_v = 0;
    int flag_c = 0;
    int flag_l = 0;
    int flag_n = 0;
    int flag_s = 0;
    int flag_h = 0;
    int flag_o = 0;
    int flag_f = 0;
    char *regular = NULL;
    int error = 0;
    trimString(&arrFind, &countFind, &arrFiles, &countFiles, _argc, _argv,
            &flag_e, &flag_i, &flag_v, &flag_c, &flag_l,
            &flag_n, &flag_s, &flag_h, &flag_o, &flag_f, &regular, &error);
    if (error) {
        printf("%s\n", _argv[2]);
    }
    char *fileName;
    FILE *file;
    for (int i = 0; i < countFiles; i++) {
        fileName = _argv[arrFiles[i]];
        file = fopen(fileName, "r");
        if (file != NULL) {
            grep2(_argv, arrFind, countFind, countFiles, file, fileName,
                flag_e, flag_i, flag_v, flag_c, flag_l, flag_n, flag_h, flag_o, flag_f, regular);
            fclose(file);
        } else {
            if (flag_s == 0)
                printf("grep: %s: No such file or directory\n", fileName);
        }
    }
    free(arrFiles);
    free(arrFind);
    if (regular != NULL)
        free(regular);
}

void grep2(char **_argv, int *arrFind, int countFind, int countFiles, FILE *file, char *fileName,
        int flag_e, int flag_i, int flag_v, int flag_c,
        int flag_l, int flag_n, int flag_h, int flag_o, int flag_f, char *regular) {
    char *string = NULL;
    char symbol;
    int countString = 0;
    int numberOfString = 1;
    do {
        int size = 0;
        int line_feed = 0;
        readStringFromFile(&string, file, &symbol, &size, &line_feed);
        if (feof(file) || symbol == '\n') {
            int find = 0;
            int number_string = 0;
            for (int i = 0; i < countFind; i++) {
                if (flag_f == 1) {
                    find = findRegularStringFromFile(string, regular);
                    if (find != 0)
                        number_string = i;
                } else if (flag_e == 1 && findRegularString(string, _argv[arrFind[i]])) {
                    find = findRegularString(string, _argv[arrFind[i]]);
                    number_string = i;
                } else if (flag_e == 0 && s21_strstr(string, _argv[arrFind[i]], flag_i)) {
                    find = s21_strstr(string, _argv[arrFind[i]], flag_i);
                    number_string = i;
                }
            }
            if (flag_v) {
                if (find == 0)
                    find = 1;
                else
                    find = 0;
            }
            if (find) {
                if (flag_o == 0 && find > 0)
                    find = 1;
                for (int j = 0; j < find; j++) {
                    if (!flag_c && !flag_l) {
                        if (countFiles > 1 && flag_h == 0)
                            printf("%s:", fileName);
                        if (!flag_n) {
                            if (!flag_o) {
                                for (int k = 0; k < size - 1; k++)
                                    printf("%c", string[k]);
                                if (line_feed == 1 || feof(file) == 1 && size - 1 > 0)
                                    printf("\n");
                            } else {
                                printf("%s\n", _argv[arrFind[number_string]]);
                            }
                        } else {
                            printf("%d:%s\n", numberOfString, string);
                        }
                    }
                }
                countString++;
            }
            numberOfString++;
        }
    } while (!feof(file));
    if (flag_c && !flag_l)
        printf("%d\n", countString);
    if (flag_l && countString != 0)
        printf("%s\n", fileName);
    if (string != NULL)
        free(string);
}

int findRegularString(char *string, char *findString) {
    const unsigned char *tables = pcre_maketables();
    pcre *re;
    int options = 0;
    const char *error;
    int erroffset;
    re = pcre_compile((char*)findString, options, &error, &erroffset, NULL);
    int ovector[10];
    int count = pcre_exec(re, NULL, (char *)string, strlen(string), 0, 0, ovector, 10);
    pcre_free((void *) re);
    pcre_free((void *) tables);
    int temp = 0;
    if (count >= 0)
        temp = count;
    return temp;
}

int findRegularStringFromFile(char *string, char *findString) {
    int temp = 0;
    regex_t regex;
    regmatch_t pmatch[1];
    if (regcomp(&regex, findString, REG_EXTENDED) == 0) {
        if (!regexec(&regex, string, sizeof(pmatch) / sizeof(pmatch[0]), pmatch, 0))
            temp = 1;
    }
    regfree(&regex);
    return temp;
}

char *readStringFromFile(char **_string, FILE *file, char *symbol, int *_size, int *line_feed) {
    if (*_string != NULL)
        free(*_string);
    char *string = (char*)malloc(1);
    int size = 1;
    do {
        *symbol = fgetc(file);
        if (*symbol == '\n')
            *line_feed = 1;
        if (!feof(file) && *symbol != '\n') {
            string = (char*)realloc(string, size + 1);
            string[size - 1] = *symbol;
            size++;
        }
    } while (!feof(file) && *symbol != '\n');
    string[size - 1] = '\0';
    *_string = string;
    *_size = size;
    return string;
}

int s21_strstr(char *str1, char *str2, int reg) {
    int temp = 0;
    int find = 0;
    int IFi = strlen(str1) - strlen(str2) + 1;
    int IFj = strlen(str2);
    for (int i = 0; i < IFi; i++) {
        temp = 1;
        for (int j = 0; j < IFj; j++) {
            int x = str1[i + j];
            int y = str2[j];
            if (reg) {
                if (x >= 'A' && x <= 'Z')
                    x = x - 'A' + 'a';
                if (y >= 'A' && y <= 'Z')
                    y = y - 'A' + 'a';
            }
            if (x != y)
                temp = 0;
        }
        if (temp)
            find++;
    }
    return find;
}

void trimString(int **_arrFind, int *_countFind, int **_arrFiles, int *_countFiles, int _argc, char **_argv,
                int *_flag_e, int *_flag_i, int *_flag_v, int *_flag_c, int *_flag_l,
                int *_flag_n, int *_flag_s, int *_flag_h, int *_flag_o, int *_flag_f,
                char **regular, int *error) {
    int flag_e = 0;
    *_countFind = 0;
    *_countFiles = 0;
    int *arrFind = (int*)malloc(sizeof(int));
    int *arrFiles = (int*)malloc(sizeof(int));
    for (int i = 1; i < _argc; i++) {
        if (_argv[i][0] == '-') {
            for (int j = 1; _argv[i][j]; j++) {
                if (_argv[i][j] == 'e')
                    *_flag_e = 1;
            }
        }
    }
    for (int i = 1; i < _argc; i++) {
        if (_argv[i][0] == '-' && flag_e == 0) {
            for (int j = 1; _argv[i][j]; j++) {
                if (_argv[i][j] == 'i') {
                    *_flag_i = 1;
                } else if (_argv[i][j] == 'v') {
                    *_flag_v = 1;
                } else if (_argv[i][j] == 'c') {
                    *_flag_c = 1;
                } else if (_argv[i][j] == 's') {
                    *_flag_s = 1;
                } else if (_argv[i][j] == 'o') {
                    *_flag_o = 1;
                } else if (_argv[i][j] == 'f') {
                    *_flag_f = 1;
                } else if (_argv[i][j] == 'h') {
                    *_flag_h = 1;
                } else if (_argv[i][j] == 'l') {
                    *_flag_l = 1;
                } else if (_argv[i][j] == 'n') {
                    *_flag_n = 1;
                } else if (_argv[i][j] == 'e') {
                    *_flag_e = 1;
                    flag_e = 1;
                }
            }
        } else if (*_flag_f == 1 && *regular == NULL) {
            FILE *file = fopen(_argv[i], "r");
            if (file != NULL) {
                char symbol;
                int size, line_feed;
                readStringFromFile(regular, file, &symbol, &size, &line_feed);
                *_countFind = *_countFind + 1;
                fclose(file);
            } else {
                *error = 1;
            }
        } else if (flag_e == 1 || (*_flag_e == 0 && *_countFind == 0)) {
            *_countFind = *_countFind + 1;
            arrFind = (int*)realloc(arrFind, (*_countFind) * sizeof(int));
            arrFind[(*_countFind) - 1] = i;
            flag_e = 0;
        } else {
            arrFiles = (int*)realloc(arrFiles, ++(*_countFiles) * sizeof(int));
            arrFiles[(*_countFiles) - 1] = i;
        }
    }
    *_arrFiles = arrFiles;
    *_arrFind = arrFind;
}
