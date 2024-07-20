CC=gcc
OS := $(shell uname -s)

default: all

all: clean s21_cat s21_grep

s21_cat: cat/main.c cat/s21_cat.c
	gcc cat/main.c cat/s21_cat.c -o cat/s21_cat

s21_grep: grep/main.c grep/s21_grep.c
	gcc grep/main.c grep/s21_grep.c -o grep/s21_grep -lpcre
 
test: s21_cat s21_grep
	$(CC) -o test test.c
	touch log1.txt
	touch log2.txt
	./test
 
check:
	cp ../materials/linters/CPPLINT.cfg .
	python3 ../materials/linters/cpplint.py --extension=c cat/s21_cat.* cat/main.c  grep/s21_grep.* grep/main.c test.c
ifeq ($(OS), Darwin)
	CK_FORK=no leaks -atExit -- ./test
else
	valgrind --leak-check=full ./test
endif

clean:
	rm -f grep/s21_grep cat/s21_cat test log*.txt *cfg
 
rebuild: clean all