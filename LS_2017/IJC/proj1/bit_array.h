/**
* @file bit_array.h
* Reseni IJC-DU1, priklad a)
*
* @date 20.3.2017
* @author Alena Tesarova, FIT
* @gcc 5.4
*
* Popis:
* Modul implementujici potrebna makra a inline funkce
*
*
*/


#include "error.h"
#include <stdio.h>
#include <limits.h>

/*****************************************************
* START EVERYTHING                                   *
******************************************************/

//#ifndef BIT_ARRAY_H_MACRO
//#define BIT_ARRAY_H_MACRO

typedef unsigned long * bit_array_t;
/**
* @brief velikost unsigned long v bitech
* CHAR_BIT - pocet bitu v charu
* je to asi 8 * 8 = 64
*/
#define SIZE_LONG_BIT \
    ( CHAR_BIT * sizeof( unsigned long ) )

/**
* @brief pocet unsigned longu - potrebny pocet bajtu v poli
* @param size pocet hodnot, ktere chci ulozit ( N = 303 milionu v zadani )
* navic si potrebuju ulozit N, dam pocet na zacatek, rezervuju si 4 bajty = 1 unsigned long
*/
#define SIZE_ARRAY( size ) \
    ( size / SIZE_LONG_BIT + 1 )


/**
* @brief pocet prvocisel ulozenych na nultem indexu v poli
*/
#define ARRAY_PRIMES( array ) \
    array[0]

/**
* @brief vrati jaky bit podle indexu
* @param index - podle indexu vrati kolikaty bit
*
*
*/
#define OUT_OF_ARRAY( index ) \
    ( index / SIZE_LONG_BIT )


/**
* @brief vrati na jakem jsme unsigned longu podle indexu
* @param index - podle indexu vrati jaky long
*/
#define WHICH_LONG( index ) \
    ( ( index / SIZE_LONG_BIT ) +1 )

/**
* @brief inicializuji pole, prvni polozka je velikost pole, zbytek budou nuly
* @param array - pole
* @param size - pocet prvocisel, ktere budu ukladat bitove
*/
#define ba_create( array, size) \
    unsigned long array[ SIZE_ARRAY( size ) ]  = { size , 0 }


/**
* @brief if we run out of array
*/
#define WARNING( index, array ) \
     error_msg("Index %lu mimo rozsah 0..%lu\n", (long)index, (long)ARRAY_PRIMES( array ) )

/*****************************************************
* MACROS                                             *
******************************************************/
#ifndef USE_INLINE

/**
* @brief vypocita pocet bitu v poli
*/
#define ba_size( array, size )  \
    ( sizeof( array ) * SIZE_ARRAY( size ) )

/**
* @brief ziska hodnotu zadaneho indexu
*/
#define ba_get_bit( array, index ) \
     ( index > ARRAY_PRIMES( array ) ) \
     ? WARNING( index, array ), 0 : ( array[ WHICH_LONG( index ) ] & ( 1LU << ( index % SIZE_LONG_BIT ) ) )

/**
* @brief nastavi index na value
* @param array - nase pole
* @param index - nastavovany index
* @param value - odpovida 1/0
*/

#define ba_set_bit( array, index, value ) \
    do { if (index>ARRAY_PRIMES(array)) { \
            WARNING( index, array ); }\
            ( value ) ? ( array[ WHICH_LONG( index ) ] |= ( 1LU << (index % SIZE_LONG_BIT) ) ) \
            :  (array[ WHICH_LONG( index ) ] &= ~( 1LU << ( index % SIZE_LONG_BIT ) ) ); \
    } while(0)


#endif // USE_INLINE

/*****************************************************
* END MACROS                                         *
******************************************************/

/*****************************************************
* INLINE FUNCTION                                    *
******************************************************/

#ifdef USE_INLINE


/**
* Implementace s vyuzitim maker SIZE_LONG_BIT, ARRAY_PRIMES, WHICH_LONG, SIZE_ARRAY
*/
static inline unsigned long ba_get_bit( bit_array_t array, unsigned long index)
{
    if ( index > ARRAY_PRIMES( array ) )
    {
        WARNING( index, array );
        exit(1);
    }

	return (array[ WHICH_LONG( index ) ] & ( 1LU << ( index % SIZE_LONG_BIT ) ) );
}

static inline void ba_set_bit( bit_array_t array, unsigned index, unsigned long value)
{
    if ( index > ARRAY_PRIMES(array) )
    {
        WARNING( index, array );
		exit(1);
    }
    else
    {
        if (value)
        {
             array[ WHICH_LONG( index ) ] |= ( 1LU << (index % SIZE_LONG_BIT) );
        }
        else
        {
            array[ WHICH_LONG( index ) ] &= ~( 1LU << ( index % SIZE_LONG_BIT ) );
        }
    }
}

static inline unsigned long ba_size( bit_array_t array, unsigned long size )
{
    return ( sizeof( array ) * SIZE_ARRAY( size ) );
}

#endif // USE_INLINE

/*****************************************************
* END INLINE FUNCTION                                *
******************************************************/
/*****************************************************
* END EVERYTHING                                     *
******************************************************/
