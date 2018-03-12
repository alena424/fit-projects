############
# Makefile #
# ##########
#
# @author Alena Tesarova
# @date 02.03.2018
# IPK - 1. projekt, zadani I.
#
# ###############################
#
#
# -static-libstdc++ aby to slo prelozit na eve
#
#################################

all: ipk-client ipk-server

ipk-client: ipk-client.cpp
	g++ -static-libstdc++ -std=c++11 ipk-client.cpp -o ipk-client
ipk-server: ipk-server.cpp
	g++ -static-libstdc++ -std=c++11 ipk-server.cpp -o ipk-server 

clean:
	rm -rf *.o ipk-client ipk-server
