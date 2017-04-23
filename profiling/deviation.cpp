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
 * @brief Standard deviation using this pattern dev = srt(  1 / N-1 * SUM (from 1 to N) (x - mean)^2  )
 * @param file nums Array with numbers of double
 * @param num_size size of array
 * @pre more numbers in file than 1
*/
double deviation( double nums[], int nums_size )
{
    long double sum = 0;

    for ( int i = 0; i < nums_size; i++ )
    {
        //aritmeticky prumer
        sum = MathLib::AteamMathLib::addition(sum, nums[i] );
    }
    double average = MathLib::AteamMathLib::division(sum,nums_size);

    //suma podle vzorce
    double sum_nums = 0;

    for ( int j = 0; j < nums_size; j++ )
    {
        // sumnums += ( x - average)^2
        sum_nums =  MathLib::AteamMathLib::addition( ( MathLib::AteamMathLib::power( MathLib::AteamMathLib::subtraction( nums[j], average ), 2) ), sum_nums);
    }
    // total = 1 / N - 1 * sumnums
    double total = MathLib::AteamMathLib::multiplication( sum_nums, MathLib::AteamMathLib::division( 1, MathLib::AteamMathLib::subtraction( nums_size, 1 ) ));

    return MathLib::AteamMathLib::nroot(total, 2);
}


/**
 * @brief main function - to read numbers from file
*/
int main( int argc, char *argv[])
{
    ifstream file;


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
    double num;
    double array[100000]; //a static array to save numbers
    int j = 0; //for indexing
    while ( file >> num ){
        array[j] = num;
        //printf("%f ", array[ j ] );
        j++;
    }
	
    // open file for reading, ios:in

    // read double from file
    /*double c;
    // to save values from array
    deque<double>nums ;

    while(  file >> c )
    {
        nums.push_back(c);
    }
    file.close();
    int nums_size = nums.size();
    */
    double dev = deviation( array, j);
    cout << dev << endl;
    return 0;
}


