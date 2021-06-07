CC = gcc
CFLAGS = -Wall -Wextra -lpthread

philosophers: philosophers.o
	$(CC) $(CFLAGS) -o philosophers philosophers.o

clean:
	$(RM) philosophers *.o *~
