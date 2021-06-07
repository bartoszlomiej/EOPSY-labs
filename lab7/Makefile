CC = gcc
CFLAGS = -Wall -Wextra -pthread

philosophers: philosophers.o
	$(CC) $(CFLAGS) -o philosophers philosophers.o

clean:
	$(RM) philosophers *.o *~
