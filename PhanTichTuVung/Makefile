CFLAGS = -c -Wall
CC = gcc
LIBS =  -lm 

all: scanner

scanner: scanner.o reader.o charcode.o token.o error.o writer.o
	${CC} scanner.o reader.o charcode.o token.o error.o writer.o -o scanner

reader.o: reader.c
	${CC} ${CFLAGS} reader.c

writer.o: writer.c
	${CC} ${CFLAGS} writer.c

scanner.o: scanner.c
	${CC} ${CFLAGS} scanner.c

charcode.o: charcode.c
	${CC} ${CFLAGS} charcode.c

token.o: token.c
	${CC} ${CFLAGS} token.c

error.o: error.c
	${CC} ${CFLAGS} error.c

clean:
	rm -f *.o *~
