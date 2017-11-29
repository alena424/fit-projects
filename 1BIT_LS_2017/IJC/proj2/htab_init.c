/**
* @file htab_init.c
* Reseni IJC-DU2, priklad b)
*
* @date 16.4.2017
* @author Alena Tesarova, FIT
*
* @description Funkce htab_init
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "htab.h"

/**
* @param size alokovana velikost tabulky
* @brief inicializace hashovaci tabulky
* @return nova hashovaci tabulka
*/

htab_t *htab_init(int size)
{
    htab_t *table = malloc( sizeof( struct htab_listitem)*size + sizeof( htab_t ) );
    if ( table == NULL )
    {
        return NULL;
    }
    if ( size <= 0 )
    {
        //size must be greater than zero
        return NULL;
    }
    //velikost pole ukazatelu
    table->velikost = size;
    table->n = 0;

    //aktualni pocet je nula zaznamu
    table->n = 0;

    for(int i = 0; i < size; i++)
    {
        table->list[i] = NULL;
    }
    return table;
}

/*** Konec souboru htab_init.c ***/
