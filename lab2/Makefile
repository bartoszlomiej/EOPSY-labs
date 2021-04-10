CC = gcc
CFLAGS = -Wall -Wextra

tsig:	tsig.o
	$(CC) $(CFLAGS) -o tsig tsig.o

tsig.o:	tsig.c

clean:
	$(RM) tsig *.o *~
