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

CC=gcc
CFLAGS= #-Wall -std=c99 -pedantic -lm

all: client server

client: client.c
	$(CC) $(CFLAGS) client.c -o client

server: server.c
	$(CC) $(CFLAGS) server.c -o server
clean:
	rm -rf *.o client server
