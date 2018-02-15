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

#include "gen_stack.h"

/**
* @brief stack inicialize 
*/
genStack* genStackInit ()
{
    genStack *s = malloc( sizeof ( genStack ));
    if ( s == NULL )
    {
        return NULL;
    }
    s->top = NULL;
    return s;
}

/**
 * @brief free a stack
 */
void genStackFree( genStack *s ){

    if (s != NULL){
        free(s);
        s = NULL;
    }
}

/**
* @brief release stack
* @param genStack stack
*/
void genStackDispose(genStack *s)
{
    // if stack is empty we dont have to release it
    if ( s != NULL )
    {
        genStackElem *iter = s->top;
        genStackElem *deletedItem;
        while ( iter != NULL )
        {
            deletedItem = iter;
            iter = iter->nextElem;
            free(deletedItem);
        }
        s->top = NULL;
        genStackFree(s);
    }
}

/**
* @brief push a value to stack
* @param genStack stack
* @param data data to push
*/
bool genStackPush(genStack *s, int data)
{
    genStackElem *item = malloc( sizeof ( struct genStackElem ) );
    if ( item == NULL )
    {
        return false; // havent successeded
    }
    item->data = data;
    item->nextElem = s->top;
    s->top = item; // set pointer top to new element
    return true;
}



/**
* @brief pop element from stack
* @param genStack stack
* @return data if we have otherwise returns -1
*/
int genStackPop(genStack *s)
{
    if ( s == NULL || s->top == NULL )
    {
        return -1; // no data
    }

    genStackElem *item = s->top; //top item
    int data_pom = item->data;
    s->top = s->top->nextElem;
    free(item);
    return data_pom;

}

/**
 * @brief get top element from stack
 * @param get genStack *s pointer to stack
 * @return int data
 */
int genStackTop(genStack *s) {

   if (s == NULL || s->top == NULL) {
      return -1; 
   }

   return s->top->data;
}



