/******************************************************************************
 * Laborator 01 - Zaklady pocitacove grafiky - IZG
 * ihulik@fit.vutbr.cz
 *
 * $Id:$
 *
 * Popis: Soubor obsahuje pomocne makra a funkce pro predmet IZG
 *
 * Opravy a modifikace:
 * -
 */

#ifndef Base_H
#define Base_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * Specialni nastaveni pro MS Visual C++
 * Vypne varovani o pouziti "unsafe" fci ze standardni C knihovny
 */

#ifndef _CRT_SECURE_NO_WARNINGS
    #if defined _WIN32 && defined _MSC_VER
        #define _CRT_SECURE_NO_WARNINGS
    #endif
#endif /* _CRT_SECURE_NO_WARNINGS */


/******************************************************************************
 * Includes
 */

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

/******************************************************************************
 * Definice inline fce v ruznych prostredich
 */

#ifndef IZG_INLINE
    #if defined __cplusplus
        #define IZG_INLINE inline
    #elif defined _WIN32 && !defined __GNUC__
        #define IZG_INLINE __inline
    #else
        #define IZG_INLINE static
    #endif
#endif /* IZG_INLINE */

/******************************************************************************
 * Uzitecna matematicka makra
 */

/* Prohodi hodnoty dvou promennych typu int */
#define SWAP(a, b)          { (b) ^= (a); (a) ^= (b); (b) ^= (a); }

/* Zaokrouhli hodnotu typu double na int */
#define ROUND(x)            ((int)((x) + 0.5f))

/* Zaokrouhli hodnotu typu double na unsigned char */
#define ROUND2BYTE(x)       ((unsigned char)((x) + 0.5f))

/* Absolutni hodnota */
#define ABS(x)              (((x) > 0) ? (x) : (-(x)))

/* Minimum ze dvou zadanych hodnot */
#define MIN(a, b)           (((a) < (b)) ? (a) : (b))

/* Maximum ze dvou zadanych hodnot */
#define MAX(a, b)           (((a) > (b)) ? (a) : (b))


/******************************************************************************
 * Jednoduche testovani a zpracovani runtime errors
 */

/* Kontrola zadane podminky, ktera muze byt ve finalnim kodu odstranena */
#define IZG_ASSERT(cond) assert(cond)

/* Zobrazi chybove hlaseni a ukonci program */
#define IZG_ERROR(msg) \
{ \
    fprintf(stderr, "<%s, %d> Error: %s", __FILE__, __LINE__, (msg)); \
    exit(-1); \
}

#define IZG_INFO(msg) \
{ \
    fprintf(stdout, "Info: %s", (msg)); \
}

/* Zobrazi chybove hlaseni vcetne popisu chyby v SDL knihovne a ukonci program */
#define IZG_SDL_ERROR(msg) \
{ \
    fprintf(stderr, "<%s, %d> Error: %s, SDL: %s", __FILE__, __LINE__, (msg), SDL_GetError()); \
    exit(-1); \
}

/* Otestuje podminku a generuje chybu pokud neplati */
#define IZG_CHECK(cond, msg) \
{ \
    if( !cond ) \
    { \
        IZG_ERROR(msg); \
    } \
}


/******************************************************************************
 * A jeste nejaka makra navic...
 */

/* Makro zkopiruje blok pameti o zadane velikosti (nahrada fce memcpy) */
#define MEMCOPY(pDst, pSrc, length) \
{ \
    char * _dst = (char *)(pDst); \
    const char * _src = (const char *)(pSrc); \
    size_t _length = (length); \
    size_t i = 0; \
    if( (_length & (sizeof(int) - 1)) == 0 ) { \
        for( ; i < _length; i += sizeof(int) ) \
            *((int *)(_dst + i)) = *((const int *)(_src + i)); \
    } else { \
        for( ; i < _length; ++i ) \
            *(_dst + i) = *(_src + i); \
    } \
}

#ifdef __cplusplus
}
#endif

#endif /* Base_H */

/*****************************************************************************/
/*****************************************************************************/
