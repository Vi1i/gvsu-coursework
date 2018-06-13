all: chow

clean:
	rm parser.cpp parser.hpp chow tokens.cpp

parser.cpp: parser.y
	bison -d -o $@ $^

parser.hpp: parser.cpp

tokens.cpp: tokens.l parser.hpp
	lex -o $@ $^

chow: parser.cpp main.cpp tokens.cpp
	g++ -std=c++11 -o $@ *.cpp
