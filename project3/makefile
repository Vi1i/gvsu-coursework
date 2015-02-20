
CC = gcc
CFLAGS = -g -Wall -std=c99
LDFLAGS = -lm

OBJS = stack.o infix2postfix.o driver.o

build: driver clean

driver: ${OBJS}
	${CC} ${LDFLAGS} -o $@ ${OBJS}
	
stack.o: stack.c stack.h
	${CC} ${CFLAGS} -c stack.c
	
infix2postfix.o: infix2postfix.c infix2postfix.h stack.h
	${CC} ${CFLAGS} -c infix2postfix.c
	
driver.o: driver.c infix2postfix.h
	${CC} ${CFLAGS} -c driver.c

clean:
	rm -f $(OBJS)

	