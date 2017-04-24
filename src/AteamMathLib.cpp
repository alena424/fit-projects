/**
 * @file AteamMathLib.cpp
 *
 * @brief Definitions of functions in math library.
 *
 * File containing definitions of functions, their 
 * implementation etc.
 *
 * @author Daniel Uhricek (xuhric00)
 * @author Peter Uhrin (xuhrin02)
 * @author Alena Tesarova (xtesar36)
 * @author Jan Sorm (xsorjm00)
 */


#include "AteamMathLib.h"
#include <stdexcept>

namespace MathLib
{

  unsigned long long int AteamMathLib::factorial (short int x) {
  unsigned long long int result = 1;

	// check the range of the input number
	if ( x < 0 or x > 20 ) {
    throw std::out_of_range( "The number is not an integer in <0, 20>." );
	}

	// own calculation
	for ( short int i = x; i > 0; i-- ) {
		result *= i;
	}
	
	return result;
  }

  double AteamMathLib::power (double x, int n) {
    double result = 1;
	
	  // check the range of the exponent n
	  if ( n < 0 ) {
      throw std::out_of_range( "The exponent is a negative number." );
    }

    // own calculation
    for ( int i = 1; i <= n; i++ ) {
      result *= x;
    }

    return result;
  }

  double AteamMathLib::nroot (double x, int n) {
    double result = 1;
    double lastNumber = 1;
    double epsilon = 0.00000000001;
    int i = 1;
	
    // check input values and special outputs
    if ( n <= 0 ) {
                  throw std::out_of_range( "The second operator is a negative number." );
    }
    else if ( x == 0 ) {
      return 0;
    }
    else if ( n == 1 ) {
      return x;
    }
    else if ( x < 0 and n % 2 == 0 ) {
                  throw new std::out_of_range( "There is a negative number below the even root." );
    }

    // own calculation
    while ( lastNumber > epsilon or lastNumber < -epsilon )
    {
      lastNumber = (x / power(result, n - 1) - result ) / n;
      result += lastNumber;
      i++;
    }
    
    return result;
  }

  
  double AteamMathLib::addition (double x, double y) {
    return x + y;
  }

  double AteamMathLib::subtraction (double x, double y) {
  	return x - y;
  }

  double AteamMathLib::multiplication (double x, double y) {
    return x * y;
  }

  double AteamMathLib::division (double x, double y) {
    if ( y == 0 ) {
            throw std::out_of_range( "You divide by zero." );
    }
    return x / y;
  }

  double AteamMathLib::logarithm ( double x ) {
    double result, lastNumber;
    double epsilon = 0.00000000001;

    // check value of x
    if ( x <= 0 ) {
      throw std::out_of_range( "The number is not a positive number." );
    }

    // own calculation
    int i = 2;
    if ( x >= 1 ) {
      lastNumber = ( x - 1 ) / x;
      result = lastNumber;
      while ( lastNumber > epsilon or lastNumber < -epsilon )
      {
        lastNumber *=  ( x - 1 ) * ( i - 1 ) / ( x * i );
        result += lastNumber;
        i++;
      }
    } else {
      x = 1 - x;
      lastNumber = -x;
      result = lastNumber;
      while ( lastNumber > epsilon or lastNumber < -epsilon ) {
        lastNumber *= ( i - 1 ) * x / i;
        result += lastNumber;
        i++;
      }
    }
    
    return result;
  }

}

