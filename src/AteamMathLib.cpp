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
    // IMLEMENTATION 
  }

}

