/*
Autor: Alena Tesarova (xtesar36)
Skupina: BIB (39)
Datum: 15.11.2016

Nazev: proj2.c

Popis projektu:
Implementujte vypocet prirozeneho logaritmu a exponencialni funkce s obecnym zakladem pouze pomoci matematickych operaci +,-,*,/.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MOCNINA 2.0 //budu potrebovat mocninu 2- 1,2,4,8,16...
#define LOG2 0.69314718055995	 //log 2
#define START_INT 1 //start intervalu
#define KONEC_INT 2

void help()
{
    printf("Argumenty programu:\n");
    printf("\t--log X N pozadavek pro vypocet prirozeného logaritmu z cisla X v N iteracich (Taylorova polynomu a zretezeneho zlomku)\n");
    printf("\t--pow X Y N pozadavek pro vypocet exponencialní funkce z císla Y s obecnym zakladem X v N iteracich (Taylorova polynomu a zretezeného zlomku)\n");

}
/**
*Funkce overi, jestli je cislo X ve spravnem formatu, tedy obsahuje pouze cislice a maximalne jednu tecku
*poc = pocita tecky, pokud jejich vice jak jedna vraci false
*i = indexuje pole
*/
int isRightDouble(char pole[])
{
    int i = 0;
    int poc = 0;
    while(pole[i]!='\0')
    {
        if (pole[i]=='.')
        {
            poc++;
        }
        else
        {
            if ((pole[i]<'0'||pole[i]>'9')&&(pole[0]!='+'&&pole[0]!='-'))
            {
                return 0;
            }
        }
        i++;
    }
    return (poc<=1);
}
/**
*N musi byt kladne cislo
*/
int isRightN(int n)
{
    return (n>=0);
}

double taylor_log(double x, unsigned int n)
{
    double taylor_suma = 0;
    double polynom = 1;
    double pom_x = 1 - x;
    if (x == 0)
    {
        return -INFINITY;
    }
    if (x<0)
    {
        return NAN;
    }
    if (isinf(x))
    {
        return INFINITY;
    }
    if (x>1)
    {
        for(unsigned int i = 1; i<=n; i++)
        {
            polynom = (polynom * (x-1)/x);
            taylor_suma = taylor_suma + polynom/i;
        }
    }
    else
    {
        for(unsigned int i = 1; i<=n; i++)
        {
            polynom = polynom * pom_x ;
            taylor_suma = taylor_suma - polynom/i;
        }
    }
    return taylor_suma;
}
double fb(unsigned int n)
{
    return (n + (n+1));
}
double fa(unsigned int n)
{
    return (n*n);
}

double cfrac_log(double x, unsigned int n)
{
    double delitel = 0;
    double z = (x-1)/(1+x);
    double z2 = z*z;
    if (x == 0)
    {
        return -INFINITY;
    }
    if (x<0)
    {
        return NAN;
    }
    if (isinf(x))
    {
        return INFINITY;
    }
    while (n >= 1)
    {
        delitel = fa(n)*z2 / ( fb(n) - delitel );
        n--;
    }
    return 2*z/(1 - delitel);
}

double taylor_pow(double x, double y, unsigned int n)
{
    double citatel = 1; //budeme nasobit, proto 1
    double suma = 1;
    double pom_suma = 1;
    if (x == 0)
    {
        if (y==0)
        {
            return 1;
        }
        if (y>0)
        {
            return 0;
        }
        else
        {
            return INFINITY;
        }
    }
    if (isinf(x))
    {
        if (y==0)
        {
            return 1;
        }
        else
        {
            if (y<0)
            {
                return 0;
            }
            else
            {
                if (isnan(y))
                {
                    return NAN;
                }
                else
                {
                    return INFINITY;
                }
            }
        }
    }
    if (y == -INFINITY){
        return 0;
    }
    citatel = y*taylor_log(x,n);
    for(unsigned int i = 1; i<=n; i++)
    {
        pom_suma = citatel/i *pom_suma;
        suma = suma + pom_suma;
    }

    return (suma);
}

double taylorcf_pow(double x, double y, unsigned int n)
{
    double pom_suma = 1;
    double citatel = 1;
    double suma = 1;
    if (x == 0)
    {
        if (y==0)
        {
            return 1;
        }
        if (y>0)
        {
            return 0;
        }
        else
        {
            return INFINITY;
        }
    }
    if (isinf(x))
    {
        if (y==0)
        {
            return 1;
        }
        else
        {
            if (y<0)
            {
                return 0;
            }
            else
            {
                if (isnan(y))
                {
                    return NAN;
                }
                else
                {
                    return INFINITY;
                }
            }
        }
    }
     if (y == -INFINITY){
        return 0;
    }
    citatel = y*cfrac_log(x,n);
    for(unsigned int i = 1; i<=n; i++)
    {
        pom_suma = citatel/i *pom_suma;
        suma = suma + pom_suma;

    }
    return suma;
}

double mylog(double x)
{
    double taylor_suma = 0;
    double polynom = 1;
    double e = 1e-9;
    double pom_x = 1 - x;
    int i = 1;
    if (x == 0)
    {
        return -INFINITY;
    }
    if (x<0)
    {
        return NAN;
    }
    if (x>1)
    {
        do
        {
            polynom = (polynom * (x-1)/x);
            taylor_suma = taylor_suma + polynom/i;
            i++;
        }
        while(fabs(polynom)>e);
    }
    else
    {
        do
        {
            polynom = polynom * pom_x ;
            taylor_suma = taylor_suma - polynom/i;
            //printf("%.12f, %.12f\n",polynom,taylor_suma);
            i++;
        }
        while(fabs(polynom)>e);
    }
    printf("pocet iteraci je: %d\n",i-1);
    return taylor_suma;
}

double mylog2(double x)
{
    double delitel = 0;
    double delitel2 = 0;
    unsigned n = 0;
    double e = 1e-9;
    double z = (x-1)/(1+x);
    double z2 = z*z;
    if (x == 0)
    {
        return -INFINITY;
    }
    if (x<0)
    {
        return NAN;
    }
    do
    {
        n++;
        delitel2 = delitel;
        delitel = 0;
        for (unsigned int i=n; i>=1; i--)
        {
            delitel = fa(i)*z2 / ( fb(i) - delitel );
        }
        delitel= 2*z/(1 - delitel);
        //printf("%.12g\n",delitel);
    }
    while (fabs(delitel2-delitel)>e);
    printf("pocet iteraci je %i\n",n); //delam vzdy o iteraci navic
    return delitel;
}
/** Heuristicky reseny prirozeny logaritmus x
*vychazim z vzorce: log (a*b) = log a + log b
*rozlozim na log (2 na N * x) = n * log 2 + log x
*x je v intervalu <1,2> a pocitam log x pomoci taylorova polynomu

*/
double mylog3(double x)
{
    double taylor_suma = 0;
    double e = 1e-9; //presnost
    double polynom = 1;
    double puvodni_x = x; //chci si ho pamatovat
    int i = 1; //pocet iteraci
    int n = 0; //2 na n
    if (x == 0)
    {
        return -INFINITY;
    }
    if (x<0)
    {
        return NAN;
    }
    if (x>=2)
    {
        //aby kazde cislo bylo v intervalu <1,2>
        while(x>KONEC_INT)
        {
            n++;
            x = x/MOCNINA;
        }
    }
    else
    {
        while(x<START_INT)
        {
            n++;
            x = x*MOCNINA;
        }
    }
    do
    {
        polynom = (polynom * (x-1)/x);
        taylor_suma = taylor_suma + polynom/i;
        i++;
    }
    while(fabs(polynom)>e);
    printf("pocet iteraci je: %d\n",i-1); //nakonci zvetsuju o 1
    return (puvodni_x>=2) ? taylor_suma + LOG2 * n : taylor_suma - (LOG2 * n); //pokud povodni x < 2 budeme mit 2 na -n proto odecitame -n*log
}
double mypow(double x, double y)
{
    double pom_suma = 1;
    double citatel = 1;
    double e = 1e-9;
    double suma1 = 1;
    double suma = 0;

    if (x == 0)
    {
        if (y==0)
        {
            return 1;
        }
        if (y>0)
        {
            return 0;
        }
        else
        {
            return INFINITY;
        }
    }
    citatel = y*mylog3(x);
    for(unsigned int i = 1; fabs(suma-suma1)>e; i++)
    {
        suma1 = suma;
        pom_suma = citatel/i *pom_suma;
        suma = suma1 + pom_suma;
    }
    return 1 + suma;

}
int main(int argc, char *argv[])
{
    double x,y;
    unsigned int n;
    char *zbytky;
    char *zbytky1;
    switch(argc)
    {
    case 4:
    {
        if (strcmp(argv[1],"--log")==0)
        {


            x = strtod(argv[2],&zbytky);
            n = atoi(argv[3]);
            if (strcmp(zbytky,"") == 0&&isRightN(atoi(argv[3])))
            {

                //printf(" my_taylor(%g) = %.7e\n",x,mylog(x));
                //printf("  my_cfrac(%g) = %.7e\n",x,mylog2(x));
                //printf(" my_taylor(%g) = %.7e\n\n",x,mylog3(x));
                printf("       log(%g) = %.12g\n",x,log(x));
                printf(" cfrac_log(%g) = %.12g\n",x,cfrac_log(x,n));
                printf("taylor_log(%g) = %.12g\n",x,taylor_log(x,n));

            }
            else
            {
                fprintf(stderr,"Cislo je ve spatnem formatu\n");
            }
        }
        break;
    }
    case 5:
    {
        if (strcmp(argv[1],"--pow")==0)
        {
            x = strtod(argv[2],&zbytky);
            y = strtod(argv[3],&zbytky1);
            n = atoi(argv[4]);
            if (strcmp(zbytky,"") == 0&&strcmp(zbytky1,"") == 0&&isRightN(atoi(argv[4])))
            {
                //printf("       mypow(%g,%g) = %.7e\n",x,y,mypow(x,y));
                printf("         pow(%g,%g) = %.12g\n",x,y,pow(x,y));
                printf("  taylor_pow(%g,%g) = %.12g\n",x,y,taylor_pow(x,y,n));
                printf("taylorcf_pow(%g,%g) = %.12g\n",x,y,taylorcf_pow(x,y,n));
            }
            else
            {
                fprintf(stderr,"Cislo je ve spatnem formatu\n");
            }
        }
        break;
    }
    default:
    {
        help();
        break;
    }
    }
    return 0;
}
