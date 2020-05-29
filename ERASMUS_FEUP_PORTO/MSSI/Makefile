#
# @project MSSI - brewery simulation
#
# @file main.cpp
# @author Alena Tesarova (xtesar36)

CC		= g++
FLAGS	= -std=c++11 -Wall -Wextra -Werror -g -pedantic -O2

.PHONY: clean build experiments run archive


build:	brewery #pridameTanky staceniVikend vareniVikend brewery5

brewery: main.o
	$(CC) $(FLAGS) -o $@ $^ -l simlib

main.o: main.cpp main.hpp
	$(CC) $(FLAGS) -c $<


clean:
	rm -f main.o brewery main.o pridameTanky mainTanky.o staceniVikend mainSmeny.o vareniVikend mainVareni.o brewery5 mainPomer.o
