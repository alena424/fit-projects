/******************************************************************************
 * Projekt - Zaklady pocitacove grafiky - IZG Lab 02
 * isolony@fit.vutbr.cz
 *
 * $Id: $
 *
 * *) Ovladani programu:
 *      "Leva mys"		- Kresleni cary
 *      "Prava mys"		- Kresleni kruznice
 *      "Esc"			- ukonceni programu
 *      "L"				- Nahrani zkusebniho obrazku image.bmp
 *      "S"				- Ulozeni obrazovky do out.bmp
 *      "D"				- Vykresleni usecek ze souboru lines.txt
 *
 * Opravy a modifikace:
 * itomesek@fit.vutbr.cz 
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
const char     * PROGRAM_TITLE  = "IZG 2018 Lab 02";

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
const char *				input_image		= "../data/image.bmp";
const char *				output_image	= "../data/out.bmp";
#ifdef _WIN32
const char *				lines_file    = "../data/lines.txt";
#else
const char *				lines_file    = "data/lines.txt";
#endif

/* Body pro vykresleni primitiv */
int					x_begin, 
					y_begin, 
					x_end, 
					y_end;


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

		case SDLK_d:
			/* Stisknutim tlacitka D vykreslite usecky ze souboru */
			if(drawLinesFromFile(lines_file)) {
				IZG_INFO("Drawing lines from the file succeeded\n")
			} else {
				IZG_INFO("ERROR in drawing lines from the file\n")
			}

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
        /* Pokud je zmacknuto tlacitko, ulozime pocatecni bod cary */
		x_begin = mouse->x;
		y_begin = mouse->y;
		printf("Info: Beginning / center point %d %d:\n", mouse->x, mouse->y);
}

/******************************************************************************
 ******************************************************************************
 * funkce reagujici na zmacknuti tlacitka mysi
 * mouse - udalost mysi
 */

void onLeftMouseUp(SDL_MouseMotionEvent *mouse)
{
        /* Pokud je pustene tlacitko, ulozime konecny bod cary a zavolame vykresleni */
		x_end = mouse->x;
		y_end = mouse->y;
		printf("Info: End point of line %d %d:\n", mouse->x, mouse->y);

		drawLine(x_begin, y_begin, x_end, y_end);
}

/******************************************************************************
 ******************************************************************************
 * funkce reagujici na zmacknuti tlacitka mysi
 * mouse - udalost mysi
 */

void onRightMouseUp(SDL_MouseMotionEvent *mouse)
{
        /* Pokud je pustene tlacitko, zistime polomer kruznice a zavolame vykresleni */
		x_end = mouse->x;
		y_end = mouse->y;

		int radius = int(sqrt(1.0 * (x_begin - x_end) * (x_begin - x_end) + (y_begin - y_end) * (y_begin - y_end)));
		printf("Info: Radius: %d\n", radius);

		drawCircle(x_begin, y_begin, radius);
}

/******************************************************************************
 ******************************************************************************
 * funkce tiskne napovedu na stdout
 */
void printHelpText()
{
	IZG_INFO("Application loaded - IZG LAB 02 - Vector Objects Rasterization. Controls:\n\n"
			 "Left mouse click down: \n"
			 "    Sets the beginning point of line\n\n"
			 "Left mouse click up: \n"
			 "    Sets the ending point of line\n\n"
			 "Right mouse click down: \n"
			 "    Sets the center point of circle\n\n"
			 "Right mouse click up: \n"
			 "    Sets the radius of circle\n\n"
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

			    /* Udalost mysi */
				case SDL_MOUSEBUTTONUP:
					if( event.button.button == SDL_BUTTON_LEFT )
						onLeftMouseUp(&event.motion);
					else
						onRightMouseUp(&event.motion);
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