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
#include "googletest/googletest/include/gtest/gtest.h"

using namespace MathLib;

double eps = 0.00000000001;

double Absolute(double x) {
  return (x < 0.0) ? -x : x;
}

/* Factorial tests */

TEST(Factorial, TestCase1) 
{
  unsigned long long int fac = AteamMathLib::factorial(0);
  EXPECT_EQ(fac, 1);
}

TEST(Factorial, TestCase2)
{
  unsigned long long int fac;
  fac = AteamMathLib::factorial(1);
  EXPECT_EQ(fac, 1);
  fac = AteamMathLib::factorial(2);
  EXPECT_EQ(fac, 2);
  fac = AteamMathLib::factorial(3);
  EXPECT_EQ(fac, 6);
}

TEST(Factorial, TestCase3)
{
  EXPECT_ANY_THROW(AteamMathLib::factorial(-1));
}

TEST(Factorial, TestCase4)
{
  EXPECT_ANY_THROW(AteamMathLib::factorial(22));
  EXPECT_ANY_THROW(AteamMathLib::factorial(42));
}

TEST(Factorial, TestCase5)
{
  unsigned long long int fac;
  fac = AteamMathLib::factorial(20);
  EXPECT_EQ(fac, 2432902008176640000);
  fac = AteamMathLib::factorial(15);
  EXPECT_EQ(fac, 1307674368000);
  fac = AteamMathLib::factorial(10);
  EXPECT_EQ(fac, 3628800);
}

TEST(Factorial, TestCase6)
{
  EXPECT_ANY_THROW(AteamMathLib::factorial(-1));
  EXPECT_ANY_THROW(AteamMathLib::factorial(-10));
  EXPECT_ANY_THROW(AteamMathLib::factorial(-10000));
}

/* Power tests */

TEST(Power, TestCase1)
{
  double pow;
  pow = AteamMathLib::power(0, 1);
  EXPECT_TRUE(Absolute(pow - 0) < eps);
  pow = AteamMathLib::power(0, 5);
  EXPECT_TRUE(Absolute(pow - 0)  < eps);
  pow = AteamMathLib::power(0, 10000);
  EXPECT_TRUE(Absolute(pow - 0)  < eps);
}

TEST(Power, TestCase2)
{
  double pow;
  pow = AteamMathLib::power(1, 0);
  EXPECT_TRUE(Absolute(pow - 1) < eps);
  pow = AteamMathLib::power(1, 2);
  EXPECT_TRUE(Absolute(pow - 1) < eps);
  pow = AteamMathLib::power(1, 42);
  EXPECT_TRUE(Absolute(pow - 1) < eps);
}

TEST(Power, TestCase3)
{
  EXPECT_ANY_THROW(AteamMathLib::power(1, -1));
  EXPECT_ANY_THROW(AteamMathLib::power(1, -4));
}

TEST(Power, TestCase4)
{
  double pow;
  EXPECT_NO_THROW(pow = AteamMathLib::power(-1, 1));
  EXPECT_TRUE(Absolute(pow - (-1)) < eps);
  EXPECT_NO_THROW(pow = AteamMathLib::power(-1, 2));
  EXPECT_TRUE(Absolute(pow - 1) < eps);
  EXPECT_NO_THROW(pow = AteamMathLib::power(-2, 2));
  EXPECT_TRUE(Absolute(pow - 4) < eps);
  EXPECT_NO_THROW(pow = AteamMathLib::power(-2, 3));
  EXPECT_TRUE(Absolute(pow - (-8)) < eps);
}

TEST(Power, TestCase5)
{
  double pow;
  pow = AteamMathLib::power(0.252525, 2);
  EXPECT_TRUE(Absolute(pow - 0.06376887562) < eps);
}

TEST(Power, TestCase6)
{
  double pow;
  pow = AteamMathLib::power(20, 7);
  EXPECT_TRUE(Absolute(pow - 1280000000) < eps);
  pow = AteamMathLib::power(20, 9);
  EXPECT_TRUE(Absolute(pow - 512000000000) < eps);
}

/* Natural log tests - this function was already well tested in another project
 * Not part of TDD */

TEST(Log, TestCase1)
{
  EXPECT_ANY_THROW(AteamMathLib::logarithm(0));
}

TEST(Log, TestCase2)
{
  EXPECT_ANY_THROW(AteamMathLib::logarithm(-1));
}

TEST(Log, TestCase3)
{
  double log;
  log = AteamMathLib::logarithm(1);
  EXPECT_TRUE(Absolute(log - 0) < eps);
}

TEST(Log, TestCase4)
{
  double log;
  log = AteamMathLib::logarithm(2);
  EXPECT_TRUE(Absolute(log - 0.69314718056) < eps);
}

/* Nroot tests */

TEST(Nroot, TestCase1)
{
  double root;
  root = AteamMathLib::nroot(4, 2);
  EXPECT_TRUE(Absolute(root - 2) < eps);
  root = AteamMathLib::nroot(10000, 2);
  EXPECT_TRUE(Absolute(root - 100) < eps);
}

TEST(Nroot, TestCase2)
{
  double root;
  root = AteamMathLib::nroot(0, 2);
  EXPECT_TRUE(Absolute(root - 0) < eps);
  root = AteamMathLib::nroot(0, 1000);
  EXPECT_TRUE(Absolute(root - 0) < eps);
  root = AteamMathLib::nroot(0, 2424242);
  EXPECT_TRUE(Absolute(root - 0) < eps);
}

TEST(Nroot, TestCase3)
{
  double root;
  root = AteamMathLib::nroot(-42, 1);
  EXPECT_TRUE(Absolute(root - (-42)) < eps);
  root = AteamMathLib::nroot(99.424242, 1);
  EXPECT_TRUE(Absolute(root - 99.424242) < eps);
}

TEST(Nroot, TestCase4)
{
  EXPECT_ANY_THROW(AteamMathLib::nroot(20, -1));
  EXPECT_ANY_THROW(AteamMathLib::nroot(1241231.213123, -3));
}

TEST(Nroot, TestCase5)
{
  EXPECT_ANY_THROW(AteamMathLib::nroot(-124123, 2));
}

TEST(Nroot, TestCase6)
{
  EXPECT_ANY_THROW(AteamMathLib::nroot(0, -1));
}

TEST(Nroot, TestCase7)
{
  EXPECT_ANY_THROW(AteamMathLib::nroot(-1, 4938282));
}

int main (int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
