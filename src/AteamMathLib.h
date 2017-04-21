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
    static unsigned long long int factorial (short int x);


    /**
     * @brief Power of n
     *
     * Function for calculating nth power of number x.
     * 
     * @param x Double precision number
     * @param n Natural number
     * @return x^n
     */
    static double power (double x, int n);


    /**
     * @brief Nth root
     *
     * Function for calculating nth root of number x.
     *
     * @param x Double precision number
     * @param n Natural number
     * @return Nth root of x
     */
    static double nroot (double x, int n);


    /**
     * @brief  Addition
     *
     * Function for adding numbers
     *
     * @param x Double precision number
     * @param y Double presicion number
     * @return Sum of two numbers
     */
     static double addition (double x, double y);


    /**
     * @brief Subtraction
     *
     * Function for subtracting numbers
     *
     * @param x Double precision number
     * @param y Double precision number
     * @return Difference of two numbers
     */
     static double subtraction (double x, double y);


    /**
     * @brief Multiplication
     *
     * Function for multiplicating numbers
     *
     * @param x Double precision number
     * @param y Double presicion number
     * @return Product of two numbers
     */
     static double multiplication (double x, double y);

    
    /**
     * @brief Division
     *
     * Function for dividing numbers
     *
     * @param x Double precision number
     * @param y Double precision number except 0
     * @return Quotient of two numbers
     */
     static double division (double x, double y);

    
    /**
     * @brief The logarithm of x to base e
     *
     * Function for logarithming numbers
     *
     * @param x Double precision positive number
     * @return The logarithm of positive real number x with respect to base e
     */
     static double logarithm (double x);

    // TODO
    // ONE MORE FUNCTION
    
  };
}
