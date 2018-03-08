/******************************************************************************
 * Laborator 02 - Zaklady pocitacove grafiky - IZG
 * isolony@fit.vutbr.cz
 *
 * $Id: $
 * 
 * Popis: Hlavicky funkci pro funkce studentu
 *
 * Opravy a modifikace:
 * ivelas@fit.vutbr.cz
 * itomesek@fit.vutbr.cz
 */

#include "student.h"
#include "globals.h"

#include <time.h>

/******************************************************************************
 ******************************************************************************
 Funkce vraci pixel z pozice x, y. Je nutne hlidat frame_bufferu, pokud 
 je dana souradnice mimo hranice, funkce vraci barvu (0, 0, 0).*/
S_RGBA getPixel(int x, int y)
{
	if (x > 0 && y > 0 && x < width && y < height)
		return frame_buffer[y * width + x];
	else
		return makeBlackColor();
}

/******************************************************************************
 ******************************************************************************
 Funkce vlozi pixel na pozici x, y. Je nutne hlidat frame_bufferu, pokud 
 je dana souradnice mimo hranice, funkce neprovadi zadnou zmenu.*/
void putPixel(int x, int y, S_RGBA color)
{
	if (x > 0 && y > 0 && x < width && y < height)
		frame_buffer[y * width + x] = color;
}


#include <iostream>
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Ukol za 2b
////////////////////////////////////////////////////////////////////////////////
#define FRAC_BITS 8
void drawLine (int x1, int y1, int x2, int y2) {

	int dx = (x2-x1);
	int dy = (y2-y1);
	int vymena = 0;

	if (x1 == x2 && y1 == y2 ) {
		putPixel(x1, y1 >> FRAC_BITS, COLOR_GREEN);
		return;
	}


	if (abs(dy) > abs(dx)) {
		SWAP(x1, y1);
		SWAP(x2, y2);
		vymena = 1;
	}

	if (x1 > x2) {
		SWAP(x1, x2);
		SWAP(y1, y2);
	}

	dx = (x2 - x1);
	dy = (y2 - y1);

	int y = y1 << FRAC_BITS;
	int k = (dy << FRAC_BITS) / dx;
	for (int x = x1; x <= x2; x++)
	{
		if (vymena) {
			// vymenime zpet
			putPixel(y >> FRAC_BITS, x, COLOR_GREEN);
		}
		else {
			putPixel(x, y >> FRAC_BITS, COLOR_GREEN);
		}
      
		// Doplnte a upravte ...
                    
		
		y += k;
	}
}

////////////////////////////////////////////////////////////////////////////////
// Ukol za 1b
////////////////////////////////////////////////////////////////////////////////
void put8PixelsOfCircle(int x, int y, int s1, int s2, S_RGBA color) {
	

	putPixel(s1 + x, s2 + y, color);
	putPixel(s1 - x, s2 - y, color);
	putPixel(s1 + x, s2 - y, color);
	putPixel(s1 - x, s2 + y, color);

	putPixel(s1 - y, s2 - x, color);
	putPixel(s1 + y, s2 + x, color);
	putPixel(s1 - y, s2 + x, color);
	putPixel(s1 + y, s2 - x, color);

	// Doplnte ...
    
}

void drawCircle (int s1, int s2, int R)
{
	S_RGBA col;
	col.blue = 0;
	col.green = 0;
	col.red = 255;

	int x = 0, y = R;
	int P = 1-R, X2 = 3, Y2 = 2*R-2;
	while (x <= y) {
		put8PixelsOfCircle(x, y, s1, s2, col);
		if (P >= 0) {
			P += -Y2;
			Y2 -= 2;
			y--;
		}
		P += X2;
		X2 += 2;
		x++;
	}
	return;
}

/*****************************************************************************/
/*****************************************************************************/