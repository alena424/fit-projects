/**
* @file main.c
* Implementation of IFJ17 imperative language compiler
*
* @brief main file
*
* @date 18.11.2017
*
* @author Alena Tesarova (xtesar36)
* @author Jan Sorm (xsormj00)
* @author Daniel Uhricek (xuhric00)
* @author Peter Uhrin (xuhrin02)
*
*/

#include <stdio.h>
#include <stdlib.h>
#include "lex.h"
#include "parser.h"

int main () {

   // Initialize lexical analysis
   file_init(stdin);

   // Start syntactical analysis
   parser();

   return 0;
}
