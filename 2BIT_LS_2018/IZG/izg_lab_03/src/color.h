/**
 * @file        color.h
 * @author      Ladislav Mosner, VUT FIT Brno, imosner@fit.vutbr.cz
 * @author      Petr Kleparnik, VUT FIT Brno, ikleparnik@fit.vutbr.cz
 * @author      Kamil Behun, VUT FIT Brno, ibehun@fit.vutbr.cz
 * @date        11.03.2018
 *
 * @brief       Definice datovych typu pro barvu pixelu a pomocne funkce pro praci s nimi.
 *
 */

#ifndef COLOR_H
#define COLOR_H

#include "base.h"

// Definice typu a fci pro reprezentaci RGBA barvy

// Struktura reprezentujici RGBA barvu (interne reprezentovana jako BGRA)
typedef struct RGBA
{
    unsigned char blue, green, red, alpha;

    static bool compare(const RGBA &c1, const RGBA &c2)
    {
        return (
            c1.red == c2.red && c1.green == c2.green && c1.blue == c2.blue && c1.alpha == c2.alpha
        );
    }

    static RGBA interpolate(const RGBA &c1, const RGBA &c2, float factor)
    {
        RGBA c;
        c.red   = (char) ((1 - factor) * c1.red   + factor * c2.red);
        c.green = (char) ((1 - factor) * c1.green + factor * c2.green);
        c.blue  = (char) ((1 - factor) * c1.blue  + factor * c2.blue);
        return c;
    }
} RGBA;

IZG_INLINE bool operator==(const RGBA &c1, const RGBA &c2)
{
    return RGBA::compare(c1, c2);
}

IZG_INLINE bool operator!=(const RGBA &c1, const RGBA &c2)
{
    return !RGBA::compare(c1, c2);
}

// Vytvori cernou barvu
IZG_INLINE RGBA makeBlackColor()
{
    RGBA color;
    color.red = color.green = color.blue = 0;
    color.alpha = 255;
    return color;
}

// Vytvori barvu o zadanych hodnotach
IZG_INLINE RGBA makeColor(unsigned char r, unsigned char g, unsigned char b)
{
    RGBA color;
    color.red = r;
    color.green = g;
    color.blue = b;
    color.alpha = 255;
    return color;
}

// Konstanty barev pouzivane v programu
extern const RGBA COLOR_BLACK;
extern const RGBA COLOR_GREEN;
extern const RGBA COLOR_BLUE;
extern const RGBA COLOR_RED;
extern const RGBA COLOR_WHITE;
extern const RGBA COLOR_TURQUOISE;
extern const RGBA COLOR_LIGHTSKYBLUE;
extern const RGBA COLOR_SALMON;
extern const RGBA COLOR_GOLD;
extern const RGBA COLOR_FIREBRICK;
extern const RGBA COLOR_ROYALBLUE;

#endif // COLOR_H
