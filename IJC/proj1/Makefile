############### Makefile #####################
# 1. project for IJC 2017					 #
# 											 #			
# @author Alena Tesarova					 #
#											 #	
##############################################
# 
# $@ - target
# $^ - vsechny zavisloti bez duplicit
CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -Wextra -lm

all: primes primes-i steg_decode

# target - primes
# dependencies - primes.o, bit_array.h
#primes: primes.o eratosthenes.o bit_array.h
#	$(CC) $(CFLAGS) -o $@ $^

primes: primes.o eratosthenes.o error.o
	$(CC) $(CFLAGS) $^ -lm -o $@

steg_decode: steg_decode.c ppm.o error.o bit_array.h eratosthenes.o  
	$(CC) $(CFLAGS) $^  -lm  -o $@

# inline funkce	
primes-i: eratosthenes-i.o primes-i.o eratosthenes.h error.o error.h bit_array.h 
	$(CC) $(FLAGS) primes-i.o eratosthenes-i.o error.o -lm -o $@
primes-i.o: primes.c bit_array.h eratosthenes.h error.o error.h
	$(CC) $(CFLAGS) -DUSE_INLINE -c primes.c -o $@ 
eratosthenes-i.o: eratosthenes.c bit_array.h eratosthenes.h error.o error.h
	$(CC) $(CFLAGS) -DUSE_INLINE -c $< -o $@ 


# Object files
primes.o: primes.c bit_array.h eratosthenes.h 
	$(CC) $(CFLAGS) -c primes.c -o primes.o

eratosthenes.o: eratosthenes.c eratosthenes.h error.o error.h bit_array.h 
	$(CC) $(CFLAGS) -c eratosthenes.c -o $@ 

error.o: error.h error.c
	$(CC) $(CFLAGS) -c error.c -o error.o

ppm.o: ppm.c ppm.h error.o error.h
	$(CC) $(CFLAGS) -c ppm.c -o ppm.o

clean: 
	rm -f primes *.o

distro:
	rm -f xtesar36.zip
	zip xtesar36.zip *.c *.h Makefile

run:
	time ./primes
	time ./primes-i
