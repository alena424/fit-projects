 /**
 * @file eratosthenes.c
 * Resení IJC-DU1, priklad a)
 *
 * @date 20.3.2017
 * @author Autor: Alena Tesarova, FIT
 * @gcc 4.9
 *
 * @brief obsahuje funkci Eratosthenes, ktera nam na indexy prvocisel da nulu
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "bit_array.h"
#include "eratosthenes.h"

void Eratosthenes( bit_array_t array ) {


    unsigned primes = sqrt( ARRAY_PRIMES(array) ) + 1;

    for(unsigned long i = 2; i < primes; i++ ) {
        if(ba_get_bit(array, i) == 0) {
            for( unsigned long index = i*i; index <= ARRAY_PRIMES(array); index += i ) {
                //nastavime nasobky na 1

                ba_set_bit( array, index, 1 );
            }
        }
    }
}
