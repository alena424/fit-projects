/**
 * @file        globals.h
 * @author      Ladislav Mosner, VUT FIT Brno, imosner@fit.vutbr.cz
 * @author      Petr Kleparnik, VUT FIT Brno, ikleparnik@fit.vutbr.cz
 * @author      Kamil Behun, VUT FIT Brno, ibehun@fit.vutbr.cz
 * @date        11.03.2018
 *
 * @brief       Globalni promenne.
 *
 */

#ifndef GLOBALS_H
#define GLOBALS_H

#include "SDL.h"

// Globalni promenne (definovane v main.c)

// Kreslici buffer knihovny SDL
extern SDL_Surface *screen;

// Kreslici buffer IZG cvičení
extern RGBA *framebuffer;

// Sirka a vyska framnebufferu
extern int width;
extern int height;

#endif /* GLOBALS_H */
