/**
 * @file primes.c
 * Reseni IJC-DU1, priklad b)
 *
 * @date 25.3.2017
 * @author Alena Tesarova, FIT
 * @gcc 5.4
 *
 * Popis:
 * Funkce pro zpracovani obrazku .ppm
 *
*/

#include <stdlib.h>
#include <stdio.h>
#include "error.h"
#include "ppm.h"

/**
* @brief funkce nacte obsah PPM souboru do touto funkcí dynamicky alokované struktury. Pri chybe formatu funkce: warning_msg a vrátí NULL
* @param filename - soubor (na prvnim miste je format, potom je popis, [sirka, vyska], pole bitu)
*/

struct ppm * ppm_read(const char * filename)
{
    FILE *img = fopen( filename, "rb" );
    if ( ! img )
    {
        error_msg( "Nepodarilo se nam nacist soubor" );
    }
    char format[2];
    unsigned int sirka, vyska, maxColor;

    if(fscanf(img, "%2s %u %u %u%*c", format, &sirka, &vyska, &maxColor) != 4)
    {
        error_msg( "Soubor je ve spatnem formatu" );
        fclose(img);
        return 0;
    }

    if ( format[0] != 'P' )
    {
        error_msg("Pracujeme se spatnym typem souboru, oprazek musi byt ve formatu .pmm");
    }
    unsigned int rozmery = vyska * sirka *3;
    if (rozmery > VELIKOST)
    {
        error_msg("Obrazek je prilis velky");
        fclose(img);
        return 0;
    }
    struct ppm *p = NULL;
    if((p = (struct ppm *) malloc(sizeof(struct ppm) + rozmery)) == NULL)
    {
        error_msg("Chyba pri alokaci pameti");
        fclose(img);
        return 0;
    }
    p->xsize = sirka;
    p->ysize = vyska;
    unsigned char pomocnepole[3] = { 0 };
    unsigned long i = 0;

    while (fread(pomocnepole, sizeof(unsigned char), 3, img) == 3)
    {
        //printf("pomocne pole = %03d %03d %03d \n", pomocnepole[0], pomocnepole[1], pomocnepole[2]);
        if (i > rozmery)
        {
            warning_msg("Moc velky obrazek\n");
            free(p);
            fclose(img);
            return 0;
        }

        p->data[i]	= pomocnepole[0];
        // Red
        //printf("%d ", pomocnepole[0]);
        p->data[i +1] = pomocnepole[1];		// Blue
        //printf("%d ", pomocnepole[1]);

        p->data[i +2] = pomocnepole[2];		// Green
        //printf("%d ", pomocnepole[2]);


        i += 3;
    }//while

    if (!feof(img))
    {
        warning_msg("Spatne zadany obrazek\n");
        free(p);
        fclose(img);
        return 0;
    }
    fclose(img);
    return p;

}

/**
* @brief Zapíše obsah struktury p do souboru ve formátu PPM. Pri chybe funkce warning_msg a vrátime záporné císlo.
* @param p vytvorena structura z ppm_read
*
*/
int ppm_write(struct ppm *p, const char * filename)
{

    FILE *img = fopen( filename, "wb" );
    if ( ! img )
    {
        error_msg( "Nepodarilo se nam nacist soubor" );
        return 1;
    }
    unsigned int rozmery = p->xsize * p->xsize * 3;
    if (fprintf(img,"P6\n%u %u\n255\n",p->xsize,p->ysize ) < 0)
    {
        fclose(img);
        error_msg("Nepodarilo se zapsat bity do pole\n");
        return 1;
    }

    if ((fwrite (&p->data , sizeof(unsigned char), rozmery, img) ) != rozmery )
    {
        fclose(img);
        error_msg("Nepodaril se zapis\n ");
        return 1;

    }
    fclose(img);
    return 0;



}

