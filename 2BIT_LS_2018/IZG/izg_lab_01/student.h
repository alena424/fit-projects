/******************************************************************************
 * Laborator 01 - Zaklady pocitacove grafiky - IZG
 * ihulik@fit.vutbr.cz
 *
 * $Id:$
 *
 * Popis: Hlavicky funkci pro funkce studentu
 *
 * Opravy a modifikace:
 * - ibobak@fit.vutbr.cz, orderedDithering
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

/* Funkce vlozi pixel na pozici x, y. Je nutne hlidat frame_bufferu, pokud 
   je dana souradnice mimo hranice, funkce neprovadi zadnou zmenu.
   Ukol za 0.25 bodu */
void putPixel(int x, int y, S_RGBA color);

/* Funkce vraci pixel z pozice x, y. Je nutne hlidat frame_bufferu, pokud 
   je dana souradnice mimo hranice, funkce vraci barvu (0, 0, 0).
   Ukol za 0.25 bodu */
S_RGBA getPixel(int x, int y);

/* Funkce prevadi obrazek na odstiny sedi. Vyuziva funkce GetPixel a PutPixel.
   Ukol za 0.5 bodu */   
void grayScale();

/* Funkce prevadi obrazek na cernobily pomoci algoritmu maticoveho rozptyleni.
   Ukol za 1 bod */ 
void orderedDithering();

/* Funkce prevadi obrazek na cernobily pomoci algoritmu distribuce chyby.
   Ukol za 1 bod */
void errorDistribution();

/*****************************************************************************
 * Ukazky
 */

/* Funkce prevadi obrazek na cernobily pomoci metody prahovani.
   Demonstracni funkce. */
void thresholding(int Threshold);

/* Funkce prevadi obrazek na cernobily pomoci nahodneho rozptyleni. 
   Vyuziva funkce GetPixel, PutPixel a GrayScale.
   Demonstracni funkce. */
void randomDithering();

#ifdef __cplusplus
}
#endif

#endif /* Student_H */

/*****************************************************************************/
/*****************************************************************************/