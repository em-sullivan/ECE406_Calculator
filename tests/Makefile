OBJS = ../stack.o ../calc.o
CFLAGS = -Wall -o2 -g
CC = gcc

all: stacktest postfixtest intoposttest

stacktest: ${OBJS} stacktest.o
	${CC} -o stacktest ${OBJS} stacktest.o

postfixtest: ${OBJS} postfixtest.o
	${CC} -o postfixtest ${OBJS} postfixtest.o

intoposttest: ${OBJS} intoposttest.o
	${CC} -o intoposttest ${OBJS} intoposttest.o

clean:
	rm -f  ${OBJS} *.o stacktest postfixtest intoposttest *core

stack.o: stack.h
calc.o: calc.h
