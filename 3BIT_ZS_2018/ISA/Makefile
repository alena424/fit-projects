CC=g++
CFLAGS = -std=c++11 -Wall #-Werror -Wextra #-pedantic

all: myripsniffer myripresponse

myripresponse: myripresponse.o response.o
	$(CC) $(CFLAGS) $^ -o $@ -lpcap

response.o: response.c response.h headers.h
	$(CC) $(CFLAGS) -c $< -o $@ 

myripresponse.o: myripresponse.c headers.h
	$(CC) $(CFLAGS) -c $< -o $@

myripsniffer: sniffer.o  myripsniffer.o 
	$(CC) $(CFLAGS) $^ -o $@ -lpcap 

myripsniffer.o: myripsniffer.c headers.h sniffer_h.h
	$(CC) $(CFLAGS) -c $< -o $@ 

sniffer.o: sniffer.c sniffer_h.h
	$(CC) $(CFLAGS) -c $< -o $@
 
clean:
	rm -rf *.o myripsniffer myripresponse
