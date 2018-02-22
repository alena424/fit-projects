/******************************************************************************
 * Projekt - Zaklady pocitacove grafiky - IZG Lab 01
 * ihulik@fit.vutbr.cz
 *
 * $Id: $
 *
 * *) Ovladani programu:
 *      "Leva mys"		- Vypis hodnoty pixelu
 *      "Esc"			- ukonceni programu
 *      "L"				- Nahrani zkusebniho obrazku image.bmp
 *      "S"				- Ulozeni obrazovky do out.bmp
 *
 * Opravy a modifikace:
 * - ibobak@fit.vutbr.cz, orderedDithering
 */

/******************************************************************************
******************************************************************************
 * Includes
 */

#ifdef _WIN32
    #include <windows.h>
#endif

/* nase veci... */
#include "color.h"
#include "student.h"
#include "io.h"
#include "globals.h"

/* knihovna SDL + system */
#include <math.h>
#include <stdlib.h>
#include <stdio.h>


/******************************************************************************
 ******************************************************************************
 * Globalni konstanty a promenne
 */

/* titulek hlavniho okna */
const char     * PROGRAM_TITLE  = "IZG/2018 Lab 01";

/* defaultni velikost okna */
const int          DEFAULT_WIDTH    = 800;
const int          DEFAULT_HEIGHT   = 600;

/* kreslici buffer knihovny SDL */
SDL_Surface         * screen        = NULL;

/* kreslici buffer IZG cvièení */
S_RGBA				* frame_buffer	= NULL;

/* pomocna promenna pro ukonceni aplikace */
int                 quit            = 0;
int					width			= 800;
int					height			= 600;

/* Nazvy i/o souboru */
char *				input_image		= "data/image.bmp";
char *				output_image	= "data/out.bmp";



/******************************************************************************
 ******************************************************************************
 * funkce zajistujici prekresleni obsahu okna programu
 */

void onDraw(void)
{
    /* Test existence frame bufferu a obrazove pameti */
	IZG_ASSERT(frame_buffer && screen);

    /* Test typu pixelu */
    IZG_ASSERT(screen->format->BytesPerPixel == 4);

    /* Kopie bufferu do obrazove pameti */
    SDL_LockSurface(screen);

	/* Test, pokud kopirujeme rozdilnou velikost frame_bufferu a rozdilne pameti, musime pamet prealokovat */
	if (width != screen->w || height != screen->h)
	{
		SDL_SetVideoMode(width, height, 32, SDL_SWSURFACE);
		/*SDL_FreeSurface(screen);
		if (!(screen = SDL_SetVideoMode(width, height, 32, SDL_SWSURFACE|SDL_ANYFORMAT)))
		{
			IZG_ERROR("Cannot realocate screen buffer");
			SDL_Quit();
		}*/
	}
	MEMCOPY(screen->pixels, frame_buffer, sizeof(S_RGBA) * width * height);
    SDL_UnlockSurface(screen);
	
    /* Vymena zobrazovaneho a zapisovaneho bufferu */
    SDL_Flip(screen);
}


/******************************************************************************
 ******************************************************************************
 * funkce reagujici na stisknuti klavesnice
 * key - udalost klavesnice
 */

void onKeyboard(SDL_KeyboardEvent *key)
{
    /* test existence rendereru */
	IZG_ASSERT(frame_buffer);

    /* vetveni podle stisknute klavesy */
    switch( key->keysym.sym )
    {
        /* ukonceni programu - klavesa Esc*/
        case SDLK_ESCAPE:
            quit = 1;
            break;

		case SDLK_l:
			/* Stisknuti L tlacitka nahraje obrazek */
			if (loadMyBitmap(input_image, &frame_buffer, &width, &height))
				IZG_INFO("File successfully loaded\n")
			else
				IZG_ERROR("Error in loading the file.\n");
            break;
		
		case SDLK_s:
			/* Stisknuti S tlacitka ulozi obrazek */
			if (saveMyBitmap(output_image, &frame_buffer, width, height))
				IZG_INFO("File successfully saved\n")
			else
				IZG_ERROR("Error in saving the file.\n");
            break;

		case SDLK_g:
			/* Stisknuti G prevadi obrazek do odstinu sedi */
			IZG_INFO("Converting image to gray scale.\n")
			grayScale();
            break;

		case SDLK_r:
			/* Stisknuti R tlacitka prevadi obrazek na cernobily pomoci nahodneho rozptyleni */
			IZG_INFO("Converting image to black and white using randomized dithering.\n")
			randomDithering();
            break;

        case SDLK_m:
			/* Stisknuti D tlacitka prevadi obrazek na cernobily pomoci maticoveho rozptyleni */
			IZG_INFO("Converting image to black and white using ordered dithering.\n")
			orderedDithering();
            break;

		case SDLK_d:
			/* Stisknuti D tlacitka prevadi obrazek na cernobily pomoci distribuce chyby */
			IZG_INFO("Converting image to black and white using error distribution.\n")
			errorDistribution();
            break;

		case SDLK_1:
			/* Stisknuti 1 tlacitka prevadi obrazek na cernobily pomoci prahovani s prahem = 50 */
			IZG_INFO("Converting image to black and white using thresholding (threshold = 50).\n")
			thresholding(50);
			break;

		case SDLK_2:
			/* Stisknuti 2 tlacitka prevadi obrazek na cernobily pomoci prahovani s prahem = 100 */
			IZG_INFO("Converting image to black and white using thresholding (threshold = 100).\n")
			thresholding(100);
			break;

		case SDLK_3:
			/* Stisknuti 3 tlacitka prevadi obrazek na cernobily pomoci prahovani s prahem = 150 */
			IZG_INFO("Converting image to black and white using thresholding (threshold = 150).\n")
			thresholding(150);
			break;

		case SDLK_4:
			/* Stisknuti 4 tlacitka prevadi obrazek na cernobily pomoci prahovani s prahem = 200 */
			IZG_INFO("Converting image to black and white using thresholding (threshold = 200).\n")
			thresholding(200);
			break;

        default:
            break;
    }
}

/******************************************************************************
 ******************************************************************************
 * funkce reagujici na zmacknuti tlacitka mysi
 * mouse - udalost mysi
 */

void onMouseDown(SDL_MouseMotionEvent *mouse)
{
    if( mouse->state & SDL_BUTTON_LMASK)
    {
        /* Pokud je zmacknuto tlacitko, cteme pixely za pomoci IZG_GetPixel metody */
		S_RGBA color = getPixel(mouse->x, mouse->y);
		printf("Info: Click on coordinates %d %d: RGB(%d %d %d)\n", mouse->x, mouse->y, color.red, color.green, color.blue);
    }
}

/******************************************************************************
 ******************************************************************************
 * funkce tiskne napovedu na stdout
 */
void printHelpText()
{
	IZG_INFO("Application loaded - IZG LAB 01 - Reduction of color space. Controls:\n\n"
			 "Left mouse click: \n"
			 "    Writes current pixel value to console\n"
			 "       (depends on GetPixel function)\n\n"
			 "L key:\n"
			 "    Loads the testing image.bmp image\n"
			 "       (depends on PutPixel function)\n\n"
			 "S key:\n"
			 "    Saves current view into out.bmp image\n"
			 "        (depends on GetPixel function)\n\n"
			 "G key:\n"
			 "    Converts the image into grayscale color space\n"
			 "       (depends on GetPixel function)\n"
			 "       (depends on PutPixel function)\n\n"
			 "1..4 key:\n"
			 "    Converts the image into black and white color space\n"
			 "       (depends on GetPixel function)\n"
			 "       (depends on PutPixel function)\n\n"
			 "R key:\n"
			 "    Converts the image into black and white color space using randomized dithering\n"
			 "       (depends on GetPixel function)\n"
			 "       (depends on PutPixel function)\n"
			 "       (depends on GrayScale function)\n\n"
			 "M key:\n"
			 "    Converts the image into black and white color space using ordered dithering\n"
			 "       (depends on GetPixel function)\n"
			 "       (depends on PutPixel function)\n"
			 "       (may depend on GrayScale function)\n\n"
			 "D key:\n"
			 "    Converts the image into black and white color space using error distribution\n"
			 "       (depends on GetPixel function)\n"
			 "       (depends on PutPixel function)\n"
			 "       (may depend on GrayScale function)\n\n"
			 "========================================================================\n\n")
}

/******************************************************************************
 ******************************************************************************
 * hlavni funkce programu
 * argc - pocet vstupnich parametru
 * argv - pole vstupnich parametru
 */

int main(int argc, char *argv[])
{
    SDL_Event event;
	
	/* Inicializace SDL knihovny */
    if( SDL_Init(SDL_INIT_VIDEO) == -1 )
    {
        IZG_SDL_ERROR("Could not initialize SDL library");
    }

    /* Nastaveni okna */
    SDL_WM_SetCaption(PROGRAM_TITLE, 0);
   
	/* Alokace frame bufferu (okno + SW zapisovaci */
	if (!(screen = SDL_SetVideoMode(width, height, 32, SDL_SWSURFACE)))
    {
        SDL_Quit();
        return 1;
    }

	if (!(frame_buffer = (S_RGBA *)malloc(sizeof(S_RGBA) * width * height)))
	{
		SDL_Quit();
        return 1;
	}

	printHelpText();

	/* Kreslime, dokud nenarazime na SDL_QUIT event */
	while(!quit) 
    {
        /* Reakce na udalost */
        while( SDL_PollEvent(&event) )
        {
            switch( event.type )
            {
                /* Udalost klavesnice */
                case SDL_KEYDOWN:
                    onKeyboard(&event.key);
                    break;

                /* Udalost mysi */
				case SDL_MOUSEBUTTONDOWN:
                    onMouseDown(&event.motion);
                    break;

                /* SDL_QUIT event */
                case SDL_QUIT:
                    quit = 1;
                    break;

                default:
                    break;
            }
        }

		/* Provedeme preklopeni zapisovaciho frame_bufferu na obrazovku*/
		onDraw();
    }

	/* Uvolneni pameti */
	SDL_FreeSurface(screen);
	free(frame_buffer);
    SDL_Quit();
  
	IZG_INFO("Bye bye....\n\n");
    return 0;
}


/*****************************************************************************/
/*****************************************************************************/
