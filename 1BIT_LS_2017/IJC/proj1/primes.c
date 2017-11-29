  /**
 * @file primes.c
 * Reseni IJC-DU1, priklad a)
 *
 * @date 20.3.2017
 * @author Alena Tesarova, FIT
 * @gcc 5.4
 *
 * Popis:
 * Hlavni main modul
 *
 *
 ############### POPIS ########################################

   Podmíněným překladem zajistěte, aby se při definovaném symbolu
   USE_INLINE místo těchto maker definovaly inline funkce stejného jména
   všude kde je to možné (bez změn v následujícím testovacím příkladu!).
   USE_INLINE nesmí být definováno ve zdrojovém textu --
   překládá se s argumentem -D (gcc -DUSE_INLINE ...).

   Napište pomocná makra BA_GET_BIT_(p,i), BA_SET_BIT_(p,i,b)
   pro indexování bitů v poli T *p nebo T p[NN] bez kontroly mezí,
   kde T je libovolný celočíselný typ (char, unsigned short, ...).
   (Tato makra pak použijete v dalších makrech a inline funkcích.)

   Budete pravděpodobně potřebovat zvětšit limit velikosti zásobníku.
   Na Unix-like systémech použijte příkaz ulimit -a pro zjištění velikosti
   limitu a potom "ulimit -s zadana_velikost_v_KiB".

   Každé prvočíslo tiskněte na zvláštní řádek v pořadí
   vzestupném.  Netiskněte  nic  jiného  než  prvočísla (bude se
   automaticky  kontrolovat!).  Pro kontrolu správnosti prvočísel
   můžete použít program  factor (./primes|factor).

   Zdrojový text programu se musí jmenovat "primes.c" !
   Napište Makefile tak, aby příkaz "make" vytvořil všechny varianty:
    primes      používá makra
    primes-i            inline funkce
   a aby příkaz "make run" všechny varianty spustil stylem:
     time ./primes
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h> //sqrt

#include "bit_array.h"
#include "eratosthenes.h"


#define PRIMES 303000000 // pocet prvocisel
#define LIMIT 10


void printPrimes( bit_array_t array ){
    unsigned int index = PRIMES - 1;
    unsigned int poc = 0;
    int i = 0;
    unsigned int pom_array[ LIMIT];
    while (poc != 10 && index != 0){
            //printf("pracujeme s indexem %d a hodnotou %lu\n", index, ba_get_bit(array, index) );
            if ( ( ba_get_bit( array, index ) ) == 0){
                  pom_array[poc] = index;
                   i++;

                 poc++;
            }
            index--;

    }
    for (int j = LIMIT-1; j >= 0; j--){
        printf("%d\n", pom_array[j] );
    }
}


int main(){
    ba_create( array, PRIMES );
    /*
    printf("Pocet prvocisel je: %d a size_long_bit je %lu\n", PRIMES, SIZE_LONG_BIT);
    printf("velikost pole je: %lu unsigned longu a pocet bajtu je: %lu\n", SIZE_ARRAY( PRIMES ), sizeof ( array ) );
    printf("na indexu cislo 11 je: %lu\n", ba_get_bit( array, 11) );

    printf("pocet prvocisel je: %lu \n", ARRAY_PRIMES(array) );*/

    Eratosthenes(array);
    printPrimes( array );


    return 0;
}
