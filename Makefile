CC=gcc
CFLAGS=-Wall -g
LIBS=-lfl

myParser.exe: lex.yy.c myhtml.tab.c
	$(CC) $(CFLAGS) -o myParser.exe lex.yy.c myhtml.tab.c $(LIBS)

lex.yy.c: myhtml.l myhtml.tab.h
	flex myhtml.l

myhtml.tab.c myhtml.tab.h: myhtml.y
	bison -d myhtml.y

clean:
	rm -f myParser.exe lex.yy.c myhtml.tab.c myhtml.tab.h 