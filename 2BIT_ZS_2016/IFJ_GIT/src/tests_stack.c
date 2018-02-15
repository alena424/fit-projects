/**
* @file tests_les.c
* Implementation of IFJ17 imperative language compiler
*
* @brief tests for lexical analyzator
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
#include "expression.h"

//colors
#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */

#define EXPR_SYMBOL 17
#define EXPR_TYPE 5

char *exprSymbolStr[] =
{
    "e_plus", "e_minus", "e_mul", "e_div", "e_int_div",
    "e_l_bracket", "e_r_bracket",
    "e_eq", "e_ne", "e_lt", "e_gt", "e_le", "e_ge",
    "e_identifier",
    "e_unary_minus",
    "e_dollar",
    "e_expression"
};

exprSymbol symbol_enum[] =
{
    e_plus, e_minus, e_mul, e_div, e_int_div,
    e_l_bracket, e_r_bracket,
    e_eq, e_ne, e_lt, e_gt, e_le, e_ge,
    e_identifier,
    e_unary_minus,
    e_dollar,
    e_expression
} ;



char *exprTypeStr[] =
{
    "ex_void", "ex_integer", "ex_double", "ex_string", "ex_bool"
};

exprType type_enum[] =
{
    ex_void, ex_integer, ex_double, ex_string, ex_bool
} ;

tStack *s;

void printStack( tStack *s)
{
    printf( CYAN "STACK PRINT\n" RESET);
    //printf("===========================\n");
    if (s != NULL )
    {
        tStackElement list = s->top;
        int index_type = 0;
        int index_symbol = 0;
        while (list != NULL)
        {
            // print data
            for (int i = 0; i < EXPR_SYMBOL; i++)
            {
                if ( symbol_enum[i] == list->data->symbol )
                {
                    index_symbol = i;
                }
            }
            for (int j = 0; j < EXPR_TYPE; j++)
            {
                if ( type_enum[j] == list->data->type )
                {
                    index_type = j;
                }
            }
            printf("(%s, %s) -> ", exprSymbolStr[index_symbol], exprTypeStr[index_type]);
            list = list->nextPtr;
        }
        printf("NULL");
      //  printf("\n===========================\n\n");
    }
    else
    {
        printf("NULL");
    }
    printf("\n");
}
int main()
{
    // inicializujeme
    printf("~~~Stack init~~~\n\n");
    s = stackInit(MAX_STACK);
    printStack(s);
    printf("Is Stack empty?\n");
    if ( stackEmpty(s) )
    {
        printf(GREEN "CORRECT: Stack is empty\n" RESET);
    }
    else
    {
        printf( RED "ERROR\n" RESET);
    }
    printf("Size of stack is: %d\n", stackSize(s));
    printf( "\n~~~Pushing e_dollar a ex_void to stack~~~\n\n");
    tExprItem  *tmp, *pop, *pop2, *one, *two;
    tmp = newExprItem( e_dollar, ex_void );
    stackPush(s, tmp);

    printStack(s);
    printf("\n~~~Pushing more data structures~~~\n\n");
    one =  newExprItem( e_identifier, ex_integer );
    two = newExprItem( e_identifier, ex_double );
    stackPush(s, one);
    stackPush(s, two);

    printStack(s);
    printf("\n~~~Pop 2 items~~~\n\n");
    pop = stackPop(s);
    pop2 = stackPop(s);
    printStack(s);	
    free(pop);
    free(pop2);
    printf("\n~~~FREE~~~\n\n");
    printStack(s);
    printf("\n~~~Delete and free stack~~~\n\n");
    stackDispose(s);
    printStack(s);


    return 0;
}
