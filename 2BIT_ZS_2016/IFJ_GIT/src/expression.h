/**
 * @file expressions.h
 * Implementation of IFJ17 imperative language compiler
 *
 * @brief syntax analysis for expressions
 *
 * @date 11.11.2017
 *
 * @author Alena Tesarova (xtesar36)
 * @author Jan Sorm (xsormj00)
 * @author Daniel Uhricek (xuhric00)
 * @author Peter Uhrin (xuhrin02)
 *
 */

#ifndef _EXPRESSION
#define _EXPRESSION

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lex.h"
#include "error.h"
#include "token.h"
#include "parser.h"
#include "symtable.h"
#include "stack.h"
#include "generator.h"

exprType expression( token_t *token, TSymtable *table );

int isOperator( exprSymbol symbol );
int isRelational( exprSymbol symbol );
exprSymbol decodeToken( token_t* token, tStack *stack );
tExprItem* newExprItem ( exprSymbol symbol, exprType type );
void callError ( token_t *token, tStack *stack, errors error );

#endif
