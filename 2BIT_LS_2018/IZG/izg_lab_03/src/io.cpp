/**
 * @file        color.cpp
 * @author      Ladislav Mosner, VUT FIT Brno, imosner@fit.vutbr.cz
 * @author      Petr Kleparnik, VUT FIT Brno, ikleparnik@fit.vutbr.cz
 * @author      Kamil Behun, VUT FIT Brno, ibehun@fit.vutbr.cz
 * @date        11.03.2018
 *
 * @brief       Definice funkci pro zapis a cteni frame_buffer z/do souboru.
 *
 */

#include "io.h"
#include "base.h"
#include "student.h"

// SDL knihovna
#include <SDL.h>

/**
 * @brief Nacteni BMP souboru do framebuffer struktury
 * @param[in] filename Cesta k BMP souboru
 * @param[out] framebuffer Framebuffer
 * @param[out] width Sirka framebufferu
 * @param[out] height Vyska framebufferu
 * @return Vraci 0/1
 */
int loadMyBitmap(const char *filename, RGBA** framebuffer, int *width, int *height) {
    // Vytvorime SDL surface a nacteme do neho soubor
    SDL_Surface *image;
    int x, y, bpp;
    if (!(image = SDL_LoadBMP(filename))) {
        IZG_SDL_ERROR("Input file does not exist");
        return 0;
    }

    // Kontrola framebufferu, nasledne uvolneni a realokace
    // (z duvodu jine velikosti - nezname velikosti, nemuzeme tudiz porovnat)
    IZG_ASSERT(*framebuffer);
    free(*framebuffer);

    if (!(*framebuffer = (RGBA *) malloc(sizeof(RGBA) * image->h * image->w))) {
        IZG_ERROR("Cannot allocate the frame buffer");
        return 0;
    }

    // Ulozime globalni promenne width a height
    *width = image->w;
    *height = image->h;
    bpp = image->format->BytesPerPixel;

    // Pixel po pixelu ulozime do framebuffer
    for (y = 0; y < *height; ++y) {
        for (x = 0; x < *width; ++x) {
            Uint8 *p = (Uint8 *) image->pixels + y * image->pitch + x * bpp;
            (*framebuffer)[y * *width + x] = makeColor(p[2], p[1], p[0]);
        }
    }
    return 1;
}

/**
 * @brief Ulozeni BMP souboru z framebuffer struktury
 * @param[in] filename Cesta k BMP souboru
 * @param[in] framebuffer Framebuffer
 * @param[in] width Sirka framebufferu
 * @param[in] height Vyska framebufferu
 * @return Vraci 0/1
 */
int saveMyBitmap(const char *filename, RGBA** framebuffer, int width, int height) {
    int x, y;

    // Vytvorime RGB SDL Surface pro ukladani
    SDL_Surface *image;

    IZG_ASSERT(*framebuffer);
    if (!(image = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_ANYFORMAT, width, height, 32, 0xFF000000, 0xFF0000, 0xFF00, 0))) {
        IZG_SDL_ERROR("Cannot allocate the saving object");
        return 0;
    }

    // Projdeme vsechny body framebufferu a ulozime do obrazovych dat
    for (y = 0; y < height; ++y) {
        for (x = 0; x < width; ++x) {
            RGBA color = (*framebuffer)[y * width + x];
            *((Uint32 *) image->pixels + y * width + x) = SDL_MapRGB(image->format, color.red, color.green, color.blue);
        }
    }

    // Ulozime BMP
    return !SDL_SaveBMP(image, filename);
}
