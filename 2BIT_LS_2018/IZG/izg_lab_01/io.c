/******************************************************************************
 * Laborator 01 - Zaklady pocitacove grafiky - IZG
 * ihulik@fit.vutbr.cz
 *
 * $Id:$
 *
 * Popis:
 *    soubor obsahuje funkce pro zapis a cteni frame bufferu z/do souboru
 *
 * Opravy a modifikace:
 * -
 */

/******************************************************************************
 ******************************************************************************
 * Includes
 */

/* nase */
#include "io.h"
#include "base.h"
#include "student.h"

/* SDL knihovna */
#include <SDL.h>

/******************************************************************************
 ******************************************************************************
 * Nacteni BMP souboru do frame_buffer struktury
 */
int loadMyBitmap(const char *filename, S_RGBA** frame_buffer, int *width, int *height)
{
	/* Vytvorime SDL surface a nacteme do neho soubor */
	SDL_Surface *image;
	int x, y, bpp;
	if (!(image = SDL_LoadBMP(filename)))
	{
		IZG_SDL_ERROR("Input file does not exist");
		return 0;
	}

	/* Kontrola frame_bufferu, nasledne uvolneni a realokace (z duvodu jine velikosti - nezname velikosti, nemuzeme tudiz porovnat)*/
	IZG_ASSERT(*frame_buffer);
	free(*frame_buffer);

	if (!(*frame_buffer = (S_RGBA *)malloc(sizeof(S_RGBA) * image->h * image->w)))
	{
		IZG_ERROR("Cannot allocate the frame buffer");
		return 0;
	}

	/* Ulozime globalni promenne width a height */
	*width = image->w;
	*height = image->h;
	bpp = image->format->BytesPerPixel;

	/* pixel po pixelu ulozime do frame_bufferu (lepsi memcpy, ale pro demonstraci PutPixel) */
	for (y = 0; y < *height; ++y)
		for (x = 0; x < *width; ++x)
		{
			Uint8 *p = (Uint8 *)image->pixels + y * image->pitch + x * bpp;
			putPixel(x, y, makeColor(p[2], p[1], p[0]));
		}
	return 1;
}

/******************************************************************************
 ******************************************************************************
 * Ulozeni BMP souboru z frame_buffer struktury
 */
int saveMyBitmap(const char *filename, S_RGBA** frame_buffer,int width, int height)
{
	int x, y;
	/* Vytvorime RGB SDL Surface pro ukladani */
	SDL_Surface *image;

	IZG_ASSERT(*frame_buffer);
	if (!(image = SDL_CreateRGBSurface(SDL_SWSURFACE|SDL_ANYFORMAT, width, height, 32, 0xFF000000, 0xFF0000, 0xFF00, 0)))
	{
		IZG_SDL_ERROR("Cannot allocate the saving object");
		return 0;
	}

	/* Projdeme vsechny body framebufferu a ulozime do obrazovych dat (lepsi memcpy, ale pro demonstraci funkcnosti GetPixel) */
	for (y = 0; y < height; ++y)
		for (x = 0; x < width; ++x)
		{
			S_RGBA color = getPixel(x, y);
				*((Uint32 *)image->pixels + y * width + x) = SDL_MapRGB(image->format, color.red, color.green, color.blue);
		}

	/* Ulozime BMP */
	return !SDL_SaveBMP(image, filename);
	//return 1;
}

/*****************************************************************************/
/*****************************************************************************/
