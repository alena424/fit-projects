/**
 * @file tail.c
 * @brief IJC-DU2, priklad a)
 *
 * @date 11.4.2017
 * @author Alena Tesarova, FIT, xtesar36
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> // for function isdigit()
#include <string.h>

#define RADEK_MAX 1024
#define POCET_R 10 //defaultne nastavujeme 10 radku

typedef struct rad
{
    int pocet;
    char **radky;
} Rad;

/**
* @brief Funkce cte ze souboru po zmaku, radky uklada do struktury output
* @pre Radek neni delsi nez RADEK_MAX
* @param f slozka pripadne stdout ze ktereho cteme znaky
* @param output struktura, do ktere ukladame
*/
void getlines( FILE *f, Rad *output )
{
    int i = 0; //index

    int lastindex = 0;
    int limit = 0;
    char c;
    int poc = 0; //celkovy pocet nactenych radku
    char row[RADEK_MAX];
    while (fgets(row, RADEK_MAX+1, f) != NULL)
    {

        lastindex = strlen(row);
        row[lastindex - 1] = '\0';
        if ( lastindex >= RADEK_MAX )
        {
            fprintf(stderr, "Can not read line, line is too big\n");
            //skipnout vse do '\n'
            while (((c = fgetc(f)) != '\n') && (c != EOF))
            {
                continue;
            }
        }

        if ( i >= ( output->pocet ) )
        {
            i = 0;
        }
        //puts(row);
        if ( output->pocet != 0 )
        {
            strcpy(output->radky[i], row);
            i++;
            poc++;
        }

    }
    // ted je v i posledni index
    if ( poc < output->pocet )
    {
        limit = poc;
    }
    else
    {
        limit = output->pocet;
    }
    int j = 0;
    while (j != limit  ) //pocitadlo radku
    {
        if (  i == limit) // i je pro indexovani - rotujeme
        {
            i = 0;
        }
        printf("%s\n", output->radky[i]);
        i++;
        j++;
    }
}

/**
* @brief inicializace struktury pro zadany pocet radku
* @param pocet pocet radku, defaulne 10
* @todo uverit malloc
*/
Rad init( int pocet )
{

    Rad output;
    output.pocet = pocet;
    output.radky = malloc(sizeof(char*) * pocet);
    /*if ( output.radky == NULL )
    {
        fprintf(stderr, "Can not get memory for structure\n" );
        return;
    }*/

    for (int i = 0; i < pocet; i++)
    {
        output.radky[i] = malloc(sizeof(char) * RADEK_MAX);
        /*if ( output.radky[i] == NULL )
        {
            fprintf(stderr, "Can not get memory for structure\n" );
            return;
        }*/
    }
    return output;
}
/**
* @brief uvolneni structury
*/
void freememory(Rad *output)
{
    for (int i = 0; i < output->pocet; i++)
    {
        free(output->radky[i]);
    }
    free(output->radky);
}

int main( int argc, char *argv[] )
{

    char* soubor = NULL;
    char* endptr = NULL;
    int pocet_radku = 0;
    switch (argc)
    {

    case 1:
        pocet_radku = POCET_R;

    case 2:
        soubor = argv[1];
        pocet_radku = POCET_R;
        break;

    case 3:
        if ( strcmp( argv[1], "-n" ) != 0 )
        {
            fprintf(stderr, "Argument -n needed\n");
            return EXIT_FAILURE;
        }
        if ( isdigit( strtol(argv[2], NULL, 10) ) )
        {
            fprintf(stderr, "Argument after -n must be a number\n");
            return EXIT_FAILURE;
        }
        pocet_radku = strtol(argv[2], &endptr, 10);

        break;
    case 4:
        if ( strcmp( argv[1], "-n" ) != 0 )
        {
            fprintf(stderr, "Argument -n needed\n");
            return EXIT_FAILURE;
        }
        if ( isdigit( strtol(argv[2], NULL, 10) ) )
        {
            fprintf(stderr, "Argument after -n must be a number\n");
            return EXIT_FAILURE;
        }
        pocet_radku = strtol(argv[2], &endptr, 10);
        soubor = argv[3];
        break;
    default:
        fprintf(stderr, "Bad format of arguments\n");
        return EXIT_FAILURE;
    }
    FILE *f  = NULL;
    if (soubor)
    {
        f = fopen( soubor, "r+");
        if ( f == NULL )
        {
            fprintf(stderr, "Can not find file %s\n", soubor);
            return EXIT_FAILURE;
        }
    }
    else
    {
        f = stdin;
    }
    //inicalize
    if ( *endptr != 0 )
    {
        fprintf(stderr, "Argument after -n must be a number\n");
        return EXIT_FAILURE;
    }

    Rad output = init( pocet_radku);

    if (  output.radky == NULL )
    {
        fprintf(stderr,"Can not get memory\n");
        return EXIT_FAILURE;
    }
    getlines(f, &output );


    freememory(&output);

    fclose(f);

    return 0;
}
/*** Konec souboru tail.c ***/
