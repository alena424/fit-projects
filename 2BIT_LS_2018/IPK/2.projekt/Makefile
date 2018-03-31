############
# Makefile #
# ##########
#
# @author Alena Tesarova
# @date 28.03.2018
# IPK - 2. projekt, zadani II.
#
# ###############################

all: ipk-dhcpstarve

ipk-dhcpstarve: ipk-dhcpstarve.c headers.h dhcp.o
	gcc -Wall $^ -o ipk-dhcpstarve

dhcp.o: dhcp.c dhcp.h
	gcc dhcp.c 

object_files:
	gcc 

clean:
	rm -rf *.o ipk-dhcpstarve
