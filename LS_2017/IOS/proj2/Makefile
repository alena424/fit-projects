#################################################
#						#
# @brief IOS, proj2 - process (using pthread)	#
# @author Alena Tesarova (xtesar36)		#
# @date 27.4.2015				#
#						#			
#################################################

CFLAGS= -std=gnu99 -Wall -Wextra -Werror -pedantic -pthread 

all: proj2

proj2: proj2.c proj2.h
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f *.o proj2 /dev/shm/sem.* *.out

disto:
	zip proj2.c proj.h Makefile
