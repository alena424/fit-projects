/**
 * @file AteamMathLib.h
 *
 * @brief Declarations of functions in math library.
 *
 * Header file of math library for project CalculaTron.
 * Contains the prototypes for the mathematics functions,
 * constants, global variables and macros.
 *
 * @author Daniel Uhricek (xuhric00)
 * @author Peter Uhrin (xuhrin02)
 * @author Alena Tesarova (xtesar36)
 * @author Jan Sorm (xsorjm00)
 */


namespace MathLib
{
  class AteamMathLib 
  {
  
  public:

    /**
      * @brief Factorial
      *
      * Recursive function returning factorial
      * of x for x in <0, 21>
      *
      * @param x Integer in <0, 21>
      * @return Integer of maximum value 5.1090942e+19
      */
    unsigned long long int factorial (unsigned short int x);


    /**
     * @brief Power of n
     *
     * Function for calculating nth power of number x.
     * 
     * @param x Double precision number
     * @param n Natural number
     * @return x^n
     */
    double power (double x, unsigned int n);


    /**
     * @brief Nth root
     *
     * Function for calculating nth root of number x.
     *
     * @param x Double precision number
     * @param n Natural number
     * @return Nth root of x
     */
    double nroot (double x, double n);


    // TODO
    // ONE MORE FUNCTION
    
  };
}
