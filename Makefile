
OBJS = lex.yy.o calc.tab.o ast.o matcher.o symtable.o compute.o
CC = clang
CFLAGS = -Wall

.SUFFIXES: .c .o

calc: $(OBJS) main.o
	$(CC) -o calc $^

.c.o:
	$(CC) $(CFLAGS) -c $<

lex.yy.c: calc.l calc.tab.h
	flex calc.l

calc.tab.h: calc.tab.o
calc.tab.c: calc.y
	bison calc.y

.PHONY: test
test: $(OBJS) test.o
	$(CC) -o test $^
	./test
	rm -rf test test.o