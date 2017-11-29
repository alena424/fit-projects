/**
 * @file tail2.cc
 * @brief IJC-DU2, priklad a)
 *
 * @date 11.4.2017
 * @author Alena Tesarova, FIT, xtesar36
 *
 */

#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <deque>

using namespace std;

#define RADEK_MAX 1024
#define POCET_R 10

/**
* @brief Funkce nacita radky do pole, kde pole neni ohranicene
* @param in Vstup pro cteni
* @param radky Pocet radku pro tisk vystupu, defaultne 10
*/
void getlines( std::istream *in, unsigned int radky )
{
    deque<string> pole;
    string line;
    while (getline(*in, line))
    {
        pole.push_back(line);
        if (pole.size() > radky)
        {
            pole.pop_front();
        }
    }
    for ( auto zaznam : pole)
    {
        cout << zaznam << endl;
    }

}
/**
* @brief Hlavni funkce por nacitani a overovani argumentu
*/

int main( int argc, char *argv[] )
{

    ios::sync_with_stdio(false);

    char* soubor = NULL;
    char* endptr;
    //string soubor;
    unsigned int pocet_radku = 0;
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
            cerr << "Argument -n needed" << endl;
            return EXIT_FAILURE;
        }

        pocet_radku = strtol(argv[2], &endptr, 10);
        if ( *endptr != 0 )
        {
            cerr << "Argument after -n must be a number \"" << soubor << "\"!" << endl;
            return EXIT_FAILURE;
        }

        break;
    case 4:
        if ( strcmp( argv[1], "-n" ) != 0 )
        {
            cerr << "Argument -n needed\n" << endl;
            return EXIT_FAILURE;
        }

        pocet_radku = strtol(argv[2], &endptr, 10);
        if ( *endptr != 0 )
        {
            cerr << "Argument after -n must be a number \"" << soubor << "\"!" << endl;
            return EXIT_FAILURE;
        }
        soubor = argv[3];
        break;
    default:
        cerr << "Bad format of arguments" << endl;
        return EXIT_FAILURE;
    }

    istream *in;
    ifstream f;


    if ( soubor )
    {
        f.open(soubor, ifstream::in);
        if ( !f.is_open() )
        {
            cerr << "Can not find file  \"" << soubor << "\"!" << endl;
            return EXIT_FAILURE;
        }
        in = &f;
    }
    else
    {
        in = &cin;
    }
    getlines( in, pocet_radku );

    f.close();

    return 0;
}

/*** Konec souboru tail2.cc ***/
