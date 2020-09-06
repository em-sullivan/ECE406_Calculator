CORESRC = ../Core/Src/
COREINC = ../Core/Inc/
OBJS = ${CORESRC}stack.o ${CORESRC}calc.o
CFLAGS = -Wall -o2 -g -I ${COREINC}
CC = gcc

all: stacktest postfixtest intoposttest printtest

stacktest: ${OBJS} stacktest.o
	${CC} -o stacktest ${OBJS} stacktest.o

postfixtest: ${OBJS} postfixtest.o
	${CC} -o postfixtest ${OBJS} postfixtest.o

intoposttest: ${OBJS} intoposttest.o
	${CC} -o intoposttest ${OBJS} intoposttest.o

printtest: ${OBJS} printtest.o
	${CC} -o printtest ${OBJS} printtest.o

clean:
	rm -f  ${OBJS} *.o stacktest postfixtest intoposttest printtest *core

stack.o: stack.h
calc.o: calc.h
