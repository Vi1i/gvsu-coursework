#
# 'make depend' uses makedepend to automatically generate dependencies 
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'mycc'
# 'make clean'  removes all .o and executable files
#

CC = g++
CFLAGS = -g -std=c++11 -Werror -Wall -Wextra -Wimplicit-fallthrough=0
INCLUDES = -I./include
LFLAGS =
LIBS =
SRCS = LexerStream.cpp Lexer.cpp Parser.cpp main.cpp
OBJS = $(SRCS:.c=.o)
	MAIN = chow

.PHONY: depend clean

all:    $(MAIN)
	@echo  Simple lexer named chow has been compiled

$(MAIN): $(OBJS) 
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	$(RM) *.o *~ $(MAIN)

depend: $(SRCS)
	makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it
