#
# Subor:       makefile
# Datum:       24.03.2014
# Autor:       Martin Hučko, xhucko00@stud.fit.vutbr.cz (FIT VUT)
# Projekt:     IJC-DU1 a)
# Prekladac:   gcc (Ubuntu/Linaro 4.8.1-10ubuntu9) 4.8.1
# Popis:       Makefile pre IJC-DU1
#

# Prekladac
CC=gcc

# Parametre prekladu
CFLAGS=-O2 -std=c99 -Wall -Wextra -pedantic -lm

# Vytvori vsetky potrebne subory
all: prvocisla prvocisla-inline steg-decode steg-encode

# Zavislosti pre program prvocisla
prvocisla: prvocisla.o eratosthenes.o error.o
	$(CC) $(CFLAGS) prvocisla.o eratosthenes.o error.o -lm -o $@
prvocisla.o: prvocisla.c bit-array.h eratosthenes.h
	$(CC) $(CFLAGS) -c prvocisla.c -o $@
eratosthenes.o: eratosthenes.c bit-array.h eratosthenes.h
	$(CC) $(CFLAGS) -c eratosthenes.c -o $@

# Zavislosti pre inline verziu programu prvocisla
prvocisla-inline: prvocisla-inline.o eratosthenes-inline.o error.o
	$(CC) $(CFLAGS) prvocisla-inline.o eratosthenes-inline.o error.o -lm -o $@
prvocisla-inline.o: prvocisla.c bit-array.h eratosthenes.h
	$(CC) $(CFLAGS) -DUSE_INLINE -c prvocisla.c -o $@
eratosthenes-inline.o: eratosthenes.c bit-array.h eratosthenes.h
	$(CC) $(CFLAGS) -DUSE_INLINE -c eratosthenes.c -o $@

# Zavislosti pre program steg-decode
steg-decode: steg-decode.o eratosthenes.o error.o ppm.o
	$(CC) $(CFLAGS) steg-decode.o eratosthenes.o error.o ppm.o -lm -o $@
steg-decode.o: steg-decode.c eratosthenes.h error.h ppm.h
	$(CC) $(CFLAGS) -c steg-decode.c -o $@

# Zavislosti pre program steg-encode
steg-encode: steg-encode.o eratosthenes.o error.o ppm.o
	$(CC) $(CFLAGS) steg-encode.o eratosthenes.o error.o ppm.o -lm -o $@
steg-encode.o: steg-encode.c eratosthenes.h error.h ppm.h
	$(CC) $(CFLAGS) -c steg-encode.c -o $@

# Ostatne zavislosti
bit-array.h: error.o
error.o: error.c error.h
	$(CC) $(CFLAGS) -c error.c -o $@
ppm.o: ppm.c ppm.h error.h
	$(CC) $(CFLAGS) -c ppm.c -o $@

.PHONY:
	clean clean-all

# Odstrani vygenerovane objekty
clean:
	rm -f *.o

# Odstrani vygenerovane objekty a subory
clean-all:
	rm -f *.o prvocisla prvocisla-inline steg-decode steg-encode