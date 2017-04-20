/**
 * @file deviation.cpp
 *
 * @brief Standart deviation using functions of math library
 *
 * @author Daniel Uhricek (xuhric00)
 * @author Peter Uhrin (xuhrin02)
 * @author Alena Tesarova (xtesar36)
 * @author Jan Sorm (xsorjm00)
 */


#include "AteamMathLib.h"
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <vector>
#include <deque>

using namespace std;

/**
 * @brief Standard deviation
 * @param file File to read numbers from
*/

/**
 * Poznamky k dokumentaci:

- cim je vetsi, tim jsou cisla mene podobna, cim je mensi, tim jsou cisla podobna vice
- znaci se pismenem sigma
- rozlisujeme vyberovou smerodatnou odchylku a smerodatnou odchylku
	* vyberova - por skutecny odhad odchylky na empiricky zjistene rade cisel
- vychazi z vypoctu aritmetickeho prumeru
vzorec1:
s = srt(  1 / N-1 * SUMA (od 1 do N) (x - prumer)^2  )

vzorec2:
*/

double deviation( char *soubor )
{
    ifstream file;
    istream *in;
    // open file for reading, ios:in
    file.open( soubor, ifstream::in );
    if ( !file.is_open() )
    {
        throw new std::runtime_error("Can not open file");
        return 0;
    }
    // read double from file
    double c;
    // to save values from array
    deque<double>nums ;

    while( file >> c )
    {
        nums.push_back(c);
    }
    file.close();

    /*verify that the nums were stored correctly:
    for (int i = 0; i < nums.size(); ++i) {
        cout << nums[i] << std::endl;
    	}*/

    long double sum = 0;
    long double arrayn[] = {7,8,9,10};
    //vector<long double> nums ( arrayn, arrayn + sizeof(arrayn) / sizeof(long double) );
    for ( int i = 0; i < nums.size(); i++ )
    {
        //aritmeticky prumer
        sum = MathLib::AteamMathLib::addition(sum, nums[i] );
    }
    double average = sum/nums.size();

    //suma podle vzorce
    double sum_nums = 0;
    for ( int j = 0; j < nums.size(); j++ )
    {
        // sumnums += ( x - average)^2
        sum_nums =  MathLib::AteamMathLib::addition( ( MathLib::AteamMathLib::power( MathLib::AteamMathLib::subtraction( nums[j], average ), 2) ), sum_nums);
    }
    // total = 1 / N - 1 * sumnums
    double total = MathLib::AteamMathLib::multiplication( sum_nums, MathLib::AteamMathLib::division( 1, MathLib::AteamMathLib::subtraction( nums.size(), 1 ) ));

    return MathLib::AteamMathLib::nroot(total, 2);
}

int main()
{

    char * soubor = "numsmale.txt";
    cout << "Odchylka je:" << deviation(soubor);
    //cout << "5 + 6 =" << MathLib::AteamMathLib::addition(5.0,6.0);
    return 0;
}


