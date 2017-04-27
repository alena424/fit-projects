
/**
* @file libhtab.c
* @brief IJC-DU2, priklad b)
*
* @date 16.4.2017
* @author Alena Tesarova, FIT (xtesar36)
*
* @describe modul s funkcemi pracujici nad hashovaci tabulkou
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> //isspace()

#include "htab.h"


/**
* @brief hashovaci funkce zadana
* @param str string, ze ktereho se vytvori cislo, podle ktereho budeme indexovat v hashovaci tabulce
*/

unsigned int hash_function(const char *str){
    unsigned int h=0;
    const unsigned char *p;
    for(p=(const unsigned char*)str; *p!='\0'; p++)
        h = 65599*h + *p;
    return h;
}

/**
* @brief inicializace tabulky
* @param velikost Velikost hashovaci tabulky
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



/**
* @return pocet prvku tabulky
*/
size_t htab_size(htab_t *table)
{
    return table->n;
}

/**
* @return pocet prvku pole
*/
unsigned int htab_bucket_count(htab_t *table)
{
    return table->velikost;
}

/**
* @brief zruseni vsech polozek, tabulka zustane prazdna
* @param hashovaci tabulka
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

/**
* @brief konstruktor: vytvoreni a inicializace tabulky daty z tabulky t2, t2 nakonec zustane prazdna a alokovana
* @return nova tabulka table
*/
htab_t *htab_move (int newsize, htab_t *tab2 )
{
    //alokujeme novou tabulku do ktere budeme kopirovat data
    htab_t *table = htab_init(newsize);
    if ( table == NULL ){
        fprintf(stderr, "Can not allocate memory for another hash table\n");
        return NULL;
    }
    table->velikost = tab2->velikost;
    table->n = tab2->n;

    for( unsigned int i = 0; i < tab2->velikost; i++ ){
        struct htab_listitem *list = tab2->list[i];
        struct htab_listitem *list2 = NULL;
        //table->list[i] = malloc(sizeof( htab_listitem);
        while( list != NULL ){
            struct htab_listitem *zaznam = ( struct htab_listitem* )malloc(sizeof( struct htab_listitem ) );
            //if
            zaznam->key = list->key;
            zaznam->data = list->data;
            zaznam->next = NULL;

            if ( table->list[i] == NULL ){
                //prvni zaznam
                list2 = zaznam;
            }else{
                list2->next = zaznam;
                list2 = list2->next;
            }
            list = list->next;

        }
         table->list[i] = list2;
    }
    htab_clear(tab2);
    return table;
}

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
/**
* @brief jako htab_lookup_add, ale bez alokace
* @return pokud nenalezne klic, vrací NULL
*/
struct htab_listitem *htab_find(htab_t *table, const char *klic) {
    unsigned int index = hash_function(klic) % table->velikost;
    struct htab_listitem *list = table->list[index];
    while( list != NULL ){
            if ( strcmp( list->key, klic ) == 0 ){
                //nasli jsme
                list->data += 1;
                return list;
            }
        list = list->next;
    }
    return NULL;
}

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
/*** Konec libhtab.c ***/


