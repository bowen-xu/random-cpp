#include <gtest/gtest.h>

#include <randomcpp.hpp>
#include <vector>
#include <array>
#include <list>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <string>
#include <forward_list>

// Helper function for approximately comparing floats
bool approximately_equal(float a, float b, float tolerance = 0.0001f)
{
   return std::abs(a - b) <= tolerance;
}

class RandomSequencesTest : public ::testing::Test
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

// Choice Tests
TEST(RandomSequencesTest, ChoiceFromEmptyVectorThrows)
{
   std::vector<int> sequence;
   EXPECT_THROW(randomcpp::choice(sequence), std::logic_error);
}

TEST(RandomSequencesTest, ChoiceFromIntegerVector)
{
   std::vector<int> sequence = {5, 6, 7, 8, 9};
   for (unsigned i = 0; i < 100; ++i)
   {
      auto choice = randomcpp::choice(sequence);
      EXPECT_GE(choice, 5);
      EXPECT_LE(choice, 9);
   }
}

TEST(RandomSequencesTest, ChoiceFromFloatVector)
{
   std::vector<float> sequence;
   for (int i = 5; i < 50; ++i)
   {
      sequence.push_back(i + 0.5f);
   }
   for (unsigned i = 0; i < 100; ++i)
   {
      auto choice = randomcpp::choice(sequence);
      EXPECT_GE(choice, 5.5f);
      EXPECT_LE(choice, 49.5f);
      EXPECT_TRUE(approximately_equal(std::round(choice) - choice, 0.5f));
   }
}

TEST(RandomSequencesTest, ChoiceFromDoubleVector)
{
   std::vector<double> sequence;
   for (int i = 5; i < 50; ++i)
   {
      sequence.push_back(i + 0.8);
   }
   for (unsigned i = 0; i < 100; ++i)
   {
      auto choice = randomcpp::choice(sequence);
      EXPECT_GE(choice, 5.8);
      EXPECT_LE(choice, 49.8);
   }
}

TEST(RandomSequencesTest, ChoiceFromDumbArray)
{
   int sequence[] = {4, 6, 8, 10, 12, 14};
   for (unsigned i = 0; i < 50; ++i)
   {
      auto choice = randomcpp::choice(sequence);
      EXPECT_GE(choice, 4);
      EXPECT_LE(choice, 14);
      EXPECT_EQ(choice % 2, 0);
   }
}

TEST(RandomSequencesTest, ChoiceFromStdArray)
{
   std::array<int, 5> sequence = {0, 1, 2, 3, 4};
   for (unsigned i = 0; i < 100; ++i)
   {
      auto choice = randomcpp::choice(sequence);
      EXPECT_GE(choice, 0);
      EXPECT_LE(choice, 4);
   }
}

TEST(RandomSequencesTest, ChoiceFromSingleElement)
{
   std::vector<double> sequence = {8.888};
   for (unsigned i = 0; i < 100; ++i)
   {
      EXPECT_EQ(8.888, randomcpp::choice(sequence));
   }
}

TEST(RandomSequencesTest, ChoiceFromIntegerList)
{
   std::list<int> sequence = {5, 6, 7, 8, 9};
   for (unsigned i = 0; i < 100; ++i)
   {
      auto choice = randomcpp::choice(sequence);
      EXPECT_GE(choice, 5);
      EXPECT_LE(choice, 9);
   }
}

TEST(RandomSequencesTest, ChoiceFromIntegerForwardList)
{
   std::forward_list<int> sequence = {9, 8, 7, 6, 5};
   for (unsigned i = 0; i < 100; ++i)
   {
      auto choice = randomcpp::choice(sequence);
      EXPECT_GE(choice, 5);
      EXPECT_LE(choice, 9);
   }
}

TEST(RandomSequencesTest, ChoiceFromSet)
{
   std::set<int> sequence = {5, 6, 7, 8, 9};
   for (unsigned i = 0; i < 100; ++i)
   {
      auto choice = randomcpp::choice(sequence);
      EXPECT_GE(choice, 5);
      EXPECT_LE(choice, 9);
   }
}

TEST(RandomSequencesTest, ChoiceFromUnorderedSet)
{
   std::unordered_set<int> sequence = {5, 6, 7, 8, 9};
   for (unsigned i = 0; i < 100; ++i)
   {
      auto choice = randomcpp::choice(sequence);
      EXPECT_GE(choice, 5);
      EXPECT_LE(choice, 9);
   }
}

TEST(RandomSequencesTest, ChoiceFromMap)
{
   std::map<int, int> sequence = {{5, 42}, {6, 42}, {7, 42}, {8, 42}, {9, 42}};
   for (unsigned i = 0; i < 100; ++i)
   {
      auto choice = randomcpp::choice(sequence);
      EXPECT_EQ(choice.second, 42);
      EXPECT_GE(choice.first, 5);
      EXPECT_LE(choice.first, 9);
   }
}

TEST(RandomSequencesTest, ChoiceFromUnorderedMap)
{
   std::unordered_map<int, int> sequence = {{5, 24}, {6, 24}, {7, 24}, {8, 24}, {9, 24}};
   for (unsigned i = 0; i < 100; ++i)
   {
      auto choice = randomcpp::choice(sequence);
      EXPECT_EQ(choice.second, 24);
      EXPECT_GE(choice.first, 5);
      EXPECT_LE(choice.first, 9);
   }
}

TEST(RandomSequencesTest, ChoiceFromString)
{
   std::string sequence("crazy");
   for (unsigned i = 0; i < 100; ++i)
   {
      auto choice = randomcpp::choice(sequence);
      EXPECT_NE(sequence.find(choice), std::string::npos);
   }
}

// Shuffle Tests
TEST(RandomSequencesTest, ShuffleIntegerVector)
{
   std::vector<int> sequence = {5, 6, 7, 8, 9};
   auto sequence_old(sequence);
   randomcpp::shuffle(&sequence);
   EXPECT_NE(sequence, sequence_old);
}

TEST(RandomSequencesTest, ShuffleString)
{
   std::string sequence("crazy");
   std::string sequence_old(sequence);
   EXPECT_EQ(sequence, sequence_old);
   randomcpp::shuffle(&sequence);
   EXPECT_NE(sequence, sequence_old);
}

TEST(RandomSequencesTest, ShuffleDumbArray)
{
   int sequence[] = {6, 8, 10, 12, 14, 16, 18, 20, 22};

   // auto sequence_old = std::copy(sequence);
   int sequence_old[sizeof(sequence) / sizeof(sequence[0])];
   std::copy(std::begin(sequence), std::end(sequence), std::begin(sequence_old));
   randomcpp::shuffle(&sequence);
   EXPECT_NE(sequence, sequence_old);
}

// Sample and Additional Tests
TEST(RandomSequencesTest, SampleIsUnique)
{
   std::string sequence("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890");
   std::set<char> uniqueness_checker;
   for (unsigned i = 0; i < 100; ++i)
   {
      uniqueness_checker.clear();
      auto sub_seq = randomcpp::sample(sequence, 4);
      for (const auto &el : sub_seq)
      {
         EXPECT_TRUE(uniqueness_checker.insert(el).second);
      }
   }
}

// Sample integer vector
TEST(RandomSequencesTest, SampleIntegerVector)
{
   std::vector<int> sequence;
   for (int i = 0; i < 60; i += 3)
   {
      sequence.push_back(i);
   }

   for (unsigned i = 0; i < 100; ++i)
   {
      auto sub_seq = randomcpp::sample(sequence, 12);
      for (const auto &el : sub_seq)
      {
         auto itr = std::find(sequence.begin(), sequence.end(), el);
         EXPECT_NE(itr, sequence.end());
      }
   }
}

// Sample float vector
TEST(RandomSequencesTest, SampleFloatVector)
{
   std::vector<float> sequence;
   for (int i = 0; i < 60; i += 3)
   {
      sequence.push_back(i * 1.5f);
   }

   for (unsigned i = 0; i < 100; ++i)
   {
      auto sub_seq = randomcpp::sample(sequence, 12);
      for (const auto &el : sub_seq)
      {
         auto itr = std::find(sequence.begin(), sequence.end(), el);
         EXPECT_NE(itr, sequence.end());
      }
   }
}

// Sample integer list
TEST(RandomSequencesTest, SampleIntegerList)
{
   std::list<int> sequence;
   for (int i = 0; i < 60; i += 4)
   {
      sequence.push_back(i);
   }

   for (unsigned i = 0; i < 100; ++i)
   {
      auto sub_seq = randomcpp::sample(sequence, 12);
      for (const auto &el : sub_seq)
      {
         auto itr = std::find(sequence.begin(), sequence.end(), el);
         EXPECT_NE(itr, sequence.end());
      }
   }
}

// Sample string
TEST(RandomSequencesTest, SampleString)
{
   std::string sequence("a suPeR LoNg String!");

   for (unsigned i = 0; i < 100; ++i)
   {
      auto sub_seq = randomcpp::sample(sequence, 4);
      for (const auto &el : sub_seq)
      {
         auto itr = std::find(sequence.begin(), sequence.end(), el);
         EXPECT_NE(itr, sequence.end());
      }
   }
}

// Sample set
TEST(RandomSequencesTest, SampleSet)
{
   std::set<int> set;
   for (int i = 0; i < 60; i += 3)
   {
      set.insert(i);
   }

   for (unsigned i = 0; i < 100; ++i)
   {
      auto sub_set = randomcpp::sample(set, 4);
      for (const auto &el : sub_set)
      {
         auto itr = set.find(el);
         EXPECT_NE(itr, set.end());
      }
   }
}

// Sample unordered_set
TEST(RandomSequencesTest, SampleUnorderedSet)
{
   std::unordered_set<int> set;
   for (int i = 0; i < 60; i += 3)
   {
      set.insert(i);
   }

   for (unsigned i = 0; i < 100; ++i)
   {
      auto sub_set = randomcpp::sample(set, 4);
      for (const auto &el : sub_set)
      {
         auto itr = set.find(el);
         EXPECT_NE(itr, set.end());
      }
   }
}

// Sample map
TEST(RandomSequencesTest, SampleMap)
{
   std::map<int, int> map;
   for (int i = 5; i < 10; ++i)
   {
      map.insert(std::make_pair(i, 42));
   }

   for (unsigned i = 0; i < 100; ++i)
   {
      auto sub_map = randomcpp::sample(map, 4);
      for (const auto &el : sub_map)
      {
         auto itr = map.find(el.first);
         EXPECT_NE(itr, map.end());
      }
   }
}

// Sample unordered_map
TEST(RandomSequencesTest, SampleUnorderedMap)
{
   std::unordered_map<int, int> map;
   for (int i = 5; i < 10; ++i)
   {
      map.insert(std::make_pair(i, 42));
   }

   for (unsigned i = 0; i < 100; ++i)
   {
      auto sub_map = randomcpp::sample(map, 4);
      for (const auto &el : sub_map)
      {
         auto itr = map.find(el.first);
         EXPECT_NE(itr, map.end());
      }
   }
}

// Sample integer array
TEST(RandomSequencesTest, SampleIntegerArray)
{
   std::array<int, 30> sequence;
   for (int i = 0; i < 30; ++i)
   {
      sequence[i] = i;
   }

   for (unsigned i = 0; i < 100; ++i)
   {
      std::array<int, 10> sub_seq;
      randomcpp::sample(sequence, &sub_seq);
      for (const auto &el : sub_seq)
      {
         EXPECT_LT(el, 30);
      }
   }
}

// Sample integer dumb array
TEST(RandomSequencesTest, SampleIntegerDumbArray)
{
   int sequence[30];
   for (int i = 0; i < 30; ++i)
   {
      sequence[i] = i;
   }

   for (unsigned i = 0; i < 100; ++i)
   {
      int sub_seq[10];
      randomcpp::sample(sequence, &sub_seq);
      for (const auto &el : sub_seq)
      {
         EXPECT_LT(el, 30);
      }
   }
}

// has_resize helper
TEST(RandomSequencesTest, HasResizeHelper)
{
   EXPECT_TRUE(randomcpp::_detail::has_resize<std::vector<int>>::value);
   EXPECT_TRUE(randomcpp::_detail::has_resize<std::vector<float>>::value);
   EXPECT_TRUE(randomcpp::_detail::has_resize<std::list<int>>::value);
   EXPECT_TRUE(randomcpp::_detail::has_resize<std::string>::value);

   EXPECT_FALSE(randomcpp::_detail::has_resize<std::set<int>>::value);
   EXPECT_FALSE(randomcpp::_detail::has_resize<std::unordered_set<int>>::value);

   bool has;
   has = randomcpp::_detail::has_resize<std::map<int, int>>::value;
   EXPECT_FALSE(has);

   has = randomcpp::_detail::has_resize<std::map<int, std::string>>::value;
   EXPECT_FALSE(has);

   has = randomcpp::_detail::has_resize<std::map<std::string, std::string>>::value;
   EXPECT_FALSE(has);

   has = randomcpp::_detail::has_resize<std::unordered_map<std::string, std::string>>::value;
   EXPECT_FALSE(has);

   has = randomcpp::_detail::has_resize<std::array<int, 10>>::value;
   EXPECT_FALSE(has);
}
