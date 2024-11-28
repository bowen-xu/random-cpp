#include <gtest/gtest.h>

#include <randomcpp.hpp>
#include <map>

static void display_histogram(std::map<int, int> const &hist)
{
   for (auto const &i : hist)
   {
      std::printf("%d: %s\n", i.first, std::string(i.second / 15, '*').c_str());
   }
}

static int key_for_max_value(std::map<int, int> const &hist)
{
   std::pair<int, int> max{0, -1};
   for (auto const &i : hist)
   {
      if (i.second > max.second)
      {
         max = i;
      }
   }
   return max.first;
}

class RandomRealValuesTest : public ::testing::Test
{
protected:
   static void SetUpTestSuite()
   {
      randomcpp::seed(1); // Set a consistent seed for repeatability
   }

   void SetUp() override
   {
      randomcpp::reset(); // Reset random state before each test
   }
};

TEST_F(RandomRealValuesTest, RandomIsAlwaysInCorrectRange)
{
   float min = 5.0f, max = 0.0f;
   for (unsigned i = 0; i < 5000; ++i)
   {
      auto r = randomcpp::random();
      EXPECT_GE(r, 0.0f);
      EXPECT_LT(r, 1.0f);
      min = std::min(min, r);
      max = std::max(max, r);
   }
   EXPECT_GT(max, 0.9999f);
   EXPECT_LT(min, 0.0015f);
}

TEST_F(RandomRealValuesTest, UniformRangeIsCorrectAB)
{
   float high = 2.5f;
   for (float low = 0.5f; low < 10.0f; low += 0.8f)
   {
      float min = high, max = low;
      for (unsigned i = 0; i < 100; ++i)
      {
         auto r = randomcpp::uniform(low, high);
         EXPECT_LE(r, high);
         EXPECT_GE(r, low);
         min = std::min(min, r);
         max = std::max(max, r);
      }
      EXPECT_GT(max, high - 0.5f);
      EXPECT_LT(min, min + 0.5f);
      high += 1.3f;
   }
}

TEST_F(RandomRealValuesTest, UniformRangeIsCorrectBA)
{
   float high = 2.5f;
   for (float low = 0.5f; low < 10.0f; low += 0.8f)
   {
      float min = high, max = low;
      for (unsigned i = 0; i < 100; ++i)
      {
         auto r = randomcpp::uniform(high, low);
         EXPECT_LE(r, high);
         EXPECT_GE(r, low);
         min = std::min(min, r);
         max = std::max(max, r);
      }
      EXPECT_GT(max, high - 0.5f);
      EXPECT_LT(min, min + 0.5f);
      high += 1.3f;
   }
}

TEST_F(RandomRealValuesTest, TriangularDistDefault)
{
   std::map<int, int> histogram;
   for (int n = 0; n < 5000; ++n)
   {
      ++histogram[std::round(randomcpp::triangular() * 10)];
   }
   EXPECT_EQ(key_for_max_value(histogram), 5);
}

TEST_F(RandomRealValuesTest, TriangularDistAlternateRange)
{
   std::map<int, int> histogram;
   for (int n = 0; n < 5000; ++n)
   {
      ++histogram[std::round(randomcpp::triangular(0.0, 10.0))];
   }
   EXPECT_EQ(key_for_max_value(histogram), 5);
}

TEST_F(RandomRealValuesTest, TriangularDistAlternateMode)
{
   std::map<int, int> histogram;
   for (int n = 0; n < 5000; ++n)
   {
      ++histogram[std::round(randomcpp::triangular(0.0, 1.0, 0.2) * 10)];
   }
   EXPECT_EQ(key_for_max_value(histogram), 2);
}

TEST_F(RandomRealValuesTest, BetaVariateDist)
{
   std::map<int, int> histogram;
   for (int n = 0; n < 5000; ++n)
   {
      ++histogram[std::round(randomcpp::betavariate(2.0, 5.0) * 10)];
   }
   EXPECT_EQ(key_for_max_value(histogram), 2);
}

TEST_F(RandomRealValuesTest, BetaVariateDistAlt)
{
   std::map<int, int> histogram;
   for (int n = 0; n < 5000; ++n)
   {
      auto num = randomcpp::betavariate(2.0, 2.0) * 10;
      // std::cout << num << ' ';
      ++histogram[std::round(num)];
   }
   EXPECT_EQ(key_for_max_value(histogram), 5);

}

TEST_F(RandomRealValuesTest, ExpoVariateDist)
{
   float mean = 0.0f;
   for (int n = 0; n < 5000; ++n)
   {
      mean += randomcpp::expovariate(1.0f / 9);
   }
   mean /= 5000;
   EXPECT_EQ(std::round(mean), 9.0f);
}

TEST_F(RandomRealValuesTest, GammaVariateDist)
{
   std::map<int, int> histogram;
   for (int n = 0; n < 5000; ++n)
   {
      ++histogram[std::round(randomcpp::gammavariate(1.0, 1.0) * 10)];
   }
   EXPECT_EQ(key_for_max_value(histogram), 1);
}

TEST_F(RandomRealValuesTest, GaussianDist)
{
   float mean = 0.0f, stddev = 0.0f;
   for (int n = 0; n < 10000; ++n)
   {
      float u = randomcpp::gauss(5.0, 2.0);
      mean += u;
      float delta = u - 5.0f;
      stddev += delta * delta;
   }
   mean /= 10000;
   stddev = std::sqrt(stddev / 10000);
   EXPECT_EQ(std::round(mean), 5.0f);
   EXPECT_EQ(std::round(stddev), 2.0f);
}

TEST_F(RandomRealValuesTest, NormalVariateDist)
{
   float mean = 0.0f, stddev = 0.0f;
   for (int n = 0; n < 10000; ++n)
   {
      float u = randomcpp::normalvariate(8.0, 4.0);
      mean += u;
      float delta = u - 8.0f;
      stddev += delta * delta;
   }
   mean /= 10000;
   stddev = std::sqrt(stddev / 10000);
   EXPECT_EQ(std::round(mean), 8.0f);
   EXPECT_EQ(std::round(stddev), 4.0f);
}