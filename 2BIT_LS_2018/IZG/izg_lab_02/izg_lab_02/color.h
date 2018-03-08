/******************************************************************************
 * Laborator 01 - Zaklady pocitacove grafiky - IZG
 * ihulik@fit.vutbr.cz
 *
 * $Id:$

 * Popis:
 *    soubor obsahuje definice barevnych datovych typu + pomocne funkce pro
 *    praci s nimi 
 *
 * Opravy a modifikace:
 * -
 */

#ifndef Color_H
#define Color_H

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 * Includes
 */

#include "base.h"


/*****************************************************************************
 * Definice typu a fci pro reprezentaci RGBA barvy
 */

/* Struktura reprezentujici RGBA barvu (interne reprezentovana jako BGRA) */
typedef struct S_RGBA
{
    unsigned char   blue, green, red, alpha;
} S_RGBA;


/* Vytvori cernou barvu */
IZG_INLINE S_RGBA makeBlackColor()
{
    S_RGBA color;
    color.red = color.green = color.blue = 0;
    color.alpha = 255;
    return color;
}

/* Vytvori barvu o zadanych hodnotach */
IZG_INLINE S_RGBA makeColor(unsigned char r, unsigned char g, unsigned char b)
{
    S_RGBA color;
    color.red = r;
    color.green = g;
    color.blue = b;
    color.alpha = 255;
    return color;
}

/* Konstanty barev pouzivane v programu */
extern const S_RGBA     COLOR_BLACK;
extern const S_RGBA     COLOR_BLUE;
extern const S_RGBA     COLOR_RED;
extern const S_RGBA     COLOR_WHITE;
extern const S_RGBA     COLOR_GREEN;

#ifdef __cplusplus
}
#endif

#endif // Color_H

/*****************************************************************************/
/*****************************************************************************/
