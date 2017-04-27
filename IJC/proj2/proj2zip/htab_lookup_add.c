/**
* @file htab_lookup_add.c
* Reseni IJC-DU2, priklad b)
*
* @date 16.4.2017
* @author Alena Tesarova, FIT
*
* @description Funkce htab_lookup_add
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "htab.h"

/**
* @brief  V tabulce  t  vyhleda zaznam odpovidajici retezci  key
* @return pokud nalezne key vraci ukazatel na zaznam, pokud nenalezne, automaticky prida zaznam a vrati ukazatel
*/
struct htab_listitem *htab_lookup_add(htab_t *table, const char *klic)
{
    //najdeme index
    unsigned int index = hash_function(klic) % table->velikost;
    struct htab_listitem *zaznam = ( struct htab_listitem* )malloc(sizeof( struct htab_listitem ) );
    if ( zaznam == NULL )
    {
        return NULL;
    }
    struct htab_listitem *prevlist = table->list[index];
    struct htab_listitem *list = table->list[index];
    char * klicpom = (char *) malloc( ( strlen(klic) +1) );

    //inicializace promennych
    strcpy(klicpom, klic);
    zaznam->key = klicpom;
    zaznam->data = 1;
    zaznam->next = NULL;

    //porjdeme seznamem a najdeme zaznam
    //pokud je seznam prazdny, vytvorime prvni prvek
    if ( list == NULL )
    {
        table->list[index] = zaznam;
        return table->list[index];
    }
    while( list != NULL)
    {
        if ( strcmp(list->key, klic ) == 0 )
        {
            //nasli jsme, zvysime pocet vyskytu
            list->data += 1;
            return list;
        }
        prevlist = list;
        list = list->next;
    }
    //nenasli jsme, pridame nakonec seznamu
    prevlist->next = zaznam;
    list = prevlist;
    table->list[index] = list;
    return table->list[index];
}

/*** Konec souboru htab_lookup_add.c ***/
