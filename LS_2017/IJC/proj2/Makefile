##############################
#							 #
# Makefile for proj2 IJC	 #
# @author Alena Tesarova	 #
# @login xtesar36			 #
# @date 13.4.2017			 #
#							 #
##############################

CC 			= gcc
CFLAGS 		= -std=c99 -pedantic -Wall -Wextra 

CXX			= g++
CXXFLAGS	= -std=c++11 -Wall -Werror -Wextra -pedantic
CDYNFLAGS 	= -std=c99 -fPIC -Wall  -pedantic
all: tail tail2 wordcount wordcount-dynamic

### hlavni ###

tail: tail.c
	$(CC) $(CFLAGS) tail.c -o tail 

tail2: tail2.cc
	$(CXX) $(CXXFLAGS) tail2.cc -o tail2

### staticky ###
wordcount: wordcount.o htab.h libhtab.a io.o
	$(CC) $(CFLAGS) -static wordcount.c libhtab.a io.o -L. -o wordcount  
### dynamicky ###
wordcount-dynamic: wordcount.c htab.h libhtab.so io.c io.h
	$(CC) $(CDYNFLAGS) -o $@ $^ -L. -lhtab

### objekty ###
tail.o: tail.c
	$(CC) $(CFLAGS) -o tail.o -c tail.c

tail2.o: tail2.cc
	$(CXX) $(CXXFLAGS) -c  tail2.cc -o tail2.o

libhtab.o: libhtab.c io.c htab.h io.h
	$(CC) -o $@ -c $< 

io.o: io.c io.h
	$(CC) -o $@ -c $<

### Pomocne objektove soubory pro dyn. knihovnu
htab_remove-d.o: htab_remove.c htab.h
	$(CC) $(CDYNFLAGS) -o $@ -c $<
htab_init-d.o: htab_init.c htab.h
	$(CC) $(CDYNFLAGS) -o $@ -c $<
htab_lookup_add-d.o: htab_lookup_add.c htab.h
	$(CC) $(CDYNFLAGS) -o $@ -c $<
htab_free-d.o: htab_free.c htab.h 
	$(CC) $(CDYNFLAGS) -o $@ -c $<
htab_clear-d.o: htab_clear.c htab.h
	$(CC) $(CDYNFLAGS) -o $@ -c $<
htab_foreach-d.o: htab_foreach.c htab.h
	$(CC) $(CDYNFLAGS) -o $@ -c $<
hash_function-d.o: hash_function.c htab.h
	$(CC) $(CDYNFLAGS) -o $@ -c $<
LIBDYN= htab_remove-d.o htab_foreach-d.o htab_init-d.o htab_lookup_add-d.o htab_free-d.o htab_clear-d.o hash_function-d.o hash_function-d.o

#### Pomocne objektove soubory por statickou knihovnu ###
wordcount.o: wordcount.c htab.h
	$(CC) $(CFLAGS) -o $@ -c $<
htab_remove-s.o: htab_remove.c htab.h
	$(CC) $(CFLAGS) -o $@ -c $<
htab_init-s.o: htab_init.c htab.h
	$(CC) $(CFLAGS) -o $@ -c $<
htab_lookup_add-s.o: htab_lookup_add.c htab.h
	$(CC) $(CFLAGS) -o $@ -c $< 
htab_free-s.o: htab_free.c htab.h 
	$(CC) $(CFLAGS) -o $@ -c $<
htab_clear-s.o: htab_clear.c htab.h
	$(CC) $(CFLAGS) -o $@ -c $<
htab_foreach-s.o: htab_foreach.c htab.h
	$(CC) $(CFLAGS) -o $@ -c $<
hash_function-s.o: hash_function.c htab.h
	$(CC) $(CFLAGS) -o $@ -c $<
LIBSTA=wordcount.o htab_remove-s.o htab_foreach-s.o htab_init-s.o htab_lookup_add-s.o htab_free-s.o htab_clear-s.o hash_function-s.o 


### knihovny ###
### staticka ###
libhtab.a: $(LIBSTA) htab.h
	ar rcs libhtab.a $^
#ranlib libhtab.a

### dynamicka ###
libhtab.so: $(LIBDYN) htab.h
	$(CC) $(CFLAGS) -shared -o $@ $^

### clean ###	
	
clean:
	rm -rf *.o tail tail2 wordcount *.a *.so

### distro ###

distro:
	rm -f xtesar36.zip
	zip xtesar36.zip *.c *.h *.cc Makefile


### Vysledky, porovnani dynamicka a staticka knihovna casy ###
# staticka
# real 0,296s
# user 0,217s
#
# dynamicka
# real 0,327s
# user 0,248
#
# spousteno na merlinovi 25.4.2017
#
###############################################################
