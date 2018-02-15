/**
* @file generator.h
* Implementation of IFJ17 imperative language compiler
*
* @brief File contains declarations needed for generator of internal code
*
* @date 23.11.2017
*
* @author Alena Tesarova (xtesar36)
* @author Jan Sorm (xsormj00)
* @author Daniel Uhricek (xuhric00)
* @author Peter Uhrin (xuhrin02)
*
*/

#ifndef _GENERATOR
#define _GENERATOR

#define INSTR_LEN 16

/**
 * Element in which is one instruction stored
 */
typedef struct instrElem {
   char* str;               // Data of instruction
   struct instrElem *next;  // Next instruction
} instrElem;

/**
 * Linked list for storing isntructions
 */
typedef struct instrList {
   instrElem* First;        // First element in list
   instrElem* Last;         // Last element in lsit
   instrElem* Act;          // Currently selected element in list
} instrList;

extern instrList globalInstrList;

void instrListInit(instrList* l);

void instrListAppend(instrList* l, char* instr, char* str1, char* str2, char* str3);

void instrListAppendInstrLabel(instrList* l, char* instr, char* name, int idx);

void instrListAppendFunction(instrList* l, instrList* fun);

void instrListDeleteFirst(instrList* l);

void instrListPrintAll(instrList* l);

void instrListClear(instrList* l);



#endif
