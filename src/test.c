#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void compareFiles(char *str);
void test_cat(char *_str);
void test_grep(char *_str);
void test1();
void test2();
void createFile();

int main() {
    // createFile();
    test1();
    test2();
    return 0;
}

void createFile() {
    FILE *file = fopen("text.txt", "w");
    for (int i = 0; i < 256; i++)
        fprintf(file, "%c\n", i);
    fclose(file);
}

void test1() {
#if defined __linux__
    test_cat("--number-nonblank text2.txt");
    test_cat("--number text2.txt");
    test_cat("--squeeze-blank text2.txt");
    test_cat("text.txt");
    test_cat("-b text.txt");  //  5
    test_cat("-e text.txt");
    test_cat("-E text.txt");
    test_cat("-n text.txt");
    test_cat("-s text.txt");
    test_cat("-t text.txt");  // 10
    test_cat("-T text.txt");
    test_cat("-b text.txt text2.txt");
    test_cat("-e text.txt text2.txt");
    test_cat("-E text.txt text2.txt");
    test_cat("-n text.txt text2.txt");  // 15
    test_cat("-s text.txt text2.txt");
    test_cat("-t text.txt text2.txt");
    test_cat("-T text.txt text2.txt");
#endif
    test_cat("text2.txt");
    test_cat("-b text2.txt");
    test_cat("-e text2.txt");
    test_cat("-E text2.txt");
    test_cat("-n text2.txt");
    test_cat("-s text2.txt");
    test_cat("-t text2.txt");
    test_cat("-T text2.txt");
    test_cat("-b text2.txt text4.txt");
    test_cat("-e text2.txt text4.txt");
    test_cat("-E text2.txt text4.txt");
    test_cat("-n text2.txt text4.txt");
    test_cat("-s text2.txt text4.txt");
    test_cat("-t text2.txt text4.txt");
    test_cat("-T text2.txt text4.txt");
    test_cat("text3.txt");
    test_cat("-b text3.txt");
    test_cat("-e text3.txt");
    test_cat("-E text3.txt");
    test_cat("-n text3.txt");
    test_cat("-s text3.txt");
    test_cat("-t text3.txt");
    test_cat("-T text3.txt");
    test_cat("-b text3.txt text4.txt");
    test_cat("-e text3.txt text4.txt");
    test_cat("-E text3.txt text4.txt");
    test_cat("-n text3.txt text4.txt");
    test_cat("-s text3.txt text4.txt");
    test_cat("-t text3.txt text4.txt");
    test_cat("-T text3.txt text4.txt");
}

void test2() {
#if __linux__
    test_grep("1 text.txt");
    test_grep("-e [0-9] -e q text.txt");
    test_grep("-i 1 text.txt");
    test_grep("-v 1 text.txt");
    test_grep("-c 1 text.txt");
    test_grep("-l 1 text.txt");
    test_grep("-n 1 text.txt");
    test_grep("-h 1 text.txt");
    test_grep("-s 1 text999.txt");
    test_grep("-o 1 text.txt");
#endif
    test_grep("1 text2.txt");
    test_grep("-e [0-9] -e q text2.txt");
    test_grep("-i qwe text2.txt");
    test_grep("-v qwe text2.txt");
    test_grep("-c qwe text2.txt");
    test_grep("-l qwe text2.txt");
    test_grep("-n qwe text2.txt");
    test_grep("-h qwe text2.txt");
    test_grep("-s qwe text2.txt");
    test_grep("-o qwe text2.txt");
    test_grep("1 text3.txt");
    test_grep("-e [0-9] -e q text3.txt");
    test_grep("-i qwe text3.txt");
    test_grep("-v qwe text3.txt");
    test_grep("-c qwe text3.txt");
    test_grep("-l qwe text3.txt");
    test_grep("-n qwe text3.txt");
    test_grep("-h qwe text3.txt");
    test_grep("-s qwe text3.txt");
    test_grep("-o qwe text3.txt");
    test_grep("-f flagF.txt text3.txt");
    test_grep("1 text4.txt");
    test_grep("-e [0-9] -e q text4.txt");
    test_grep("-i qwe text4.txt");
    test_grep("-v qwe text4.txt");
    test_grep("-c qwe text4.txt");
    test_grep("-l qwe text4.txt");
    test_grep("-n qwe text4.txt");
    test_grep("-h qwe text4.txt");
    test_grep("-s qwe text4.txt");
    test_grep("-o qwe text4.txt");
    test_grep("-f flagF.txt text4.txt");
}

void compareFiles(char *str) {
    static int count = 0;
    count++;
    FILE *file1 = fopen("log1.txt", "r");
    FILE *file2 = fopen("log2.txt", "r");
    char symbol1;
    char symbol2;
    int error = 0;
    if (file1 != NULL && file2 != NULL) {
        while (feof(file1) == 0 && feof(file2) == 0) {
            symbol1 = fgetc(file1);
            symbol2 = fgetc(file2);
            if (symbol1 != symbol2) {
                error = 1;
            }
        }
        if (feof(file1) != feof(file2))
            error = 1;
        fclose(file1);
        fclose(file2);
    }
    if (error == 1) {
        printf("Error: test %d, %s\n", count, str);
    } else {
        printf("Success: test %d\n", count);
    }
}

void test_cat(char *_str) {
    char *str1 = (char*)malloc(100);
    char *str2 = (char*)malloc(100);
    str1[0] = '\0';
    str2[0] = '\0';
    str1 = strcat(str1, "cat/./s21_cat ");
    str2 = strcat(str2, "cat ");
    str1 = strcat(str1, _str);
    str2 = strcat(str2, _str);
    str1 = strcat(str1, " > log1.txt");
    str2 = strcat(str2, " > log2.txt");
    system(str1);
    system(str2);
    compareFiles(str1);
    free(str1);
    free(str2);
}

void test_grep(char *_str) {
    char *str1 = (char*)malloc(100);
    char *str2 = (char*)malloc(100);
    str1[0] = '\0';
    str2[0] = '\0';
    str1 = strcat(str1, "grep/./s21_grep ");
    str2 = strcat(str2, "grep -a ");
    str1 = strcat(str1, _str);
    str2 = strcat(str2, _str);
    str1 = strcat(str1, " > log1.txt");
    str2 = strcat(str2, " > log2.txt");
    system(str1);
    system(str2);
    compareFiles(str1);
    free(str1);
    free(str2);
}
