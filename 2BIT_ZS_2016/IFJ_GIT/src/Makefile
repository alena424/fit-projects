#####################################
# Makefile for IFJ project 
#  @date 5.11.2017
#  
#  @author Alena Tesarova (xtesar36)
#  @author Jan Sorm (xsormj00)
#  @author Daniel Uhricek (xuhric00)
#  @author Peter Uhrin (xuhrin02)
#
#  ###################################

CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -Wextra

.PHONY = clean

all: main

# run all tests we have
test: tests symt stack gener lex

lex:
	./tests_lex

symt:
	./tests_symtable 

stack:
	./tests_stack

gener: 
	./tests_generator

tests: tests_symtable tests_lex tests_stack tests_generator tests_gen_stack

debug: main.o parser.o error.o stack.o gen_stack.o expression.o lex.o symtable.o generator.o
	$(CC) -fsanitize=address -g $(CFLAGS) -o main $^ -lm

main: main.o parser.o error.o stack.o gen_stack.o expression.o lex.o symtable.o generator.o
	$(CC) $(CFLAGS) -o $@ $^ -lm

# object files

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# tests

tests_symtable: tests_symtable.o symtable.o 
	$(CC) $(CFLAGS) -o $@ $^

tests_lex: tests_lex.o lex.o
	$(CC) $(CFLAGS) -o $@ $^

tests_stack: tests_stack.o error.o stack.o expression.o lex.o symtable.o generator.o
	$(CC) $(CFLAGS) -o $@ $^ -lm

tests_generator: tests_generator.o generator.o
	$(CC) $(CFLAGS) -o $@ $^ 

tests_gen_stack: tests_gen_stack.o gen_stack.o
	$(CC) $(CFLAGS) -o $@ $^ 


clean:
	rm -rf *.o tests_symtable tests_lex tests_stack tests_generator main
