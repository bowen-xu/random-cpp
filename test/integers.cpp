#include <gtest/gtest.h>

#include <randomcpp.hpp>

class RandomIntegersTest : public ::testing::Test
{
protected:
   // This function runs once for the entire test suite before any tests are run.
   static void SetUpTestSuite()
   {
      randomcpp::seed(1); // Set the seed once for all tests
   }

   // Optionally, you can use SetUp() to reset the seed for each test case (if needed)
   void SetUp() override
   {
      randomcpp::reset(); // Reset random state before each test
   }
};

TEST_F(RandomIntegersTest, RandomIntegerInsidePositiveRange)
{
   for (unsigned i = 0; i < 100; ++i)
   {
      int rand_i = randomcpp::randint(0, 5);
      EXPECT_LE(rand_i, 5);
      EXPECT_GE(rand_i, 0);
   }
}

TEST_F(RandomIntegersTest, RandomIntegerInsideNegativeRange)
{
   for (unsigned i = 0; i < 100; ++i)
   {
      int rand_i = randomcpp::randint(-10, -5);
      EXPECT_LE(rand_i, -5);
      EXPECT_GE(rand_i, -10);
   }
}

TEST_F(RandomIntegersTest, RandomRangeStop)
{
   for (unsigned i = 0; i < 100; ++i)
   {
      int rand_i = randomcpp::randrange(5);
      EXPECT_LT(rand_i, 5);
      EXPECT_GE(rand_i, 0);
   }
}

TEST_F(RandomIntegersTest, RandomRangeStartStop)
{
   for (unsigned i = 0; i < 100; ++i)
   {
      int rand_i = randomcpp::randrange(1, 6);
      EXPECT_LT(rand_i, 6);
      EXPECT_GE(rand_i, 1);
   }
}

TEST_F(RandomIntegersTest, RandomRangeStartStopStepOdd)
{
   for (unsigned i = 0; i < 100; ++i)
   {
      int rand_i = randomcpp::randrange(1, 6, 2);
      EXPECT_LT(rand_i, 6);
      EXPECT_GE(rand_i, 1);
      EXPECT_EQ(rand_i % 2, 1); // Odd
   }
}

TEST_F(RandomIntegersTest, RandomIntegerRangeWithBoth)
{
   for (unsigned i = 0; i < 100; ++i)
   {
      int rand_neg = randomcpp::randint(-5, 5);
      EXPECT_LE(rand_neg, 5);
      EXPECT_GE(rand_neg, -5);
   }
}

TEST_F(RandomIntegersTest, RepeatRandomIntegersNotEqual)
{
   for (unsigned i = 0; i < 100; ++i)
   {
      EXPECT_NE(randomcpp::randint(0, 1000), randomcpp::randint(0, 1000));
   }
}