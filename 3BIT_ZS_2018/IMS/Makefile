
CC		= g++
FLAGS	= -std=c++11 -Wall -Wextra -Werror -g -pedantic -O2

.PHONY: clean build experiments run archive

# dalsi experimenty jsou zakomentovane
build:	brewery #pridameTanky staceniVikend vareniVikend brewery5

brewery: main.o
	$(CC) $(FLAGS) -o $@ $^ -l simlib

main.o: main.cpp main.hpp
	$(CC) $(FLAGS) -c $<

pridameTanky: mainTanky.o
	$(CC) $(FLAGS) -o $@ $^ -l simlib

mainTanky.o: mainTanky.cpp mainTanky.hpp
	$(CC) $(FLAGS) -c $<

staceniVikend: mainSmeny.o
	$(CC) $(FLAGS) -o $@ $^ -l simlib

mainSmeny.o: mainSmeny.cpp mainSmeny.hpp
	$(CC) $(FLAGS) -c $<

vareniVikend: mainVareni.o
	$(CC) $(FLAGS) -o $@ $^ -l simlib

mainVareni.o: mainVareni.cpp mainVareni.hpp
	$(CC) $(FLAGS) -c $<

brewery5: mainPomer.o
	$(CC) $(FLAGS) -o $@ $^ -l simlib

mainPomer.o: mainPomer.cpp mainPomer.hpp
	$(CC) $(FLAGS) -c $<


clean:
	rm -f main.o brewery main.o pridameTanky mainTanky.o staceniVikend mainSmeny.o vareniVikend mainVareni.o brewery5 mainPomer.o
