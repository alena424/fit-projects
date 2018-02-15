/**
* @file generator.c
* Implementation of IFJ17 imperative language compiler
*
* @brief Implementation generator of internal code
*
* @date 23.11.2017
*
* @author Alena Tesarova (xtesar36)
* @author Jan Sorm (xsormj00)
* @author Daniel Uhricek (xuhric00)
* @author Peter Uhrin (xuhrin02)
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "generator.h"

instrList globalInstrList;

/**
 *  @brief  Function for initialization of instruction list
 *          before first use.
 *
 *  @param  l   instrList*   instruction list
 */
void instrListInit(instrList* l) {
   l->First = NULL;
   l->Last = NULL;
   l->Act = NULL;
}

/**
 *  @brief  Function for basic appending to instruction list
 *
 *  @param  l   instrList*   instruction list
 *  @param  instr    char*   instruction data
 *  @param  str1     char*   first part of 3AC
 *  @param  str2     char*   second part of 3AC
 *  @param  str3     char*   third part of 3AC
 */
void instrListAppend(instrList* l, char* instr, char* str1, char* str2, char* str3) {

   // Allocating new element
   instrElem* newElemPtr = (instrElem *) malloc(sizeof(instrElem));

   if (newElemPtr == NULL)
      return;

   // Allocating memory for instruction string 
   // 3 number of spaces
   int totalLength = 3;

   if (instr != NULL) totalLength += strlen(instr);
   if (str1 != NULL) totalLength += strlen(str1);
   if (str2 != NULL) totalLength += strlen(str2);
   if (str3 != NULL) totalLength += strlen(str3);

   newElemPtr->str = (char *) malloc(sizeof(char) * (totalLength + 1));

   // Filling data
   if (instr != NULL) strcpy(newElemPtr->str, instr);
   strcat(newElemPtr->str, " ");

   if (str1 != NULL) strcat(newElemPtr->str, str1);
   strcat(newElemPtr->str, " ");

   if (str2 != NULL) strcat(newElemPtr->str, str2);
   strcat(newElemPtr->str, " ");

   if (str3 != NULL) strcat(newElemPtr->str, str3);
   newElemPtr->next = NULL;

   newElemPtr->str[totalLength] = '\0';


   // Inserting first element
   if (l->First == NULL) {
      l->First = newElemPtr; 
   } else {
      // Inserting into nonempty list
      l->Last->next = newElemPtr;
   }

   // Setting new end pointer */
   l->Last = newElemPtr;

}

/**
 *  @brief  Function for appending instruction using labels
 *          into instruction list. (eg. labels, jumps...)
 *
 *  @param  l  *instrList   instruction list
 *  @param  fun     *char   function name
 *  @param  name    *char   {funname, while, if, else, endif}
 *  @param  idx     *char   counter
 */
void instrListAppendInstrLabel(instrList* l, char* instr, char* name, int idx) {

   // Allocating new element
   instrElem* newElemPtr = (instrElem *) malloc(sizeof(instrElem));

   if (newElemPtr == NULL)
      return;

   // Buffer - each int should fit in 12 bits, last for '\0'
   char numstr[13] = {0};

   // Converting int to string
   sprintf(numstr, "%d", idx);

   // Allocating memory for instruction string 
   int totalLength = strlen(instr) + strlen(" $") + strlen(name) + strlen(numstr);
   newElemPtr->str = (char *) malloc(sizeof(char) * (totalLength + 1));

   // Filling data
   strcpy(newElemPtr->str, instr);
   strcat(newElemPtr->str, " $");
   strcat(newElemPtr->str, name);
   if (idx >= 0) {
      // Will be -1 for function names
      strcat(newElemPtr->str, numstr);
   }
   newElemPtr->str[totalLength] = '\0';
   newElemPtr->next = NULL;

   // Inserting first element
   if (l->First == NULL) {
      l->First = newElemPtr;
   } else {
      // Insering into nonempty list
      l->Last->next = newElemPtr;
   }

   // Setting new end pointer
   l->Last = newElemPtr;

}


/**
 *  @brief  Function for deleting first element in list.
 *
 *  @param  l   instrList*   instruction list
 */
void instrListDeleteFirst(instrList* l) {
   
   // Empty list
   if (l->First == NULL)
      return;

   instrElem* elemPtr = l->First;

   // If first active, activity is canceled 
   if (l->First == l->Act)
      l->Act = NULL;

   // If it was only element
   if (l->First == l->Last) {
      l->First = NULL;
      l->Last = NULL;
   } else {
      // Wasnt only one, change beginning 
      l->First = l->First->next;
   }

   // Freeing memory
   free(elemPtr->str);
   free(elemPtr);

}


/**
 *  @brief  Function to append function
 *
 *  @param  l   instrList*   instruction list
 *  @param  fun instrList*   functions instruction list
 */
void instrListAppendFunction(instrList* l, instrList* fun) {
   // Instr list is nonempty
   if (l->Last != NULL) {
      l->Last->next = fun->First; 
   } else {
      // Instr list is emptpy and appendend list will be the only one
      l->First = fun->First; 
   }
}


/**
 *  @brief  Function to print all elements of instruction list.
 *
 *  @param  l   instrList*   instruction list
 */
void instrListPrintAll(instrList* l) {
   for (instrElem* iter = l->First; iter != NULL; iter = iter->next) {
      printf("%s\n", iter->str);
   }
}

/**
 *  @brief  Function to free all the data from instruction list 
 *
 *  @param  l   instrList*   instruction list
 */
void instrListClear(instrList* l) {
   while (l->First != NULL)
      instrListDeleteFirst(l);
}
