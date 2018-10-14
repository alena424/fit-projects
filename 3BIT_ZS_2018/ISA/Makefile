CC=g++
CFLAGS = -std=c++11 -Wall #-Werror -Wextra #-pedantic

all: myripsniffer

myripsniffer: sniffer.o  myripsniffer.o 
	$(CC) $(CFLAGS) $^ -o $@ -lpcap 

myripsniffer.o: myripsniffer.c headers.h sniffer_h.h
	$(CC) $(CFLAGS) -c $< -o $@ 

sniffer.o: sniffer.c sniffer_h.h
	$(CC) $(CFLAGS) -c $< -o $@
 
clean:
	rm -rf *.o myripsniffer
