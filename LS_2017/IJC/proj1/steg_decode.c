/**
* @file steg_decode.c
* Reseni IJC-DU1, priklad b)
*
* @date 25.3.2017
* @author Alena Tesarova, FIT
* @gcc 5.4
*
* Popis:
* Hlavni funkce pro zpracovani a pocitani zpravy z obrazku
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "bit_array.h"
#include "eratosthenes.h"
#include "ppm.h"
#include "error.h"

int main( int argc, char *argv[] )
{
    if ( argc != 2 )
    {
        error_msg( "Spatny pocet zadanych argumentu\n" );
    }
    struct ppm *p = NULL;

    if ((p = ppm_read(argv[1] ) ) == NULL)
    {
        error_msg("Nepodario se nacist obrazek\n");
    }

    unsigned int rozmery = p->xsize * p->ysize * 3;
    unsigned char zprava = 0;
    int j = 0;
    char lsb;
    //bit_array_t array = NULL;

    ba_create(array, VELIKOST);
    Eratosthenes(array);

    //projdeme polem do rozemry
    for(unsigned long i = 2; i <= rozmery; i++)
    {
        //printf("na indexu %lu je %d \n", i, p->data[i] );
        if ( ba_get_bit(array, i) == 0 )
        {
            //z p->data[i] vezmu LSB
            lsb = p->data[i] & 1;
            if (lsb)
            {
                zprava |= 1 << j;
            }
            j++;

            if (j == 8 )
            {
                //printf("%d ", zprava);
                if ( zprava == '\0')
                {
                    break;
                }
                printf( "%c", zprava);
                j = 0;
                zprava = 0;
            }
        }
    }
    return 0;
}
