CC = gcc
CFLAGS = -Wall -g

myParser.exe: lex.yy.c myhtml.tab.c
	$(CC) $(CFLAGS) -o myParser.exe lex.yy.c myhtml.tab.c -lfl

myhtml.tab.c myhtml.tab.h: myhtml.y
	bison -d myhtml.y

lex.yy.c: myhtml.l myhtml.tab.h
	flex myhtml.l

clean:
	rm -f myParser.exe lex.yy.c myhtml.tab.c myhtml.tab.h 