#include <gtest/gtest.h>
#include "calculator.h"

TEST(test_calc_correct, test1) //Correct tests
{
    try
    {
        Calculator calc;
        ASSERT_EQ(calc.Evaluate("123 + 42.1"), 165.1);

    }
    catch (...)
    {
        FAIL();
    }
    
}

TEST(test_calc_correct, test2)
{
    try
    {
        Calculator calc;
        ASSERT_EQ(calc.Evaluate("-(21 * 0.3)"), -6.3);

    }
    catch (...)
    {
        FAIL();
    }
    
}

TEST(test_calc_correct, test3)
{
    try
    {
        Calculator calc;
        ASSERT_EQ(calc.Evaluate("1 + (2 * (2.5 + 2.5 + (3 - 2))) - (3/1.5)"), 11);

    }
    catch (...)
    {
        FAIL();
    }
    
}


TEST(test_calc_correct, test4)
{
    try
    {
        Calculator calc;
        ASSERT_EQ(calc.Evaluate("-10 + (8 *  2.5) - (3/1.5)"), 8);

    }
    catch (...)
    {
        FAIL();
    }
    
}

TEST(test_calc_exception, test_exc1) //Tests causing exception raise
{
    try
    {
        Calculator calc;
        float res = calc.Evaluate("123 7 + 42.1");
        FAIL();
    }
    catch (const std::exception& e)
    {
        ASSERT_EQ(std::string(e.what()), "Expression has whitespaces between numbers");
    }
}

TEST(test_calc_exception, test_exc2)
{
    try
    {
        Calculator calc;
        float res = calc.Evaluate("123 + abc");
        FAIL();
    }
    catch (const std::exception& e)
    {
        ASSERT_EQ(std::string(e.what()), "Unsupported expression symbol a");
    }
}

TEST(test_calc_exception, test_exc3)
{
    try
    {
        Calculator calc;
        float res = calc.Evaluate("");
        FAIL();
    }
    catch (const std::exception& e)
    {
        ASSERT_EQ(std::string(e.what()), "Empty expression");
    }
}

TEST(test_calc_exception, test_exc4)
{
    try
    {
        Calculator calc;
        float res = calc.Evaluate("123 + ");
        FAIL();
    }
    catch (const std::exception& e)
    {
        ASSERT_EQ(std::string(e.what()), "Not enough operands for operation +");
    }
}

TEST(test_calc_exception, test_exc5)
{
    try
    {
        Calculator calc;
        float res = calc.Evaluate("(123 + 5))");
        FAIL();
    }
    catch (const std::exception& e)
    {
        ASSERT_EQ(std::string(e.what()), "Invalid brackets balance");
    }
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

