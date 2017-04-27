
/**
* @file hash_function.c
* @brief IJC-DU2, priklad b)
*
* @date 16.4.2017
* @author Alena Tesarova, FIT
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "htab.h"


/**
* @brief hashovaci funkce -- zadana
* @param str string, ze ktereho se vytvori cislo, podle ktereho budeme indexovat v hashovaci tabulce
*/

unsigned int hash_function(const char *str){
    unsigned int h=0;
    const unsigned char *p;
    for(p=(const unsigned char*)str; *p!='\0'; p++)
        h = 65599*h + *p;
    return h;
}

/*** Konec souboru hash_function.c ***/
