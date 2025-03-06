#include <gtest/gtest.h>

#include <algorithm>

#include <randomcpp.hpp>
#include <map>

class RandomTest : public ::testing::Test
{
protected:
   // This function runs once for the entire test suite before any tests are run.
   static void SetUpTestSuite()
   {
      randomcpp::seed(1); // Set the seed once for all tests
   }

   // Optionally, use SetUp() to reset the random state for each test case if needed
   void SetUp() override
   {
      randomcpp::reset(); // Reset random state before each test
   }
};

TEST_F(RandomTest, RandomFloatInsideRange)
{
   float rand_f = randomcpp::uniform(0, 0.5);
   EXPECT_LT(rand_f, 0.5);
   EXPECT_GT(rand_f, 0.0);
}

TEST_F(RandomTest, RandomResetProducesRepeatableResults)
{
   randomcpp::reset();
   int rand_1 = randomcpp::randint(0, 100);
   randomcpp::reset();
   int rand_2 = randomcpp::randint(0, 100);
   EXPECT_EQ(rand_1, rand_2);
}

TEST_F(RandomTest, DifferentSeedsDifferentResults)
{
   randomcpp::seed(2);
   int rand_1 = randomcpp::randint(0, 100);
   randomcpp::seed(3);
   int rand_2 = randomcpp::randint(0, 100);
   EXPECT_NE(rand_1, rand_2);
}

TEST_F(RandomTest, RandomVectorAllWithinRange)
{
   randomcpp::seed(2);
   auto rand_set = randomcpp::sample(0, 100, 20);
   // Check the size
   EXPECT_EQ(rand_set.size(), 20);
   // Check the range
   for (auto i : rand_set)
   {
      EXPECT_LE(i, 100);
      EXPECT_GE(i, 0);
   }
}

TEST_F(RandomTest, RandomVectorAllUnique)
{
   randomcpp::reset();
   auto rand_set = randomcpp::sample(100, 200, 20, true);
   // Check the size
   EXPECT_EQ(rand_set.size(), 20);
   // Check for uniqueness
   std::sort(rand_set.begin(), rand_set.end());
   EXPECT_EQ(std::adjacent_find(rand_set.begin(), rand_set.end()), rand_set.end());
   // Check the range
   for (auto i : rand_set)
   {
      EXPECT_LE(i, 200);
      EXPECT_GE(i, 100);
   }
}

TEST_F(RandomTest, RandomVectorAllUnique2)
{
   randomcpp::reset();
   auto rand_set = randomcpp::sample(100, 2000, 20, true);
   // Check the size
   EXPECT_EQ(rand_set.size(), 20);
   // Check for uniqueness
   std::sort(rand_set.begin(), rand_set.end());
   EXPECT_EQ(std::adjacent_find(rand_set.begin(), rand_set.end()), rand_set.end());
   // Check the range
   for (auto i : rand_set)
   {
      EXPECT_LE(i, 2000);
      EXPECT_GE(i, 100);
   }
}

TEST_F(RandomTest, RandomVectorInvalidRange)
{
   randomcpp::reset();
   EXPECT_THROW(randomcpp::sample(0, 5, 20, true), std::exception);
}
