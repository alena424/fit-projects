/******************************************************************************
 * Laborator 02 - Zaklady pocitacove grafiky - IZG
 * isolony@fit.vutbr.cz
 *
 * $Id:$
 *
 * Popis: Hlavicky funkci pro funkce studentu
 *
 * Opravy a modifikace:
 * - 
 */

#ifndef Student_H
#define Student_H

/*****************************************************************************
 * Includes
 */
#include "color.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 * Ukoly pro cviceni
 */
void drawLine (int x1, int y1, int x2, int y2);

void drawCircle (int s1, int s2, int R);


/*****************************************************************************
 * Ukazky
 */
/* Funkce vlozi pixel na pozici x, y. Je nutne hlidat frame_bufferu, pokud 
   je dana souradnice mimo hranice, funkce neprovadi zadnou zmenu.*/
void putPixel(int x, int y, S_RGBA color);

/* Funkce vraci pixel z pozice x, y. Je nutne hlidat frame_bufferu, pokud 
   je dana souradnice mimo hranice, funkce vraci barvu (0, 0, 0).*/
S_RGBA getPixel(int x, int y);

#ifdef __cplusplus
}
#endif

#endif /* Student_H */

/*****************************************************************************/
/*****************************************************************************/