/**
* @file tests_symtable.c
* Implementation of IFJ17 imperative language compiler
*
* @brief tests for hash table operations
*
* @date 14.11.2017
*
* @author Alena Tesarova (xtesar36)
* @author Jan Sorm (xsormj00)
* @author Daniel Uhricek (xuhric00)
* @author Peter Uhrin (xuhrin02)
*
*/

//colors
#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */

#include "symtable.h"

TSymtable* ptrht;
htab_listitem* UNDEFPTR;

#define COUNTER_TYPE 4

Type_data data_types[] =
{
    TInteger, TDouble, TString, Unknown
};

char *data_types_char[] =
{
    "TInteger", "TDouble", "TString", "Unknown"
};

/* tiskne celou tabulku */
void htPrintTable( TSymtable *ptrht )
{
    TParam *parpom;

    printf ("\n------------HASH TABLE--------------\n");
    for ( int i=0; i<TABLE_SIZE; i++ )
    {
        printf ( "%i:",i);
        htab_listitem* ptr = ptrht->symtab_item[i];
        while ( ptr != NULL )
        {
            printf ( CYAN " (%s, " RESET, ptr->name );
            printf( BLUE "counter %d, " RESET, ptr->counter_params);
            printf(  "defined %d, " , ptr->is_defined);
            // zjistime parametr
            int index_wanted = 4; //unknown
            for ( int i = 0; i < COUNTER_TYPE; i++ ){
                if ( ptr->data_type == data_types[i] ){
                    index_wanted = i;
                }
            }
            printf( "type: %s, ", data_types_char[index_wanted] );
            printf( YELLOW "params: " RESET);
            parpom = ptr->params;
            while( parpom != NULL )
            {
                printf("%s, ", parpom->name);
                parpom = parpom->next;
            }
            printf(")  ");
            ptr = ptr->nextPtr;
        }
        printf ("\n");
    }
}

int main()
{
    // init hash table
    printf("init hash table\n");
    ptrht = symtabInit(TABLE_SIZE);
    printf("We try to add a token data name_variable to hash table\n");
    char *name = "name_variable";
    symtabLookAdd(ptrht, name);
    htPrintTable(ptrht);

    printf("We try to add a token data treee to hash table\n");
    char *tree = "treee";
    symtabLookAdd(ptrht, tree);
    htPrintTable(ptrht);

    printf("We add ahoj, blabla, blabla1, blabla2, kukuku\n");

    symtabLookAdd(ptrht, "ahoj");
    symtabLookAdd(ptrht, "blabla");
    symtabLookAdd(ptrht, "blabla1");
    symtabLookAdd(ptrht, "blabla2");
    symtabLookAdd(ptrht, "kukuku");
    htPrintTable(ptrht);

    printf("We add ahoj2, blabla3, blabla4, blabla5, kukuku1\n");

    symtabLookAdd(ptrht, "ahoj2");
    symtabLookAdd(ptrht, "blabla3");
    symtabLookAdd(ptrht, "blabla4");
    symtabLookAdd(ptrht, "blabla5");
    symtabLookAdd(ptrht, "kukuku1");
    htPrintTable(ptrht);

    printf("We search blabla1\n");
    htab_listitem *var2 = symtabFind(ptrht, "blabla1");
    if ( var2 != NULL ){
        printf( GREEN "SUCCEFUL we have found it\n" RESET);
    } else {
       printf( RED "FAIL we have not found it"RESET );
    }

    printf("We add type TInteger to blabla1\n");
    symtabAddType(ptrht, "blabla1", TInteger);
    htPrintTable(ptrht);

    printf("We add params name_param1, name_param2, name_param3, 4, 5, 6 to blabla1\n");
    symtabAddParam(ptrht, "blabla1", TInteger, "name_param1");
    symtabAddParam(ptrht, "blabla1", TDouble, "name_param2");
    symtabAddParam(ptrht, "blabla1", TString, "name_param3");
    symtabAddParam(ptrht, "blabla1", TString, "name_param4");
    symtabAddParam(ptrht, "blabla1", TString, "name_param5");
    symtabAddParam(ptrht, "blabla1", TString, "name_param6");

    symtabAddParam(ptrht, "blabla2", TInteger, "name_param1");
    symtabAddParam(ptrht, "blabla2", TDouble, "name_param2");
    symtabAddParam(ptrht, "blabla2", TString, "name_param3");
    symtabAddParam(ptrht, "blabla2", TString, "name_param4");
    symtabAddParam(ptrht, "blabla2", TString, "name_param5");
    symtabAddParam(ptrht, "blabla2", TString, "name_param6");
    htPrintTable(ptrht);

    printf("We add params name_param4 to nonexisting borec\n");
    htab_listitem *var4 = symtabAddParam(ptrht, "borec", TDouble, "name_param2");
    if ( var4 == NULL ){
        printf( GREEN "SUCCESS returns NULL\n" RESET);
    } else {
        printf( RED "FAIL\n" RESET);
    }
    printf("We add mnamka\n");
    htab_listitem *mnamka = symtabLookAdd(ptrht, "mnamka");

    printf("Lets turn this item to defined\n");
    mnamka->is_defined = true;
     printf("Mnamka name is: %s\n", mnamka->name);
    printf("set to defined \n");
    if ( isDefined( ptrht ) ){
        printf("All table is defined\n");
    } else {
        printf("All table is not defined\n");
    }
    printf("Add 2 params to mnamka\n");
    symtabAddParam(ptrht, "mnamka", TInteger, "name_param1");
    symtabAddParam(ptrht, "mnamka", TDouble, "name_param2");
    printf("Trying to add TDouble to mnamka\n");
    htPrintTable(ptrht);
    symtabAddType(ptrht, "mnamka", TDouble);
    printf("Trying to add TString to mnamka\n");
    symtabAddType(ptrht, "mnamka", TString);
    htPrintTable(ptrht);

    printf("Clean table\n");
    symtabClear(ptrht);
    htPrintTable(ptrht);

    return 0;
}
