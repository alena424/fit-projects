/**
* @file parser.h
* Implementation of IFJ17 imperative language compiler
*
* @brief File contains all functions and global variables for syntactic analysis.
*
* 
* @date 07.11.2017
*
* @author Alena Tesarova (xtesar36)
* @author Jan Sorm (xsormj00)
* @author Daniel Uhricek (xuhric00)
* @author Peter Uhrin (xuhrin02)
*
*/

#ifndef _PARSER
#define _PARSER

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "lex.h"
#include "error.h"
#include "token.h"
#include "expression.h"
#include "symtable.h"

TSymtable *globalTable;

void check ( token_type_t type, errors error );
void next();
void getAndCheck( token_type_t type, errors error );
void freeAndError( errors error );

void program();
void function();
void parameters( int mode, htab_listitem *fce );
void parameters2( int mode, htab_listitem *fce, TParam *par );
void scope();
void statementList( bool scope );
void statement( bool scope );
void print( bool scope );
void print2( bool scope );
void arguments( bool scope, htab_listitem *fce );
void arguments2( bool scope, htab_listitem *fce, TParam *par );
void inicialization( bool scope, htab_listitem *var, Type_data typeId );
void elsif( bool scope );

void parser();

#endif
