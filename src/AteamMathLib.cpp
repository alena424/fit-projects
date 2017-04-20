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
	if ( x < 0 or x > 21 ) {
		throw new std::runtime_error("Out of range.");
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
		throw new std::runtime_error("Out of range.");
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
	int i = 1;
	
	// check input values and special outputs
	if ( n <= 0 ) {
		throw new std::runtime_error("Invalid value n.");
	}
	else if ( x == 0 ) {
		return 0;
	}
	else if ( n == 1 ) {
		return x;
	}
	else if ( x < 0 ) {
		throw new std::runtime_error("Invalid value x.");	
	}

	// own calculation
	while ( lastNumber > 0.0000001 or lastNumber < -0.0000001 )
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
		throw new std::runtime_error("Invalid value");
	}
	return x / y;
  }

}

