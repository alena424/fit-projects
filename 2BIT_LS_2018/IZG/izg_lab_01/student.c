/******************************************************************************
 * Laborator 01 - Zaklady pocitacove grafiky - IZG
 * ihulik@fit.vutbr.cz
 *
 * $Id: $
 *
 * Popis: Hlavicky funkci pro funkce studentu
 *
 * Opravy a modifikace:
 * - ibobak@fit.vutbr.cz, orderedDithering
 */

#include "student.h"
#include "globals.h"

#include <time.h>

const int M[] = {
    0, 204, 51, 255,
    68, 136, 187, 119,
    34, 238, 17, 221,
    170, 102, 153, 85
};

const int M_SIDE = 4;

/******************************************************************************
 ******************************************************************************
 Funkce vraci pixel z pozice x, y. Je nutne hlidat frame_bufferu, pokud
 je dana souradnice mimo hranice, funkce vraci barvu (0, 0, 0).
 Ukol za 0.25 bodu */
S_RGBA getPixel(int x, int y)
{
    // kontrola mezi x a y
	if ( x < width && y < height &&
        x >= 0 && y >= 0) {
		// muzu pocitat, chci data z bufferu
			return frame_buffer[ (y* width)+ x];
	}

    return COLOR_BLACK; //vraci barvu (0, 0, 0)
}
/******************************************************************************
 ******************************************************************************
 Funkce vlozi pixel na pozici x, y. Je nutne hlidat frame_bufferu, pokud
 je dana souradnice mimo hranice, funkce neprovadi zadnou zmenu.
 Ukol za 0.25 bodu */
void putPixel(int x, int y, S_RGBA color)
{
    if ( x < width && y < height &&
        x >= 0 && y >= 0) {
            //kontrola
            frame_buffer[ (width * y)+x  ] = color;
        }

}
/******************************************************************************
 ******************************************************************************
 Funkce prevadi obrazek na odstiny sedi. Vyuziva funkce GetPixel a PutPixel.
 Ukol za 0.5 bodu */
void grayScale()
{
    // musime projit cely obrazek
    S_RGBA pixel;
    for ( int i = 0; i < width ; i++ ) {
        for ( int j = 0; j < height; j++ ) {
            pixel = getPixel( i,j );
            // 0.299R + 0.587G + 0.114B
            unsigned int intensity = ROUND( (0.299 * pixel.red )+ ( 0.587 * pixel.green ) + ( 0.114 * pixel.blue ) );
            pixel.red = intensity;
            pixel.blue = intensity;
            pixel.green = intensity;

            putPixel(i,j,pixel);
        }
    }
}

/******************************************************************************
 ******************************************************************************
 Funkce prevadi obrazek na cernobily pomoci algoritmu maticoveho rozptyleni.
 Ukol za 1 bod */

void orderedDithering()
{
    grayScale();
    for ( int i = 0; i < width; i++ ) {
        for ( int j = 0; j < height; j++ ) {
            S_RGBA pixel = getPixel( i,j );
            int intensity = pixel.blue;
            //porovnam ho s matici
            int matrix_r = i % M_SIDE;
            int matrix_c = j % M_SIDE;
            //porovname
            if ( intensity > M[ ( matrix_r * M_SIDE ) + matrix_c ] ) {
                pixel = COLOR_WHITE;
            } else {
                pixel = COLOR_BLACK;
            }
            putPixel(i,j,pixel);
        }
    }

}


void applyDistribution(int x, int y, int error, float magic){
    S_RGBA pixel = getPixel(x,y);
    int nextintensity = pixel.blue;
    //nextintensity = ROUND( nextintensity + ( magic * error ) );
    nextintensity = ROUND(magic*error ) + nextintensity;
    if ( nextintensity > 255){
        nextintensity = 255;
    }
    if ( nextintensity < 0 ) {
        nextintensity = 0;
    }
    pixel.blue = pixel.red = pixel.green = nextintensity;
    putPixel( x, y, pixel );
}
/******************************************************************************
 ******************************************************************************
 Funkce prevadi obrazek na cernobily pomoci algoritmu distribuce chyby.
 Ukol za 1 bod */
void errorDistribution()
{
    // todo

    unsigned int intensity = 0;

    int error;
    // prevedeme na cernobily obrazek
    grayScale();
    for ( int i = 0; i < width; i++ ) {
         for ( int j = 0; j < height; j++ ) {
            S_RGBA pixel = getPixel(i,j); // aktualni pixel
            intensity = pixel.red;
            if ( intensity <= 127 ) {
                // bude cerna

                putPixel(i,j, COLOR_BLACK);
                // pokud je 0 cerna, tak se chyba rovna intesite
                error = intensity;
            } else {
                // bude bila
                putPixel(i,j, COLOR_WHITE);
                // pokud je pixel bily, tak se chyba rovna intensite - 255
                error = intensity - 255;
            }


            applyDistribution(i+1, j, error, 3.0/8);
            applyDistribution(i, j+1, error, 3.0/8);
            applyDistribution(i+1, j+1, error, 1.0/4);

/*
            // ted zmenim ty 3 pixely
            for ( int x = 0; x <= 1; x++ ) {
                for ( int y = 0; y <= 1; y++  ) {
                    S_RGBA nextpixel = getPixel( x+i, y+j );
                    int nextintensity = nextpixel.blue;
                    if (x != y) {
                        // neni diagonala, pouzivam 3/8
                        nextintensity = ROUND( nextintensity + ( downright * error ) );
                    } else{
                        nextintensity = ROUND( nextintensity + ( diagonal * error ) );
                    }
                    if ( x != 0 && y != 0) {
                        if ( nextintensity > 255){
                            nextintensity = 255;
                        }
                        if ( nextintensity < 0 ) {
                            nextintensity = 0;
                        }
                        nextpixel.blue = nextpixel.red = nextpixel.green = nextintensity;
                        putPixel( x+i, y+j, nextpixel );
                    }
                }
            }
            */
         }
    }
}

/******************************************************************************
 ******************************************************************************
 Funkce prevadi obrazek na cernobily pomoci metody prahovani.
 Demonstracni funkce */
void thresholding(int Threshold)
{
	/* Prevedeme obrazek na grayscale */
	grayScale();

	/* Projdeme vsechny pixely obrazku */
	for (int y = 0; y < height; ++y)
		for (int x = 0; x < width; ++x)
		{
			/* Nacteme soucasnou barvu */
			S_RGBA color = getPixel(x, y);

			/* Porovname hodnotu cervene barevne slozky s prahem.
			   Muzeme vyuzit jakoukoli slozku (R, G, B), protoze
			   obrazek je sedotonovy, takze R=G=B */
			if (color.red > Threshold)
				putPixel(x, y, COLOR_WHITE);
			else
				putPixel(x, y, COLOR_BLACK);
		}
}

/******************************************************************************
 ******************************************************************************
 Funkce prevadi obrazek na cernobily pomoci nahodneho rozptyleni.
 Vyuziva funkce GetPixel, PutPixel a GrayScale.
 Demonstracni funkce. */
void randomDithering()
{
	/* Prevedeme obrazek na grayscale */
	grayScale();

	/* Inicializace generatoru pseudonahodnych cisel */
	srand((unsigned int)time(NULL));

	/* Projdeme vsechny pixely obrazku */
	for (int y = 0; y < height; ++y)
		for (int x = 0; x < width; ++x)
		{
			/* Nacteme soucasnou barvu */
			S_RGBA color = getPixel(x, y);

			/* Porovname hodnotu cervene barevne slozky s nahodnym prahem.
			   Muzeme vyuzit jakoukoli slozku (R, G, B), protoze
			   obrazek je sedotonovy, takze R=G=B */
			if (color.red > rand()%255)
			{
				putPixel(x, y, COLOR_WHITE);
			}
			else
				putPixel(x, y, COLOR_BLACK);
		}
}
/*****************************************************************************/
/*****************************************************************************/
