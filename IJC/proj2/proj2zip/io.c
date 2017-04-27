
/**
* @file io.c
* Reseni IJC-DU2, priklad b)
*
* @date 16.4.2017
* @author Alena Tesarova, FIT
*
* @describe modul pro cteni slova ze souboru
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> //isspace()

#include "io.h"
/**
* @brief Funkce cte jedno slovo ze souboru f do zadaneho pole znaku
* @return vrati delku slova (z delsich slov nacte prvnich max-1 znaku a zbytek preskoci)
* @return funkce vrací EOF, pokud je konec souboru
*/

int get_word(char *s, int max, FILE *f)
{
    int pom;
    int idx = 0;
    if ( s == NULL || f == NULL )
    {
        return EXIT_FAILURE;
    }
    //nactu slovo ze souboru
    while( (pom = fgetc(f)) != EOF )
    {
        if ( ! isspace(pom) )
        {
            if ( idx >= max-1 )
            {
                s[idx] = '\0';
                return idx;
            }
            s[idx] = pom;
            idx++;
        }
        else
        {
            if ( idx )
            {
                s[idx] = '\0';
                return idx;
            }
        }
    }
    if ( idx )
    {
        s[idx] = '\0';
        return idx;
    }
    return EOF;
}
/*** Konec souboru io.c ***/

