CC = gcc
CFLAGS = -Wall -Wextra

philosophers:	philosophers.o
	$(CC) $(CFLAGS) -o philosophers philosophers.o

philosophers.o:	philosophers.c

clean:
	$(RM) philosophers *.o *~
