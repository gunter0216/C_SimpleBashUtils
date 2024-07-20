CC=gcc
 
default: all
 
all: clean s21_cat

s21_cat: main.c s21_cat.c
	gcc main.c s21_cat.c -o s21_cat
 
check:
	cp ../../materials/linters/CPPLINT.cfg .
	python3 ../../materials/linters/cpplint.py --extension=c s21_cat.* main.c

clean:
	rm -f s21_cat *cfg
 
rebuild: clean all