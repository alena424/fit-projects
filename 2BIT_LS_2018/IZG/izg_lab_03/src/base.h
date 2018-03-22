/**
 * @file        base.h
 * @author      Ladislav Mosner, VUT FIT Brno, imosner@fit.vutbr.cz
 * @author      Petr Kleparnik, VUT FIT Brno, ikleparnik@fit.vutbr.cz
 * @author      Kamil Behun, VUT FIT Brno, ibehun@fit.vutbr.cz
 * @date        11.03.2018
 *
 * @brief       Pomocne makra, definice pomocnych struktur.
 *
 */

#ifndef BASE_H
#define BASE_H

// Specialni nastaveni pro MS Visual C++
// Vypne varovani o pouziti "unsafe" fci ze standardni C knihovny
#ifndef _CRT_SECURE_NO_WARNINGS
    #if defined _WIN32 && defined _MSC_VER
        #define _CRT_SECURE_NO_WARNINGS
    #endif
#endif // _CRT_SECURE_NO_WARNINGS

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>

// Definice inline funkce v ruznych prostredich
#ifndef IZG_INLINE
    #if defined __cplusplus
        #define IZG_INLINE inline
    #elif defined _WIN32 && !defined __GNUC__
        #define IZG_INLINE __inline
    #else
        #define IZG_INLINE static
    #endif
#endif /* IZG_INLINE */

// Definice pomocnych struktur

// Pouzity algoritmus
enum UseAlg
{
    USE_PINEDA_TRIANGLE_FILL,
    USE_PINEDA_POLYGON_FILL
};

// Bod ve 2D
struct Point
{
    int x, y;   // Souradnice bodu

    // Konstruktory (C++)
    Point() {}
    Point(int _x, int _y) : x(_x), y(_y) {}
    Point(const Point& p) : x(p.x), y(p.y) {}

    Point operator=(const Point &p) {
        x = p.x;
        y = p.y;
        return p;
    }
};

// Parametry hranovych funkci
struct EdgeParam
{
	int deltaX, deltaY;    // Parametry hranove funkce

    // Konstruktory (C++)
    EdgeParam() {}
	EdgeParam(int _deltaX, int _deltaY) : deltaX(_deltaX), deltaY(_deltaY) {}

    EdgeParam operator=(const EdgeParam &p)
    {
        deltaX = p.deltaX;
        deltaY = p.deltaY;
        return p;
    }
};

// Typ - vektor bodu
typedef std::vector<Point> SeedStack;

// Typ - vektor hodnot hranove funkce
typedef std::vector<int> EdgeFncValues;

// Typ - vektor parametru hranove funkce
typedef std::vector<EdgeParam> EdgeParams;

// Create C array from the vector of points
#define STACK_TO_ARRAY(stack, points) \
{\
    points = new Point[stack.size()];\
    SeedStack::iterator i;\
    int j(0);\
    for (i = stack.begin(); i != stack.end(); ++i)\
        points[j++] = *i;\
}

// Uzitecna matematicka makra

// Prohodi hodnoty dvou promennych typu int
#define SWAP(a, b)      {(b) ^= (a); (a) ^= (b); (b) ^= (a);}

// Zaokrouhli hodnotu typu double na int
#define ROUND(x)        ((int)((x) + 0.5f))

// Zaokrouhli hodnotu typu double na unsigned char
#define ROUND2BYTE(x)   ((unsigned char)((x) + 0.5f))

// Absolutni hodnota
#define ABS(x)          (((x) > 0) ? (x) : (-(x)))

// Minimum ze dvou zadanych hodnot
#define MIN(a, b)       (((a) < (b)) ? (a) : (b))

// Maximum ze dvou zadanych hodnot
#define MAX(a, b)       (((a) > (b)) ? (a) : (b))

// Jednoduche testovani a zpracovani runtime errors

// Kontrola zadane podminky, ktera muze byt ve finalnim kodu odstranena
#define IZG_ASSERT(cond) assert(cond)

// Zobrazi chybove hlaseni a ukonci program
#define IZG_ERROR(msg)\
{\
    fprintf(stderr, "<%s, %d> Error: %s", __FILE__, __LINE__, (msg));\
    system("pause");\
    exit(-1);\
}

// Info hlaseni
#define IZG_INFO(msg)\
{\
    fprintf(stdout, "Info: %s", (msg));\
}

// Zobrazi chybove hlaseni vcetne popisu chyby v SDL knihovne a ukonci program
#define IZG_SDL_ERROR(msg)\
{\
    fprintf(stderr, "<%s, %d> Error: %s, SDL: %s", __FILE__, __LINE__, (msg), SDL_GetError());\
    exit(-1);\
}

// Otestuje podminku a generuje chybu pokud neplati
#define IZG_CHECK(cond, msg)\
{\
    if (!cond) {\
        IZG_ERROR(msg);\
    }\
}

// A jeste nejaka makra navic...

// Makro zkopiruje blok pameti o zadane velikosti (nahrada funkce memcpy)
#define MEMCOPY(dst, src, length)\
{\
    char *_dst = (char *)(dst);\
    const char *_src = (const char *)(src);\
    size_t _length = (length);\
    if ((_length &(sizeof(int) - 1)) == 0) {\
        for (size_t i = 0; i < _length; i += sizeof(int))\
            *((int *)(_dst + i)) = *((const int *)(_src + i));\
    } else {\
        for (size_t i = 0; i < _length; ++i)\
            *(_dst + i) = *(_src + i);\
    }\
}

#endif // BASE_H
