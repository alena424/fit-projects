/**
* @file countword.c
* Reseni IJC-DU2, priklad b)
*
* @date 16.4.2017
* @author Alena Tesarova, FIT (xtesar36)
*
* @describe
* Pocitani cetnosti slov pomoci hashovaci tabulky
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> //isspace()

#include "htab.h"
#include "io.h"

#define HTAB_SIZE 1000
/**
* Vysvetleni
* Mame kolem 200 000 anglickych slov, pouzijme velikost tabulky 1000,
* kde kazdy seznam bude delky az 200, coz je delka tak mala na to,
* abychom v seznamu velice rychle vyhledavali. Rychlost je dulezitejsi nez alokovana pamet.
*
*/

#define NEWSIZE 200 //delka slouzici k prealokovani
#define SIZE_WORD 127 //ze zadani


/**
* @brief funkce pro tisk
*/
int function(unsigned int data, const char *key)
{
    fprintf(stdout, "%s\t%d\n", key, data);
    return EXIT_SUCCESS;
}

int main()
{
    //promenne
    htab_t *table = htab_init( HTAB_SIZE );
    if ( table == NULL )
    {
        fprintf(stderr, "Can not inicialize hash table\n");
        return EXIT_FAILURE;
    }
    //printf("Velikost pole ukazatelu je %d a pocet prvku v tabulce je %d\n", htab_bucket_count(table), htab_size(table) );
    char slovo[SIZE_WORD]= { 0 };
    //for debugging
    //FILE *f = fopen( "tail.txt", "r" );
    struct htab_listitem *zaznam = NULL;

    while ( get_word(slovo, SIZE_WORD, stdin ) != EOF)
    {
        //printf("%s\n", slovo);
        if ( (zaznam = htab_lookup_add(table, slovo)) == NULL)
        {
            fprintf(stderr, "Not enough memory to inicialize new word\n");
            return EXIT_FAILURE;
        }
        table->n += 1;

        //vymazat slovo
    }
    //printf("Velikost pole ukazatelu je %d a pocet prvku v tabulce je %d\n", htab_bucket_count(table), htab_size(table) );
    if (htab_foreach(table, &function) == EXIT_FAILURE){
        fprintf(stderr, "Table is empty\n");
        return EXIT_FAILURE;
    }
    /* PRO REALOKACI
    htab_t *tabcopy = htab_move(NEWSIZE,table);
    pro kopirovani dalsi tabulky
    printf("\n");
    htab_foreach(tabcopy, &function);
    htab_clear(tabcopy);
    htab_free(tabcopy);
    */

    //htab_t *table2 = NULL;
    //htab_move( NEWSIZE, table2  );

    htab_clear(table);
    htab_free(table);
    table = NULL;

    return 0;
}
