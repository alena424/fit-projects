  /**
 * @file error.h
 * Reseni IJC-DU1, priklad b)
 *
 * @date 20.3.2017
 * @author Alena Tesarova, FIT
 * @gcc 5.4
 *
 * Popis:
 * Hlavickovy soubor pro error.c
 *
*/


#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include "error.h"

// error_msg("Index %lu mimo rozsah 0..%lu",
 //              (unsigned long)index, (unsigned long)mez).
void warning_msg(const char *fmt, ...){
    va_list ap;
    va_start( ap, fmt );
    fprintf(stderr,"CHYBA: ");
    vfprintf(stderr, fmt, ap );
    va_end( ap );

}

void error_msg(const char *fmt, ...){

    va_list ap;
    va_start( ap, fmt );
    fprintf(stderr,"CHYBA: ");
    vfprintf(stderr, fmt, ap );
    va_end( ap );
    exit(1);
}
