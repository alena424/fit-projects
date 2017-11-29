/**
* @file htab_free.c
* Reseni IJC-DU2, priklad b)
*
* @date 16.4.2017
* @author Alena Tesarova, FIT
*
* @description Funkce htab_free
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "htab.h"


/**
* @brief uvolneni hashovaci tabulky
* @param table hashovaci tabulka
*/
void htab_free(htab_t *table)
{
    if ( table != NULL )
    {
        free(table);
    }
}

/*** Konec souboru htab_free.c ***/
