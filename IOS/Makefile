CFLAGS= -std=gnu99 -Wall -Wextra -Werror -pedantic -pthread 

all: proj2

proj2: proj2.c 
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f *.o proj2 /dev/shm/sem.* *.out

disto:
	zip proj2.c proj.h Makefile
