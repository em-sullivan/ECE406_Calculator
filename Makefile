TARGET = ${T}
OBJS = ${T}.o stack.o calc.o
CFLAGS = -Wall -o2 -g
CC = gcc

all: ${TARGET}

${TARGET}: ${OBJS}
	${CC} -o ${TARGET} ${OBJS}

clean:
	rm -f ${TARGET} ${OBJS} *core

stack.o: stack.h
calc.o: calc.h
