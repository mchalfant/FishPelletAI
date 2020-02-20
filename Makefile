CC=gcc
CFLAG = -Wall -g

main: fish.c main.c pellet.c
	$(CC) $(CFLAGS) $@.c -o $@.out

clean:
	rm -rf *.o *.out