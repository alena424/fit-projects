
/**
* @file htab_remove.c
* @brief IJC-DU2, priklad b)
*
* @date 16.4.2017
* @author Alena Tesarova, FIT
*
* @description Funkce htab_remove
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "htab.h"


/**
* @brief vyhledavani a zruseni zadane polozky
* @return false pokud neexistuje
*/
int htab_remove(htab_t *table, const char *klic)
{
    //porchazeni seznamem a mazani polozek

    //kdyz zname klic, hashovaci funkce nam vrati index
    unsigned index = hash_function(klic) % table->velikost;
    struct htab_listitem *list = table->list[ index ];
    struct htab_listitem *pomlist = table->list[ index ];
    int first = 1;

    while ( list != NULL )
    {
        //vyhledavame nasi polozku
        if ( strcmp( list->key, klic ) == 0 )
        {
            //zkontrolovat prvni prvek v poli
            //nasli jsme!
            //musime vyhodit z tabulky
            //table->list[i] = NULL;
            if ( first )
            {
                table->list[index] = list->next;
            }
            else
            {
                pomlist->next = list->next;
            }

            free((char*)list->key);
            free(list);
            return EXIT_SUCCESS;
        }
        first = 0;
        pomlist = list;
        list = list->next;
    }
    //nenasli jsme
    return EXIT_FAILURE;
}
/*** Konec souboru htab_remove.c ***/
