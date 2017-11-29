/**
* @file countword.c
* @brief IJC-DU2, priklad b)
*
* @date 16.4.2017
* @author Alena Tesarova, FIT
*
* @describe Hlavickovy soubor nad io.c
*
*/

#include <stdio.h>


/**
* @brief Funkce cte jedno slovo ze souboru f do zadaneho pole znaku
* @return vrati delku slova (z delsich slov nacte prvnich max-1 znaku a zbytek preskoci)
* @return funkce vrací EOF, pokud je konec souboru
*/
int get_word(char *s, int max, FILE *f);


/*** Konec souboru io.h ***/
