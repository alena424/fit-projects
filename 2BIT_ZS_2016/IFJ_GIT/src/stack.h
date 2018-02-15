/**
* @file stack.h
* Implementation of IFJ17 imperative language compiler
*
* @brief Stack declarations header file
*
* @date 15.11.2017
*
* @author Alena Tesarova (xtesar36)
* @author Jan Sorm (xsormj00)
* @author Daniel Uhricek (xuhric00)
* @author Peter Uhrin (xuhrin02)
*
*/

#ifndef _STACK_H_
#define _STACK_H_


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_STACK 50


// hodnoty, ktere budeme ukladat na zasobnik
typedef enum {
	e_plus, e_minus, e_mul, e_div, e_int_div,
	e_l_bracket, e_r_bracket,
	e_eq, e_ne, e_lt, e_gt, e_le, e_ge,
	e_identifier,
	e_unary_minus,
	e_dollar,
	e_expression
} exprSymbol;

typedef enum {
	ex_void, ex_integer, ex_double, ex_string, ex_bool
} exprType;

// prvek, co se bude vkladat do zasobniku
typedef struct texpritem {
	exprSymbol symbol;
	exprType type;
} tExprItem;

typedef struct tStackElement{
    struct tStackElement *nextPtr; //next element
    tExprItem* data;

} *tStackElement;

typedef struct{
    unsigned int size; //max_size of stack
    tStackElement top;

} tStack;


/**
* @brief stack inicialize and set top of stack
* @param tStack stack
*/
tStack* stackInit( unsigned int max_size);

/**
* @brief release stack
* @param tStack stack
*/
void stackDispose(tStack* s );

/**
* @brief push address on stack
* @param tStack stack
* @return false if malloc fails, TRUE otherwise
*/
bool stackPush(tStack* s, tExprItem* tExprItem);

/**
* @brief get top value on stack, this functions doesnt pop value
* @param tStack stack
* @return address to tExprItem
*/
tExprItem* stackTop(tStack* s);

/**
* @brief get and pop top value on stack
* @param tStack stack
* @return address to tExprItem
*/
tExprItem* stackPop(tStack* s);


/**
* @brief is stack empty
* @param tStack stack
* @return TRUE value if stack is empty, otherwise return false
*/
bool stackEmpty(tStack* s);

/**
* @brief get stack size
* @param tStack stack
* @return size
*/
unsigned int stackSize(tStack* s);


#endif
