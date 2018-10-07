CC=g++
CFLAGS = -std=c++11 -Wall #-Werror -Wextra #-pedantic

all: myripsniffer

myripsniffer: myripsniffer.c
	$(CC) $(CFLAGS) -o myripsniffer myripsniffer.c -lpcap

clean:
	rm -rf *.o myripsniffer
