############
# Makefile #
# ##########
#
# @author Alena Tesarova
# @date 28.03.2018
# IPK - 2. projekt, zadani II.
#
# ###############################
FLAGS = -std=gnu99 -Wall

all: ipk-dhcpstarve 

ipk-dhcpstarve: ipk-dhcpstarve.o dhcp.o udp.o
	gcc $(FLAGS) $^ -o $@ 

ipk-dhcpstarve.o: ipk-dhcpstarve.c 
	gcc $(FLAGS) -c $< -o $@ 

dhcp.o: dhcp.c dhcp.h 
	gcc $(FLAGS) -c $< -o $@

udp.o: udp.c udp.h
	gcc $(FLAGS) -c $< -o $@

clean:
	rm -rf *.o ipk-dhcpstarve
