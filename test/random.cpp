#include <catch.hpp>

#include <algorithm>

#include <randomcpp/random.hpp>
#include <map>

TEST_CASE( "Test for random", "[random]" ) {
   random::seed(1);

   SECTION ( "random float inside range" ) {
      float rand_f = random::uniform(0, 0.5);
      REQUIRE(rand_f < 0.5);
      REQUIRE(rand_f > 0.0);
   }

   SECTION ( "random reset produces repeatable results" ) {
      random::reset();
      int rand_1 = random::randint(0, 100);
      random::reset();
      int rand_2 = random::randint(0, 100);
      REQUIRE(rand_1 == rand_2);
   }

   SECTION ( "different seeds, different results" ) {
      random::seed(2);
      int rand_1 = random::randint(0, 100);
      random::seed(3);
      int rand_2 = random::randint(0, 100);
      REQUIRE(rand_1 != rand_2);
   }

   SECTION ( "random vector all within range" ) {
      random::seed(2);
      auto rand_set = random::sample(0, 100, 20);
      // Check the size
      REQUIRE(rand_set.size() == 20);
      // Check the range
      for (auto i : rand_set) {
         CHECK(i <= 100);
         CHECK(i >= 0);
      }
   }

   SECTION ( "random vector all unique" ) {
      random::reset();
      auto rand_set = random::sample(0, 100, 20, true);
      // Check the size
      REQUIRE(rand_set.size() == 20);
      // Check for uniqueness
      sort( rand_set.begin(), rand_set.end() );
      REQUIRE( adjacent_find(rand_set.begin(), rand_set.end() ) == rand_set.end() );
      // Check the range
      for (auto i : rand_set) {
         CHECK(i <= 100);
         CHECK(i >= 0);
      }
   }

   SECTION ( "random vector invalid range" ) {
      random::reset();
      REQUIRE_THROWS(random::sample(0, 5, 20, true));
   }

   SECTION ( "guassian random" ) {
      random::reset();
      std::map<int, int> hist;
      for (int n=0; n<10000; ++n) {
         ++hist[std::round(random::gauss(5.0, 2.0))];
      }
      // Validate histogram
   }
}