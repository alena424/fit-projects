/**
* @file htab_foreach.c
* Reseni IJC-DU2, priklad b)
*
* @date 16.4.2017
* @author Alena Tesarova, FIT
*
* @description Funkce htab_foreach
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "htab.h"

/**
* @brief vola zadanou funkci pro kazdy prvek tabulky
* @param func ukazatel na funkci, ktera ma key string a vraci int (napr. slouzi por tisk dat)
*/
int htab_foreach(htab_t *table, int(*func)(unsigned int , const char * ) )
{
    if ( table == NULL || func == NULL )
    {
        return EXIT_FAILURE;
    }
    for ( unsigned int i = 0; i < table->velikost; i++ )
    {
        struct htab_listitem *zaznam = table->list[i];
        while( zaznam != NULL )
        {
            //printf("%-7 %d\n", zaznam->key, zaznam->pocet);
            func(zaznam->data, zaznam->key);
            zaznam = zaznam->next;
        }
    }
    return EXIT_SUCCESS;

}
/*** Konec souboru htab_foreach.c ***/
