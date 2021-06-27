#
# BIN - cellular automata - majority problem
# 
# @author Alena Tesařová (xtesar36)
# @date 16.4.2021
#

CPP=g++
CPPFLAGS=-Wall -std=gnu++11 -g
.PHONY: ca_major.cpp ca_sim.hpp params.hpp utils.cpp

all: ca_major
ca_gp: ca_major.cpp ca_sim.hpp params.hpp utils.cpp
	$(CPP) $(CPPFLAGS) -o $@ $^

zip:
	zip xtesar36.zip ca_major.cpp ca_sim.hpp Makefile
