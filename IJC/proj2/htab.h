
/**
* @file htab.h
* @brief IJC-DU2
*
* @date 16.4.2017
* @author Alena Tesarova, FIT (xtesar36)
*
* @describe Rozhrani nad libhtab.c
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
* @brief structora seznamu jako jedne polozky v hashovaci tabulce
*/
struct htab_listitem
{
    const char *key;
    int data;
    struct htab_listitem *next;
};

/**
* @brief struktura hashovaci tabulky
*/
typedef struct htab_t
{
    unsigned int velikost;
    size_t n;
    struct htab_listitem *list[];
} htab_t;


htab_t *htab_init(int size);

size_t htab_size(htab_t *table);

unsigned int htab_bucket_count(htab_t *table);

void htab_clear( htab_t *table );

int htab_remove(htab_t *table, const char *klic);

unsigned int hash_function(const char *str);

void htab_free(htab_t *table);

htab_t *htab_move (int newsize, htab_t *tab2 );

struct htab_listitem *htab_lookup_add(htab_t *table, const char *klic);

struct htab_listitem *htab_find(htab_t *table, const char *klic);

int htab_foreach( htab_t * t, int (*func)( unsigned int, const char *) );


/*** Konec souboru htab.h ***/
