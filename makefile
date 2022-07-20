CFLAGS = -c -Wall
CC = gcc
LIBS =  -lm 

all: server

server: server.o tfp.o file.o
	${CC} server.o tfp.o file.o -o server
	
server.o : server.c
	${CC} ${CFLAGS} server.c
	
tfp.o : tfp.c
	${CC} ${CFLAGS} tfp.c
	
file.o : file.c
	${CC} ${CFLAGS} file.c

clean:
	rm -f *.o *~


