CC=gcc
 
default: all
 
all: clean s21_grep

s21_grep: main.c s21_grep.c
	gcc main.c s21_grep.c -o s21_grep -lpcre
 
check:
	cp ../../materials/linters/CPPLINT.cfg .
	python3 ../../materials/linters/cpplint.py --extension=c s21_grep.* main.c

clean:
	rm -f s21_grep *cfg
 
rebuild: clean all