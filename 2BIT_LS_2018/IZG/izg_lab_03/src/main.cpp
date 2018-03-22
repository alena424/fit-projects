/**
 * @file        main.cpp
 * @author      Ladislav Mosner, VUT FIT Brno, imosner@fit.vutbr.cz
 * @author      Petr Kleparnik, VUT FIT Brno, ikleparnik@fit.vutbr.cz
 * @author      Kamil Behun, VUT FIT Brno, ibehun@fit.vutbr.cz
 * @date        11.03.2018
 *
 * @brief       Zaklady pocitacove grafiky (IZG), 3. cviceni.
 *
 *      Ovladani programu:
 *          "Leva mys"      - Vypis hodnoty pixelu
 *          "Esc"           - Ukonceni programu
 *          "C"             - Vymaze frame buffer
 *          "S"             - Ulozeni obrazovky do out.bmp
 *          "R"             - Cervena barva
 *          "G"             - Zelena barva
 *          "B"             - Modra barva
 *          "T"             - Pineduv algoritmus pro trojuhelniky
 *          "P"             - Pineduv algoritmus pro polygony
 *          "0-9"           - Testy na Pineduv algoritmus
 *
 */

#ifdef _WIN32
#include <windows.h>
#endif

#include "color.h"
#include "student.h"
#include "io.h"
#include "globals.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

// Globalni konstanty a promenne

// Titulek hlavniho okna
const char *PROGRAM_TITLE = "IZG/2017 Lab 03";

// Vychozi velikost okna
const int DEFAULT_WIDTH = 800;
const int DEFAULT_HEIGHT = 600;

// Kkreslici buffer knihovny SDL
SDL_Surface *screen = 0;

// Kreslici buffer IZG cvičení
RGBA *framebuffer = 0;

// Pomocna promenna pro ukonceni aplikace
int quit = 0;

// Sirka a vyska okna
int width = 800;
int height = 600;

// Nazvy i/o souboru
char *outputImageName = "data/out.bmp";

// Vychozi barvy
// Vnitrek
RGBA color1 = COLOR_ROYALBLUE;
// Hrana
RGBA color2 = COLOR_WHITE;

int background = 0;

// Zasobnik bodu
SeedStack points;

// Stav pouziti algoritmu
UseAlg algorithm;
bool isOld = false;
int pointCount = 0;

/**
 * @brief Vytvori 1D pole bodu (typu Point) z 2D pole
 * @param[in] points Vstupni 2D pole bodu
 * @param[in] length Delka pole bodu
 * @return Vraci pole bodu typu Point
 */
Point *makeSeedStack(const int points[][2], int length) {
    Point *s = new Point[(unsigned int) length];
    for (int i = 0; i < length; i++) {
        s[i] = Point(points[i][0], points[i][1]);
    }
    return s;
}

/**
 * @brief Zkontroluje orientaci vektoru v poli a pripadne opravi na clockwise
 * @param[in,out] array Pole bodu polygonu
 * @param[in] length Delka pole bodu
 */
void clockwiseOrientatedArray(Point *array, int length) {
    int accum = 0;

    for (int i = 0; i < length; i++) {
        Point p1 = array[i];
        Point p2 = array[(i + 1) % length];
        accum += (p2.x - p1.x) * (p2.y + p1.y);
    }

    if (accum >= 0) {
        Point temp;
        for (int i = 0; i < length / 2; ++i) {
            temp = array[length - i - 1];
            array[length - i - 1] = array[i];
            array[i] = temp;
        }
    }
}

/**
 * @brief Spusti jeden z testu
 * @param[in] test Cislo testu
 */
void runTest(int test) {
    Point *points(0);

    switch (test) {
        // 1) Try to create non-convex polygon
        case 1: {
            const int points1[5][2] = {{654, 28}, {758, 28}, {724, 61}, {762, 96}, {644, 95}};
            points = makeSeedStack(points1, 5);
            clockwiseOrientatedArray(points, 5);
            pinedaPolygon(points, 5, COLOR_SALMON, color2);
            delete[] points;
            break;
        }
        // 2) Try to create a triangle
        case 2: {
            const int points2[3][2] = {{30, 88}, {182, 22}, {175, 128}};
            points = makeSeedStack(points2, 3);
            pinedaTriangle((const Point) points[0], (const Point) points[1], (const Point) points[2], COLOR_SALMON, color2);
            delete[] points;
            break;
        }
        // 3) Try to create triangle out of view
        case 3: {
            const int points3[3][2] = {{256, 121}, {250, -10}, {900, 200}};
            points = makeSeedStack(points3, 3);
            pinedaTriangle((const Point) points[0], (const Point) points[1], (const Point) points[2], COLOR_SALMON, color2);
            delete[] points;
            break;
        }
        // 4) Try to create polygon with intersection
        case 4: {
            const int points4[12][2] = {{47, 214}, {54, 174}, {112, 140}, {172, 169}, {178, 206}, {157, 229}, {113, 229}, {109, 198}, {136, 183}, {153, 202}, {132, 242}, {72, 237}};
            points = makeSeedStack(points4, 12);
            clockwiseOrientatedArray(points, 12);
            pinedaPolygon(points, 12, COLOR_SALMON, color2);
            delete[] points;
            break;
        }
        // 5) Try to create small polygon
        case 5: {
            const int points5[8][2] = {{292, 230}, {327, 174}, {417, 168}, {436, 204}, {433, 250}, {395, 293}, {344, 293}, {302, 280}};
            points = makeSeedStack(points5, 8);
            clockwiseOrientatedArray(points, 8);
            pinedaPolygon(points, 8, COLOR_SALMON, color2);
            delete[] points;
            break;
        }
        // 6) Try to create wrong polygon - zero points
        case 6: {
            points = new Point[0];
            pinedaPolygon(points, 0, COLOR_SALMON, color2);
            delete[] points;
            break;
        }
        // 7) Try to create wrong polygon - two points. Line must be drawn.
        case 7: {
            const int points7[2][2] = {{424, 17}, {599, 64}};
            points = makeSeedStack(points7, 2);
            pinedaPolygon(points, 2, COLOR_SALMON, color2);
            delete[] points;
            break;
        }
        // 8) Try to create really big polygon
        case 8: {
            const int points8[9][2] = {{351, 501}, {353, 372}, {429, 265}, {532, 227}, {686, 217}, {781, 282}, {777, 428}, {711, 569}, {540, 587}};
            points = makeSeedStack(points8, 9);
            clockwiseOrientatedArray(points, 9);
            pinedaPolygon(points, 9, COLOR_SALMON, color2);
            delete[] points;
            break;
        }
        // 9) Try to create really big polygon out of view
        case 9: {
            const int points9[4][2] = {{264, 316}, {-1000, 270}, {-1000, 1000}, {330, 1000}};
            points = makeSeedStack(points9, 4);
            clockwiseOrientatedArray(points, 4);
            pinedaPolygon(points, 4, COLOR_SALMON, color2);
            delete[] points;
            break;
        }
        // 10) Try to create polygon with zero length side
        case 10: {
            const int points10[6][2] = {{195, 150}, {195, 180}, {195, 200}, {270, 190}, {270, 170}, {270, 160}};
            points = makeSeedStack(points10, 6);
            clockwiseOrientatedArray(points, 6);
            pinedaPolygon(points, 6, COLOR_SALMON, color2);
            delete[] points;
            break;
        }
    }
}

/**
 * @brief Copy bitmap
 * @param[in] srcColors Source color pixels
 * @param[out] dstColors Destination color pixels
 * @param[in] x Destination position x
 * @param[in] y Destination position y
 * @param[in] srcWidth Source width
 * @param[in] srcHeight Source height
 * @param[in] dstWidth Destination width
 */
void copyBuffer(RGBA *srcColors, RGBA *dstColors, int srcWidth, int srcHeight, int dstX, int dstY, int dstWidth) {
    // Prepocitana pozice v cilovem bufferu;
    RGBA *dstColorNew(dstColors + dstX + dstY * dstWidth);

    // Castecna kopie obsahu frame bufferu
    for (int i = 0; i < srcHeight; i++) {
        memcpy(dstColorNew + i * dstWidth, srcColors + i * srcWidth, srcWidth * sizeof(RGBA));
    }
}

/**
 * @brief Run all tests and store them to the tiled bitmap
 */
void runAllMakeTiled() {
    int fw = 5 * width;
    int fh = 2 * height;

    // Memory allocation
    RGBA *tiled = new RGBA[(unsigned int) (fw * fh)];
    memset(tiled, 0, fw * fh * sizeof(RGBA));

    // All tests
    for (int y = 0; y < 2; ++y) {
        for (int x = 0; x < 5; ++x) {
            // Resolve test number
            int test = 1 + y * 5 + x;

            // Clear frame buffer
            memset(framebuffer, background, height * width * sizeof(RGBA));

            // Run test
            runTest(test);

            // Copy bitmap
            copyBuffer(framebuffer, tiled, width, height, x * width, y * height, fw);
        }
    }

    // Clear frame buffer
    memset(framebuffer, background, height * width * sizeof(RGBA));

    // Save bitmap
    saveMyBitmap("tiled.bmp", &tiled, fw, fh);

    // Dealokace pameti
    delete[] tiled;
}

/**
 * @brief Prekresleni obsahu okna programu
 */
void onDraw(void) {
    // Test existence frame bufferu a obrazove pameti
    IZG_ASSERT(framebuffer && screen);

    // Test typu pixelu
    IZG_ASSERT(screen->format->BytesPerPixel == 4);

    // Kopie bufferu do obrazove pameti
    SDL_LockSurface(screen);

    // Test, pokud kopirujeme rozdilnou velikost framebufferu a rozdilne pameti, musime pamet prealokovat
    if (width != screen->w || height != screen->h) {
        SDL_SetVideoMode(width, height, 32, SDL_SWSURFACE);
        /*SDL_FreeSurface(screen);
        if (!(screen = SDL_SetVideoMode(width, height, 32, SDL_SWSURFACE|SDL_ANYFORMAT)))
        {
        IZG_ERROR("Cannot realocate screen buffer");
        SDL_Quit();
        }*/
    }

    MEMCOPY(screen->pixels, framebuffer, sizeof(RGBA) * width * height);
    SDL_UnlockSurface(screen);

    // Vymena zobrazovaneho a zapisovaneho bufferu
    SDL_Flip(screen);
}

/**
 * @brief Funkce reagujici na stisknuti klavesnice
 * @param[in] key Udalost klavesnice
 */
void onKeyboard(SDL_KeyboardEvent *key) {
    // Test existence rendereru
    IZG_ASSERT(framebuffer);

    // Vetveni podle stisknute klavesy
    switch (key->keysym.sym) {
        case SDLK_c:
            memset(framebuffer, background, width * height * sizeof(RGBA));
            break;
        case SDLK_r:
            if (key->keysym.mod & KMOD_LCTRL)
                color2 = COLOR_RED;
            else
                color1 = COLOR_RED;
            break;
        case SDLK_g:
            if (key->keysym.mod & KMOD_LCTRL)
                color2 = COLOR_GREEN;
            else
                color1 = COLOR_GREEN;
            break;
        case SDLK_b:
            if (key->keysym.mod & KMOD_LCTRL)
                color2 = COLOR_BLUE;
            else
                color1 = COLOR_BLUE;
            break;
        case SDLK_w:
            if (key->keysym.mod & KMOD_LCTRL)
                color2 = COLOR_WHITE;
            else
                color1 = COLOR_WHITE;
            break;
        case SDLK_k:
            if (key->keysym.mod & KMOD_LCTRL)
                color2 = COLOR_BLACK;
            else
                color1 = COLOR_BLACK;
            break;
        case SDLK_t:
            algorithm = USE_PINEDA_TRIANGLE_FILL;
            break;
        case SDLK_p:
            algorithm = USE_PINEDA_POLYGON_FILL;
            points.erase(points.begin(), points.end());
            break;
        // Testy, jake budou pouzity i pri kontrole ukolu.
        case SDLK_0:
        case SDLK_1:
        case SDLK_2:
        case SDLK_3:
        case SDLK_4:
        case SDLK_5:
        case SDLK_6:
        case SDLK_7:
        case SDLK_8:
        case SDLK_9:
            runTest(key->keysym.sym - SDLK_0 + 1);
            break;
            // ukonceni programu - klavesa Esc
        case SDLK_ESCAPE:
            quit = 1;
            break;

        case SDLK_s:
            // Stisknuti S tlacitka ulozi obrazek
            if (saveMyBitmap(outputImageName, &framebuffer, width, height))
                IZG_INFO("File successfully saved\n")
            else
            IZG_ERROR("Error in saving the file.\n");
            break;
        default:
            break;
    }
}

/**
 * @brief Funkce reagujici na zmacknuti tlacitka mysi
 * @param mouse Udalost mysi
 */
void onMouseDown(SDL_MouseButtonEvent *mouse) {
    if (mouse->button == SDL_BUTTON_LEFT) {
        switch (algorithm) {
            case USE_PINEDA_POLYGON_FILL:
                putPixel(mouse->x, mouse->y, COLOR_GREEN);
                points.push_back(Point(mouse->x, mouse->y));
                pointCount++;
                isOld = true;
                break;
            case USE_PINEDA_TRIANGLE_FILL:
                putPixel(mouse->x, mouse->y, COLOR_GREEN);
                points.push_back(Point(mouse->x, mouse->y));
                pointCount++;
                isOld = true;
                if (pointCount >= 3) {
                    Point *pointsTmp;
                    STACK_TO_ARRAY(points, pointsTmp);
                    // Fill polygon
                    clockwiseOrientatedArray(pointsTmp, 3);
                    pinedaTriangle(pointsTmp[0], pointsTmp[1], pointsTmp[2], color1, color2);
                    delete[] pointsTmp;
                    points.clear();
                    isOld = false;
                    pointCount = 0;
                }
                break;
        }
    }

    if (mouse->button == SDL_BUTTON_RIGHT) {
        if (algorithm == USE_PINEDA_POLYGON_FILL && isOld) {
            Point *pointTmp;
            STACK_TO_ARRAY(points, pointTmp);
            // Fill polygon
            clockwiseOrientatedArray(pointTmp, (int) points.size());

            // Vypis souradnic
            /*printf("{");
            for (int i = 0; i < points.size(); i++) {
                if (i == 0) {
                    printf("{%d, %d}", points[0].x, points[0].y);
                } else {
                    printf(", {%d, %d}", points[i].x, points[i].y);
                }
            }
            printf("}\n");*/

            pinedaPolygon(pointTmp, (const int) points.size(), color1, color2);
            delete[] pointTmp;
            points.clear();
            isOld = false;
            pointCount = 0;
        }
    }

}

/**
 * @brief Tiskne napovedu
 */
void printHelpText() {
    IZG_INFO("Application loaded - IZG LAB 03 - 2D area filling. Controls:\n\n"
        "Right mouse click: \n"
        "    Applies selected Pineda's algorithm\n\n"
        "Left mouse click:\n"
        "    Adds new point to polygon\n\n"
        "S key:\n"
        "    Saves current view into out.bmp image\n"
        "        (depends on GetPixel function)\n\n"
        "C key:\n"
        "    Clears framebuffer\n\n"
        "0..9 key:\n"
        "    Run test for Pineda's algorithm)\n\n"
        "T key:\n"
        "    Use Pineda's algorithm for triangles\n\n"
        "P key:\n"
        "    Use Pineda's algorithm for polygons\n\n"
        "R key:\n"
        "    Sets color to red \n\n"
        "G key:\n"
        "    Sets color to green \n\n"
        "B key:\n"
        "    Sets color to blue \n\n"
        "W key:\n"
        "    Sets color to white \n\n"
        "K key:\n"
        "    Sets color to black \n\n"
        "\n\n")
}

/**
 * @brief Hlavni funkce programu
 * @param argc Pocet vstupnich parametru
 * @param argv Pole vstupnich parametru
 * @return
 */
int main(int argc, char *argv[]) {
    SDL_Event event;

    // Inicializace SDL knihovny
    if (SDL_Init(SDL_INIT_VIDEO) == -1) {
        IZG_SDL_ERROR("Could not initialize SDL library");
    }

    // Nastaveni okna
    SDL_WM_SetCaption(PROGRAM_TITLE, 0);

    // Alokace frame bufferu (okno + SW zapisovaci buffer)
    if (!(screen = SDL_SetVideoMode(width, height, 32, SDL_SWSURFACE))) {
        SDL_Quit();
        return 1;
    }
    if (!(framebuffer = (RGBA *) malloc(sizeof(RGBA) * width * height))) {
        SDL_Quit();
        return 1;
    }

    // Clear framebuffer
    memset(framebuffer, background, width * height * sizeof(RGBA));

    printHelpText();

    //runAllMakeTiled();

    // Kreslime, dokud nenarazime na SDL_QUIT event
    while (!quit) {
        // Reakce na udalost
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                // Udalost klavesnice
                case SDL_KEYDOWN:
                    onKeyboard(&event.key);
                    break;

                    // Udalost mysi
                case SDL_MOUSEBUTTONDOWN:
                    onMouseDown(&event.button);
                    break;

                    // SDL_QUIT event
                case SDL_QUIT:
                    quit = 1;
                    break;

                default:
                    break;
            }
        }

        // Provedeme preklopeni zapisovaciho framebufferu na obrazovku
        onDraw();
    }

    // Uvolneni pameti
    SDL_FreeSurface(screen);
    free(framebuffer);
    SDL_Quit();

    IZG_INFO("Bye bye....\n\n");
    return 0;
}
