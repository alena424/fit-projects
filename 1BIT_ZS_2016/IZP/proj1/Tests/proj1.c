/*
Autor: Alena Tesarova (xtesar36)
Skupina: BIB (39)
Datum: 13.10.2016

Nazev: proj1.c

Popis projektu:
Cilem projektu je vytvorit program, ktery bud binarni data formatuje do textove podoby, nebo textovou podobu dat prevadi do binarna­ podoby.
V pripade prevodu binarnich dat na text bude vystupna ­ format obsahovat adresy vstupnich bajtu, hexadecimalni kodovani a textovou reprezentaci obsahu.
V pripade prevodu textu do binarni podoby je na vstupu ocekavane hexadecimalni kodovani bajtu.

*/


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define DO_SOUSTAVY 16
#define NA_RADEK 16
#define MAX_DELKA_PRO_S 200

/**
*StringCompare
*Funkce pro porovnani dvou retezcu
*@param - prvni retezec
*@param - druhy retezec
*Navratova hodnota
*    0 - pokud jsou stejne
*    1 - pokud jsou rozdilne
*/
int stringCompare(char ret1[], char ret2[])
{
    int i = 0;
    while (ret1[i]!='\0'&&ret2[i]!='\0') //az jeden z retezcu je nakonci
    {
        if (ret1[i]!=ret2[i])
        {
            return 1;
        }
        i++;
    }
    if (ret1[i]=='\0'&&ret2[i]=='\0') //dosli jsme vlastne na konec nejakeho z retezcu?
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
/**
*Funkce rozebere retezec a testuje, jestli kazdy znak je cislice
*pokud neni cislice, vrati 0, jinak 1
*/
int isDigitUpgrade(char pole[])
{
    int i = 0;
    while(pole[i]!='\0')
    {
        if (pole[i]-'0'<0||pole[i]-'0'>9)
        {
            return 0;
        }
        i++;
    }
    return 1;
}

/**
*Funkce prevede retezec na cislo
*/
int toInt (char pole[])
{
    int i = 0;
    int cislo=0;
    while (pole[i]!=0)
    {
        cislo = (pole[i] -'0') + cislo*10;
        i++;
    }
    return cislo;
}

void functionForX()
{
    int znak;
    while((znak=getchar())!=EOF)
    {
        printf("%02x",znak);
    }
}


/*Napoveda, vypise se vzdy, kdyz je chyba*/
void help()
{
    printf("Mili uzivateli, vitej v napovede\n\nSyntax spusteni:\n");
    printf("./proj1 \t\tVystupni format:\n\t\t\tAAAAAAAA  xx xx xx xx xx xx xx xx  xx xx xx xx xx xx xx xx  |bbbbbbbbbbbbbbbb|\n\t\t\tAAA je adresa prvniho bajtu dane serie ve vstupnim souboru\n\t\t\txx hexadecimalni podoba daneho bajtu\n\t\t\tb je tisknutelna podoba daneho bajtu\n\n");
    printf("./proj1[-s M][-n N]\tZpresneni vystupu, kde M stanovi zacatecni pozici v retezci \n\t\t\tN definuje maximalni delku vstupnich bajtu ke zpracovani.\n");
    printf("./proj1 -x \t \tVeskera data na vstupu budou prevedena do hexadecimalni podoby na jeden radek.\n");
    printf("./proj1 -S \t \tProgram tiskne znaky, ktere v vypadaji jako textovy retezec.\n\t\t\tNecht retezec je nejdelsi posloupnost tisknutelnych a prazdnych znaku, jejichz delka je vetsi nebo rovna N znaku.\n");
    printf("./proj1 -r \t \tNa vstupu se ocekava sekvence hexadecimelnich cislic a tyto prevadi do binarniho formatu.\n\n");
}

/*Vytvori nam text za hexadecimalni podobou retezce*/
void makeText(char pole[], int velikost)
{
    for(int i = 0; i< DO_SOUSTAVY; i++)
    {
        if (i+1 > velikost) //chceme to vytisknout v peknem formatu, pokud uz jsme vytiskli vsechny bajty, tisneme mezery
        {
            printf(" ");
        }
        else
        {

            if (!isprint(pole[i]))
            {
                printf(".");
            }
            else
            {
                printf("%c",pole[i]);
            }
        }
    }
}

/**
*prevedeme cislo do hexa soustavy ve formatu AAAAAAAA
*/
void numberToHexa(int cislo)
{
    printf("%08x  ",cislo);
}

/**
*Funkce polePrint
*funkce nam vytiskne jeden radek ve formatu AAAAAAAA xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx |bbbbbbbbbbbbbbbb|
*@param pole: je uz prevedene pole z binarniho kodu do hexadecimalniho
*@param start: startovni pozice, zacinajici bajt
*@param n: velikost pole (ktera je 2x mensi nez velikost ret[])
*@param z: spinac, pokud je zaply argument -z pisu ve formatu xxxxxxxxxxxx
*@param ret: retezec znaku, napr.: Hello world\0
*/

void polePrint(int start, int velikostpole,char ret[])
{
    numberToHexa(start);
    for (int i = 0; i<NA_RADEK; i++)
    {

        if (i%(NA_RADEK/2)==0&&i!=0)
        {
            printf(" "); //mezera uprostred
        }
        if (i>=velikostpole)
        {
            printf("  ");
        }
        else
        {
            printf("%02x",ret[i]);

        }
        printf(" ");
    }
    printf(" |");
    makeText(ret,velikostpole); //retezec ma pulku znaku co pole
    printf("|");


}

/**
*Funkce transformToHexa
*hlavni funkce pro prevod z binarniho kodu do hexadecimalniho
*spousti se pro parametry -s -n -z
*@param m: od jakeho bajtu mame tisknout
*@param n: kolik bajtu mame tisknout
*@param x: nabyva hodnot 1 a 0 (pokud jsme ho zvolili v argumentech nebo ne)
*/
int transformToHexa(int m, int n)
{
    char ret[NA_RADEK+1]; //pole jen pro text + '\0'
    char znak; //nacitany znak
    int j =0; //pocitadlo, abychom se orientovali, na jakem znaku se prave nachazime
    int index_ret = 0; //cisluji pole ret
    if (n==0)
    {
        n=NA_RADEK;
    }
    while((znak=getchar())!=EOF)
    {
        if (index_ret%(NA_RADEK)==0&&index_ret!=0)//pokud je pole vetsi nez 16*2, prepisu pole
        {
            ret[index_ret]='\0';
            polePrint(m,index_ret,ret);
            printf("\n");
            m = m+NA_RADEK;
            n = n * 2;
            index_ret = 0;
        }
        if (j>=m&&j<m+n)
        {
            ret[index_ret]=znak; //ukladam si znak
            index_ret++;

        }
        j++;
    }
    if (j<=m) //chceme zacit nekde, kde nic neni?
    {
        return 1; //presahli jsme velikost pole
    }
    ret[index_ret]='\0'; //retezec skoncil
    polePrint(m,index_ret,ret);
    return 0;

}
/*v hexadecimanim kodu pouzivame pro vyjadreni cislic 10 - 15 pismena a - f a obracene*/
char toBin(char znak)
{

    if (znak >='a'&& znak <= 'f')
    {
        return znak - 'W'; //prevadim 'a' (97) na 10 (10) rozdil: W = 87
    }
    if (znak >='A'&& znak <= 'F')
    {
        return znak - '7'; //prevadim 'A' (65) na 10 (10) rozdil: '7' = 55
    }
    else
    {
        return znak;
    }
}

/*prevedeni 2 znaku znak v binarni soustave do hexadecimalni*/
void charToBin(char znak,char znak2)
{
    int pom;
    if (znak>='0'&&znak<='9')
    {
        znak=znak-'0'; //cislo
    }
    if (znak2>='0'&&znak2<='9')
    {
        znak2=znak2-'0'; //2.cislo
    }
    //prevod do desitkove soustavy
    pom = znak*DO_SOUSTAVY + znak2;
    printf("%c",pom);
}
/*testuji, jestli je znak v hexadecimalnim kodo
*/
int isHexa(char znak)
{

    if ((znak-'0'>=0&&znak-'0'<=9)||(znak>='a'&&znak<='f')||(znak=='\n')||(znak>='A'&&znak<='F'))
    {
        return 1;
    }
    return 0;
}

/**funkce reverse
*predela hexa kod do binarniho kodu
*/
int reverse()
{
    char znak;
    char znak2;
    int uloz =0;
    while (uloz==0&&(znak=getchar())!=EOF)
    {

        if(znak!=' ') //oddelavam mezery
        {
            if (!isHexa(znak))
            {
                return EXIT_FAILURE;
            }
            while ((znak2=getchar())!=EOF&&znak2!='\0')
            {
                if (znak2!=' ')
                {
                    if (!isHexa(znak2))
                    {
                        return EXIT_FAILURE;
                    }
                    charToBin(toBin(znak),toBin(znak2));
                    break;
                }
            }
            if(znak2=='\0')
            {
                uloz = 1;
                charToBin(0,toBin(znak2)); //pokud je pocet znaku lichy
            }
        }
    }
    return EXIT_SUCCESS;
}

/**
*Funkce pro parametr -S
*zkouma, jestli je aktualni znak tisknutelny
*n - minimalni pocet znaku pro retezec
*/
int makeStringForS(int n)
{
    int i = 0;
    char ret[MAX_DELKA_PRO_S];
    while ((ret[i]=getchar())!=EOF)
    {
        if(!isprint(ret[i]))
        {
            if (i >= n)
            {
                ret[i]='\0';
                printf("%s",ret);
                printf("\n");

            }
            i=0;
        }
        else
        {
            i++;
            if (i>=MAX_DELKA_PRO_S)
            {
                return EXIT_FAILURE;

            }
        }
    }
    //nebereme znak \n
    if (i >= n) //je posledni zkoumanany retezec retezec?
    {
        ret[i]='\0';
        printf("%s ",ret);
    }
    return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{

    int m = 0; //definuje, kde mam zacit
    int n = 0;  //defunuje kolik bajtu nactu
    switch (argc)
    {
    case 1:
    {
        transformToHexa(m,n);
        break;

    }
    case 2:
    {
        if(stringCompare(argv[1],"-x")==0)
        {
            functionForX();

        }
        else
        {
            if(stringCompare(argv[1],"-r")==0)
            {
                if(reverse())
                {
                    fprintf(stderr,"Retezec neni v hexadecimalni soustave !\n");
                }
            }
            else
            {
                help();
                return 0;
            }
        }
        break;
    }
    case 3:
    {

        if(stringCompare(argv[1],"-s")==0)
        {

            if (!isDigitUpgrade(argv[2]))
            {
                fprintf(stderr,"Zadany argument je ve spatnem formatu\n\n");
                help();
                return 0;
            }
            m = toInt(argv[2]);
            if (transformToHexa(m,n))
            {

                return 0;

            }
        }
        else
        {
            if(stringCompare(argv[1],"-n")==0)
            {
                if (!isDigitUpgrade(argv[2]))
                {
                    fprintf(stderr,"Zadany argument je ve spatnem formatu\n\n");
                    help();
                    return 0;
                }
                n = toInt(argv[2]);
                transformToHexa(m,n);

            }
            else
            {
                if(stringCompare(argv[1],"-S")==0)
                {
                    if (!isDigitUpgrade(argv[2]))
                    {
                        fprintf(stderr,"Zadany argument je ve spatnem formatu\n\n");
                        help();
                        return 0;
                    }
                    n = toInt(argv[2]);
                    if (makeStringForS(n))
                    {
                        fprintf(stderr,"Prilis mnoho znaku, omezeni je 200\n\n");
                    }
                    printf("\n");
                }
                else
                {
                    help();
                    return 0;
                }
            }
        }
        break;
    }
    case 5:
    {
        if(stringCompare(argv[1],"-s")==0)
        {
            if (!isDigitUpgrade(argv[2]))
            {
                fprintf(stderr,"Zadany argument je ve spatnem formatu\n\n");
                help();
                return 0;
            }
            m = toInt(argv[2]);

            if(stringCompare(argv[3],"-n")==0)
            {
                if (!isDigitUpgrade(argv[4]))
                {
                    fprintf(stderr,"Zadany argument je ve spatnem formatu\n\n");
                    help();
                    return 0;
                }
                n = toInt(argv[4]);
            }
            if (transformToHexa(m,n))
            {
                //fprintf(stderr,"Hodnota u argumentu -s prevysuje pocet prvku\n\n"); nic program nema vypsat, je to v zadani
                return 0;

            }
        }
        if(stringCompare(argv[1],"-n")==0)
        {

            if (!isDigitUpgrade(argv[2]))
            {
                fprintf(stderr,"Zadany argument je ve spatnem formatu\n\n");
                help();
                return 0;
            }
            n = toInt(argv[2]);
            if(stringCompare(argv[3],"-s")==0)
            {

                if (!isDigitUpgrade(argv[4]))
                {
                    fprintf(stderr,"Zadany argument je ve spatnem formatu\n\n");
                    help();
                    return 0;
                }
                m = toInt(argv[4]);
                if (transformToHexa(m,n))
                {

                    return 0;
                }
            }
        }
        if ((stringCompare(argv[1],"-n")&&stringCompare(argv[1],"-s")))//pokud neplati ani jedna podminka
        {
            help();
            return 0;
        }
        break;
    }
    default:
        help();
        return 0;
        break;
    }
    return 0;
}
