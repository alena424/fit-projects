#
# Makefile for IJC Project 1 (summer 2016)
#
# @file:		Makefile
# @version:		0.2
# @date:		24.03.2016
# @author:		Martin Pumr
#
# 'make' or 'make all'	: Create executable files
# 'make clean'			: Remove all created executables and obejct files
# 'make distro'			: Pack all source files to archive
#
#
# Legend
#
# $@	: name of target
# $<	: take first dependency
# $^	: set of dependent file
#


#
# Rules
#

# clang:	3.8.0
#CC		= clang
# gcc:		5.3.1
CC		= gcc
# -02		: optimalization level
# -lm		: add behind file which needs library
# -m32		: compile for 32 bit architecture
CFLAGS	= -O2 -std=c99 -Wall -pedantic # -m32
CUSTOM	= -g -Wextra -Werror #-lm

# Dummy targets
.PHONY : all
.PHONY : clean
.PHONY : distro

all : primes primes-inline steg-decode

#
# Executable files
#

primes : primes.c eratosthenes.h eratosthenes.o error.o error.h bit_array.h
	$(CC) $(CFLAGS) -o $@ $^ $(CUSTOM) -DNDEBUG

primes-inline : primes.c eratosthenes-inline.o eratosthenes.h error.o error.h bit_array.h
	$(CC) $(CFLAGS) -o $@ $^ -DUSE_INLINE $(CUSTOM) -DNDEBUG

steg-decode : steg-decode.c ppm.o error.o eratosthenes.o eratosthenes.h bit_array.h
	$(CC) $(CFLAGS) -o $@ $^ $(CUSTOM) -DNDEBUG

#
# Object files
#

eratosthenes.o : eratosthenes.c eratosthenes.h error.o error.h bit_array.h
	$(CC) $(CFLAGS) -o $@ -c $< $(CUSTOM) -DNDEBUG

eratosthenes-inline.o : eratosthenes.c eratosthenes.h error.o error.h bit_array.h
	$(CC) $(CFLAGS) -o $@ -c $< -DUSE_INLINE $(CUSTOM) -DNDEBUG

error.o : error.c error.h
	$(CC) $(CFLAGS) -o $@ -c $< $(CUSTOM) -DNDEBUG

ppm.o : ppm.c ppm.h error.o error.h
	$(CC) $(CFLAGS) -o $@ -c $< $(CUSTOM) -DNDEBUG


#
# Dummy targets
#

clean :
	rm -f primes primes-inline steg-decode
	rm -f eratosthenes.o eratosthenes-inline.o error.o ppm.o

distro :
	rm -f xpumrm00.zip
	zip xpumrm00.zip *.c *.h Makefile

##################šš
#SRCS		= primes.c eratosthenes.c steg-decode.c ppm.c error.c
#INCLUDES	= -I./

#depend: 
#	makedepend -- $(CFLAGS)  -- $(SRCS)
#	makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it

