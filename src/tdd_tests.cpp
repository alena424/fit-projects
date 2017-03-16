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

TEST(Factorial, TestCase1) 
{
  EXPECT_EQ(1, 1);
}

TEST(Factorial, TestCase2)
{
  EXPECT_EQ(1, 1);
}

TEST(Power, TestCase1)
{
  EXPECT_EQ(1, 1);
}

TEST(Power, TestCase2)
{
  EXPECT_EQ(1, 1);
}

TEST(Nroot, TestCase1)
{
  EXPECT_EQ(1, 1);
}

TEST(Nroot, TestCase2)
{
  EXPECT_EQ(1, 1);
}

int main (int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
