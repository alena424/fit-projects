/**
* @file gen_stack.h
* Implementation of IFJ17 imperative language compiler
*
* @brief Stack declarations header file for generating
*
* @date 05.12.2017
*
* @author Alena Tesarova (xtesar36)
* @author Jan Sorm (xsormj00)
* @author Daniel Uhricek (xuhric00)
* @author Peter Uhrin (xuhrin02)
*
*/

#ifndef _GENSTACK_H
#define _GENSTACK_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
* @brief structure of one element of stack
*/
typedef struct genStackElem {
   int data;
   struct genStackElem * nextElem;
} genStackElem;

/**
* @brief structure of stack
*/
typedef struct genStack {
   genStackElem *top;
} genStack;

/**
* @brief stack inicialize
*/
genStack* genStackInit ();

/**
* @brief release stack
* @param genStack stack
*/
void genStackDispose(genStack *s);

/**
* @brief push element on stack
* @param genStack stack
*/
bool genStackPush(genStack *s, int data);

/**
* @brief pop element from stack
* @param genStack stack
* @return data if we else otherwise -1
*/
int genStackPop(genStack *s);

/**
 * @brief get top element from stack
 * @param get genStack *s pointer to stack
 * @return int data
 */
int genStackTop(genStack *s);

#endif
