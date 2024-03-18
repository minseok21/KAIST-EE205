CC = gcc
CFLAGS = -W -Wall

all: bf fw

bf: bf.o
	$(CC) $(CFLAGS) -o bf bf.o

fw: fw.o
	$(CC) $(CFLAGS) -o fw fw.o

%.o: %.c
	$(CC) $(CFLAGS) $< -c

bf.o: bf.c

fw.o: fw.c

clean:
	@rm *.o bf fw