/**
* @file tests_gen_stack.c
* Implementation of IFJ17 imperative language compiler
*
* @brief tests for stack
*
* @date 20.10.2017
*
* @author Alena Tesarova (xtesar36)
* @author Jan Sorm (xsormj00)
* @author Daniel Uhricek (xuhric00)
* @author Peter Uhrin (xuhrin02)
*
*/

#include "gen_stack.h"

void printStack(genStack *s)
{
    if (s != NULL)
    {
        genStackElem *item = s->top; //top item
        while( item != NULL)
        {
            printf("%d ",item->data);
            item = item->nextElem;
        }
        printf("\n");

    }

}

int main()
{
    printf("Hello world!\n");
    genStack *s;
    s = genStackInit();
    genStackPush(s, 15);
    genStackPush(s, 11);
    genStackPush(s, 10);
    genStackPush(s, 1);
    printStack(s);
    int pom = genStackPop(s);
	printf("Pop value is %d\n", pom);
    pom = genStackPop(s);
	printf("Pop value is %d\n", pom);
    printStack(s);
	genStackPush(s, 333);
    genStackDispose(s);


    return 0;
}
