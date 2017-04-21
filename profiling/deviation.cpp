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


#include "../src/AteamMathLib.h"
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <vector>
#include <deque>

using namespace std;

/**
 * @brief Standard deviation
 * @param file File to read numbers from
 * @pre more numbers in file than 1
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

double deviation( deque<double>nums )
{
    long double sum = 0;
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


double getNumsFromFile( char *soubor )
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

    while(  file >> c )
    {
        nums.push_back(c);
    }
    file.close();
    double dev = deviation(nums);
    return dev;

    /*verify that the nums were stored correctly:
    for (int i = 0; i < nums.size(); ++i) {
        cout << nums[i] << std::endl;
    	}*/


}



int main( int argc, char *argv[])
{
    ifstream file;
    istream *in;

    if ( argc != 2 )
    {
        cerr << "Expect one argument: name of a file to read numbers from" << endl ;
        return 0;
    }
    else
    {
        char *soubor = argv[1];
        file.open( soubor, ifstream::in );
        if ( !file.is_open() )
        {
            throw new std::runtime_error("Can not open file");
            return 0;
        }
    }


    // open file for reading, ios:in

    // read double from file
    double c;
    // to save values from array
    deque<double>nums ;

    while(  file >> c )
    {
        nums.push_back(c);
    }
    file.close();
    double dev = deviation(nums);
    cout << dev << endl;
    return 0;
}


