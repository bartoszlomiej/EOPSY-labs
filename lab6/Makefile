CC = gcc
CFLAGS = -Wall -Wextra

copyfile:	copyfile.o
	$(CC) $(CFLAGS) -o copyfile copyfile.o

copyfile.o:	copyfile.c

clean:
	$(RM) copyfile *.o *~
