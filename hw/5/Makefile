all: dot

dot.tab.c dot.tab.h:	dot.y
	bison -v -d dot.y

lex.yy.c: dot.l dot.tab.h
	flex dot.l

dot: lex.yy.c dot.tab.c dot.tab.h
	g++ -lfl -std=c++11 -o chow dot.tab.c lex.yy.c

clean:
	rm chow dot.tab.c lex.yy.c dot.tab.h dot.output
