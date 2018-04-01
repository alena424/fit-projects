############
# Makefile #
# ##########
#
# @author Alena Tesarova
# @date 28.03.2018
# IPK - 2. projekt, zadani II.
#
# ###############################
FLAGS = -std=gnu99 #-Wall

all: ipk-dhcpstarve 
nonmodul: ipk-dhcpstarve-non

ipk-dhcpstarve-non: ipk-dhcpstarve.c
	gcc $(FLAGS) $< -o $@

ipk-dhcpstarve: ipk-dhcpstarve.o dhcp.o lease.o udp.o
	gcc $(FLAGS) $^ -o $@ 

ipk-dhcpstarve.o: ipk-dhcpstarve.c headers.h  
	gcc $(FLAGS) -c $< -o $@ 

dhcp.o: dhcp.c dhcp.h lease.h 
	gcc $(FLAGS) -c $< -o $@

lease.o: lease.c lease.h headers.h
	gcc $(FLAGS) -c $< -o $@

udp.o: udp.c udp.h
	gcc $(FLAGS) -c $< -o $@

clean:
	rm -rf *.o ipk-dhcpstarve
