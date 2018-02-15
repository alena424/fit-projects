/**
* @file stack.c
* Implementation of IFJ17 imperative language compiler
*
* @brief Functions to stack
*
* @date 20.10.2017
*
* @author Alena Tesarova (xtesar36)
* @author Jan Sorm (xsormj00)
* @author Daniel Uhricek (xuhric00)
* @author Peter Uhrin (xuhrin02)
*
*/

#include "stack.h"
#include <stdbool.h>

/**
* @brief stack inicialize and set top of stack
* @param tStack stack
*/
tStack* stackInit( unsigned int max_size)
{
    tStack *s = malloc( sizeof ( tStack ));
    if ( s == NULL )
    {
        return NULL;
    }
    s->size = max_size;
    s->top = NULL;
    return s;
}

/**
* @brief release stack
* @param tStack stack
*/
void stackDispose(tStack* s )
{
    // if stack is empty we dont have to release it
    if ( s != NULL )
    {
        tStackElement iter = s->top;
        tStackElement deletedItem;
        while ( iter != NULL )
        {
            deletedItem = iter;
            iter = iter->nextPtr;
            free(deletedItem->data);
            free(deletedItem);
        }
        s->top = NULL;
        s->size = 0;

        free(s);
        s = NULL;
    }
}

/**
* @brief push address on stack
* @param tStack stack
*/
bool stackPush(tStack* s, tExprItem* data)
{
    tStackElement item = malloc( sizeof ( struct tStackElement ) ); 
    if ( item == NULL )
    {
        return false; // havent successeded
    }
    item->data = data;
    item->nextPtr = s->top;
    s->top = item; // set pointer top to new element
    return true;
}

/**
* @brief get top value data on stack, this functions doesnt pop value
* @param tStack stack
* @return address to data
*/
tExprItem* stackTop(tStack* s)
{
    return s->top->data;
}

/**
* @brief get and pop top value on stack
* @param tStack stack
* @return address to data
*/
tExprItem* stackPop(tStack* s)
{
    if ( s == NULL || s->top == NULL )
    {
        return NULL; // no data
    }
    tStackElement item = s->top; //top item
    tExprItem *data = item->data; // save to data

    s->top = s->top->nextPtr;
    free(item);
    return data;

}

/**
* @brief is stack empty
* @param tStack stack
* @return true value if stack is empty, otherwise return false
*/
bool stackEmpty(tStack* s)
{
    if ( s == NULL )
    {
        return true;
    }
    if ( s->top )
    {
        return false;
    }
    return true;
}

/**
* @brief get stack size
* @param tStack stack
* @return size
*/
unsigned int stackSize(tStack* s)
{
    return ( s ) ? s->size : 0;
}


