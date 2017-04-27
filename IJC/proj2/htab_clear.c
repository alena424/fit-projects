
/**
* @file htab_clear.c
* Reseni IJC-DU2, priklad b)
*
* @date 16.4.2017
* @author Alena Tesarova, FIT
*
* @description Funkce htab_clear
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "htab.h"


/**
*  @brief zruseni vsech polozek, tabulka zustane prazdna
* @param table hashovaci tabulka
*/
void htab_clear( htab_t *table )
{
    for (unsigned int i = 0; i < table->velikost; i++)
    {
        struct htab_listitem *list = table->list[i];
        if ( table->list[i] != NULL)
        {
            htab_remove(table,list->key);
        }
        list = NULL;
    }
}

/*** Konec souboru htab_clear.c ***/
