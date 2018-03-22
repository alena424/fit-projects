/**
 * @file        student.cpp
 * @author      Ladislav Mosner, VUT FIT Brno, imosner@fit.vutbr.cz
 * @author      Petr Kleparnik, VUT FIT Brno, ikleparnik@fit.vutbr.cz
 * @author      Kamil Behun, VUT FIT Brno, ibehun@fit.vutbr.cz
 * @date        11.03.2018
 *
 * @brief       Deklarace funkci studentu. DOPLNUJI STUDENTI
 *
 */

#include "base.h"
#include "student.h"
#include "globals.h"
#include <math.h>

/**
 * @brief Vraci barvu pixelu z pozice [x, y]
 * @param[in] x X souradnice pixelu
 * @param[in] y Y souradnice pixelu
 * @return Barva pixelu na pozici [x, y] ve formatu RGBA
 */
RGBA getPixel(int x, int y)
{
    if (x >= width || y >= height || x < 0 || y < 0) {
        IZG_ERROR("Pristup do framebufferu mimo hranice okna\n");
    }
    return framebuffer[y * width + x];
}

/**
 * @brief Nastavi barvu pixelu na pozici [x, y]
 * @param[in] x X souradnice pixelu
 * @param[in] y Y souradnice pixelu
 * @param[in] color Barva pixelu ve formatu RGBA
 */
void putPixel(int x, int y, RGBA color)
{
    if (x >= width || y >= height || x < 0 || y < 0) {
        IZG_ERROR("Pristup do framebufferu mimo hranice okna\n");
    }
    framebuffer[y * width + x] = color;
}

/**
 * @brief Vykresli usecku se souradnicemi [x1, y1] a [x2, y2]
 * @param[in] x1 X souradnice 1. bodu usecky
 * @param[in] y1 Y souradnice 1. bodu usecky
 * @param[in] x2 X souradnice 2. bodu usecky
 * @param[in] y2 Y souradnice 2. bodu usecky
 * @param[in] color Barva pixelu usecky ve formatu RGBA
 * @param[in] arrow Priznak pro vykresleni sipky (orientace hrany)
 */
void drawLine(int x1, int y1, int x2, int y2, RGBA color, bool arrow = false)
{

    if (arrow) {
        // Sipka na konci hrany
        double vx1 = x2 - x1;
        double vy1 = y2 - y1;
        double length = sqrt(vx1 * vx1 + vy1 * vy1);
        double vx1N = vx1 / length;
        double vy1N = vy1 / length;
        double vx1NN = -vy1N;
        double vy1NN = vx1N;
        int w = 3;
        int h = 10;
        int xT = (int) (x2 + w * vx1NN - h * vx1N);
        int yT = (int) (y2 + w * vy1NN - h * vy1N);
        int xB = (int) (x2 - w * vx1NN - h * vx1N);
        int yB = (int) (y2 - w * vy1NN - h * vy1N);
        pinedaTriangle(Point(x2, y2), Point(xT, yT), Point(xB, yB), color, color, false);
    }

    bool steep = abs(y2 - y1) > abs(x2 - x1);

    if (steep) {
        SWAP(x1, y1);
        SWAP(x2, y2);
    }

    if (x1 > x2) {
        SWAP(x1, x2);
        SWAP(y1, y2);
    }

    const int dx = x2 - x1, dy = abs(y2 - y1);
    const int P1 = 2 * dy, P2 = P1 - 2 * dx;
    int P = 2 * dy - dx;
    int y = y1;
    int ystep = 1;
    if (y1 > y2) ystep = -1;

    for (int x = x1; x <= x2; x++) {
        if (steep) {
            if (y >= 0 && y < width && x >= 0 && x < height) {
                putPixel(y, x, color);
            }
        } else {
            if (x >= 0 && x < width && y >= 0 && y < height) {
                putPixel(x, y, color);
            }
        }

        if (P >= 0) {
            P += P2;
            y += ystep;
        } else {
            P += P1;
        }
    }
}

/**
 * @brief Vyplni a vykresli trojuhelnik
 * @param[in] v1 Prvni bod trojuhelniku
 * @param[in] v2 Druhy bod trojuhelniku
 * @param[in] v3 Treti bod trojuhelniku
 * @param[in] color1 Barva vyplne trojuhelniku
 * @param[in] color2 Barva hranice trojuhelniku
 * @param[in] arrow Priznak pro vykresleni sipky (orientace hrany)
 *
 * SPOLECNY UKOL. Doplnuji studenti se cvicicim.
 */
void pinedaTriangle(const Point &v1, const Point &v2, const Point &v3, const RGBA &color1, const RGBA &color2, bool arrow)
{
    // Nalezeni obalky (minX, maxX), (minY, maxY) trojuhleniku.
    int minX = MIN( v1.x, MIN(v2.x, v3.x) );
    int minY = MIN( v1.y, MIN(v2.y, v3.y) );
    int maxX = MAX( v1.x, MAX(v2.x, v3.x) );
    int maxY = MAX( v1.y, MAX(v2.y, v3.y) );

    //////// DOPLNTE KOD /////////


    // Oriznuti obalky (minX, maxX, minY, maxY) trojuhleniku podle rozmeru okna.
    if ( minX < 0 ){
        minX = 0;
    }
    if ( minY < 0 ) {
        minY = 0;
    }
    if ( maxX >= width ) {
        maxX = width -1;
    }
    if ( maxY >= height ) {
        maxY = height - 1;
    }

    //////// DOPLNTE KOD /////////


    // Spocitani parametru hranove funkce (deltaX, deltaY) pro kazdou hranu.
	// Hodnoty deltaX, deltaY jsou souradnicemi vektoru, ktery ma pocatek
	// v prvnim vrcholu hrany, konec v druhem vrcholu.
	// Vypocet prvnotni hodnoty hranove funkce.
	// hranova funkce
	// (y - y0)(x2 - x1) - (x - x0)(y2 - y1)
	// xo - nas bod
	// x - min.x

    //////// DOPLNTE KOD /////////
    int deltax1 = v2.x - v1.x;
    int deltax2 = v3.x - v2.x;
    int deltax3 = v1.x - v3.x;

    int deltay1 = v2.y - v1.y;
    int deltay2 = v3.y - v2.y;
    int deltay3 = v1.y - v3.y;

    int hran1 = deltax1*(minY - v1.y) - deltay1*(minX - v1.x);
    int hran2 = deltax2*(minY - v2.y) - deltay2*(minX - v2.x);
    int hran3 = deltax3*(minY - v3.y) - deltay3*(minX - v3.x);



    // Vyplnovani: Cyklus pres vsechny body (x, y) v obdelniku (minX, minY), (maxX, maxY).
    // Pro aktualizaci hodnot hranove funkce v bode P (x +/- 1, y) nebo P (x, y +/- 1)
    // vyuzijte hodnoty hranove funkce E (x, y) z bodu P (x, y).

    //////// DOPLNTE KOD /////////
    for( int y = minY; y < maxY; y++ ) {
        //zda sudy nebo lichy radek
        bool even = (y - minY) % 2 == 0;
        for ( int x = even ? minX : maxX ; even ? x <= maxX : x >= minX; even ? x++ : x-- ) {
            // kdyz jsem v plusu, davam pixel
            if ( hran1 >= 0 && hran2 >= 0 && hran3 >= 0 ) {
                putPixel(x,y, color1);
            }
            if ( !( ( even && x == maxX ) || (! even && x == minX ) ) ) {
                hran1 += even ? -deltay1 : deltay1;
                hran2 += even ? -deltay2 : deltay2;
                hran3 += even ? -deltay3 : deltay3;
            }
        }
        hran1 += deltax1;
        hran2 += deltax2;
        hran3 += deltax3;
    }

    // Prekresleni hranic trojuhelniku barvou color2.
    drawLine(v1.x, v1.y, v2.x, v2.y, color2, arrow);
    drawLine(v2.x, v2.y, v3.x, v3.y, color2, arrow);
    drawLine(v3.x, v3.y, v1.x, v1.y, color2, arrow);
}

/**
 * @brief Vyplni a vykresli polygon
 * @param[in] points Pole bodu polygonu
 * @param[in] size Pocet bodu polygonu (velikost pole "points")
 * @param[in] color1 Barva vyplne polygonu
 * @param[in] color2 Barva hranice polygonu
 *
 * SAMOSTATNY BODOVANY UKOL. Doplnuji pouze studenti.
 */
void pinedaPolygon(const Point *points, const int size, const RGBA &color1, const RGBA &color2)
{
    // Pri praci muzete vyuzit pro vas predpripravene datove typy z base.h., napriklad:
    //
    //      Pro ukladani parametru hranovych funkci muzete vyuzit prichystany vektor parametru hranovych funkci "EdgeParams":
    //
    //          EdgeParams edgeParams(size)                         // Vytvorite vektor (pole) "edgeParams" parametru hranovych funkci o velikosti "size".
    //          edgeParams[i].deltaX, edgeParams[i].deltaY          // Pristup k parametrum (deltaX, deltaY) hranove funkce v poli "edgeParams" na indexu "i".
    //
    //      Pro ukladani hodnot hranovych funkci muzete vyuzit prichystany vektor hodnot hranovych funkci "EdgeFncValues":
    //
    //          EdgeFncValues edgeFncValues(size)                   // Vytvorite vektor (pole) "edgeFncValues" hodnot hranovych funkci o velikosti "size".
    //          edgeFncValues[i]                                    // Pristup k hodnote hranove funkce v poli "edgeFncValues" na indexu "i".
    //

    EdgeParams edgeParam(size);
    //EdgeParam[i].deltaX

    // Nalezeni obalky (minX, maxX), (minY, maxY) polygonu.
    int minX  = points[0].x;
    int maxX = points[0].x;
    int minY = points[0].y;
    int maxY = points[0].y;
    for(  int i = 0; i < size; i ++) {
        minX =  points[i].x < minX ? points[i].x : minX;
        maxX = points[i].x > maxX ? points[i].x : maxX;
        minY = points[i].y < minY ? points[i].y : minY;
        maxY = points[i].y > maxY ? points[i].y : maxY;
    }

    //////// DOPLNTE KOD /////////


    // Oriznuti obalky (minX, maxX), (minY, maxY) polygonu podle rozmeru okna

    //////// DOPLNTE KOD /////////
    if ( minX < 0 ){ minX = 0; }
    if ( minY < 0 ){ minY = 0; }

    if ( maxX >= width ){ maxX = width -1; }
    if ( maxY >= height ){ maxY = height -1; }


    // Spocitani parametru (deltaX, deltaY) hranove funkce pro kazdou hranu.
	// Hodnoty deltaX, deltaY jsou souradnicemi vektoru, ktery ma pocatek
	// v prvnim vrcholu hrany, konec v druhem vrcholu.
	// Vypocet prvnotnich hodnot hranovych funkci pro jednotlive hrany.

		// (y - y0)(x2 - x1) - (x - x0)(y2 - y1)
	// xo - nas bod
	// x - min.x

    //////// DOPLNTE KOD /////////
    /*int deltax1 = v2.x - v1.x;
    int deltax2 = v3.x - v2.x;
    int deltax3 = v1.x - v3.x;

    int deltay1 = v2.y - v1.y;
    int deltay2 = v3.y - v2.y;
    int deltay3 = v1.y - v3.y;

    int hran1 = deltax1*(minY - v1.y) - deltay1*(minX - v1.x);
    int hran2 = deltax2*(minY - v2.y) - deltay2*(minX - v2.x);
    int hran3 = deltax3*(minY - v3.y) - deltay3*(minX - v3.x);*/

    //////// DOPLNTE KOD /////////
    EdgeFncValues edgeFncValues(size);

    for ( int i = 0; i < size; i++ ) {
        EdgeParam tmp( points[(i+1)%size].x - points[i].x, points[(i+1)%size].y - points[i].y);
        edgeParam[i] = tmp;

        edgeFncValues[i] = edgeParam[i].deltaX*(minY - points[i].y) - edgeParam[i].deltaY*(minX - points[i].x);

    }


    // Test konvexnosti polygonu

    //////// DOPLNTE KOD /////////


    // Vyplnovani: Cyklus pres vsechny body (x, y) v obdelniku (minX, minY), (maxX, maxY).
    // Pro aktualizaci hodnot hranove funkce v bode P (x +/- 1, y) nebo P (x, y +/- 1)
    // vyuzijte hodnoty hranove funkce E (x, y) z bodu P (x, y) */

    //////// DOPLNTE KOD /////////
    /*  for( int y = minY; y < maxY; y++ ) {
        //zda sudy nebo lichy radek
        bool even = (y - minY) % 2 == 0;
        for ( int x = even ? minX : maxX ; even ? x <= maxX : x >= minX; even ? x++ : x-- ) {
            // kdyz jsem v plusu, davam pixel
            if ( hran1 >= 0 && hran2 >= 0 && hran3 >= 0 ) {
                putPixel(x,y, color1);
            }
            if ( !( ( even && x == maxX ) || (! even && x == minX ) ) ) {
                hran1 += even ? -deltay1 : deltay1;
                hran2 += even ? -deltay2 : deltay2;
                hran3 += even ? -deltay3 : deltay3;
            }
        }
        hran1 += deltax1;
        hran2 += deltax2;
        hran3 += deltax3;
    }*/
    for ( int y = minY; y < maxY; y++ ) {
        //sudy nebo lichy
        bool even = (y - minY) % 2 == 0;
        for ( int x = even ? minX : maxX ; even ? x <= maxX : x >= minX; even ? x++ : x-- ) {
            // projdu vsechny heradlove funkce
            bool print = true;
            for ( int i = 0; i < size; i++ ) {
                if ( edgeFncValues[i] <= 0 ){
                    print = false;
                }
            }
            if ( print ) {
                putPixel( x, y, color1 );
            }
            // kdyz nejsme na hranice, posunujeme se dale
            if ( !( ( even && x == maxX ) || (! even && x == minX ) ) ) {
                // vsechny hradlovky zvetsim
                for ( int i = 0; i < size; i++ ) {
                    edgeFncValues[i] += even ? -edgeParam[i].deltaY : edgeParam[i].deltaY ;
                }


            }
        }
        // posun o radek dolu
        for ( int i = 0; i < size; i++ ) {
            edgeFncValues[i] += edgeParam[i].deltaX ;
        }

    }


    // Prekresleni hranic polygonu barvou color2.
    for (int i = 0; i < size; i++) {
        drawLine(points[i].x, points[i].y, points[(i + 1) % size].x, points[(i + 1) % size].y, color2/*, true*/);
    }
}
