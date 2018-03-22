/**
 * @file        student.h
 * @author      Ladislav Mosner, VUT FIT Brno, imosner@fit.vutbr.cz
 * @author      Petr Kleparnik, VUT FIT Brno, ikleparnik@fit.vutbr.cz
 * @author      Kamil Behun, VUT FIT Brno, ibehun@fit.vutbr.cz
 * @date        11.03.2018
 *
 * @brief       Definice funkci studentu.
 *
 */

#ifndef STUDENT_H
#define STUDENT_H

#ifdef __linux__
    #include <limits.h>
#endif

#include <vector>
#include <time.h>
#include <stdio.h>

#include "color.h"

void putPixel(int x, int y, RGBA color);

RGBA getPixel(int x, int y);

void pinedaTriangle(const Point &v1, const Point &v2, const Point &v3, const RGBA &color1, const RGBA &color2, bool arrow = false);

void pinedaPolygon(const Point *points, const int size, const RGBA &color1, const RGBA &color2);

#endif // STUDENT_H
