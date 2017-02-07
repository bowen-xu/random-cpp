#include <catch.hpp>

#include <algorithm>

#include <randomcpp.hpp>
#include <map>

TEST_CASE( "Test for random", "[random]" ) {
   randomcpp::seed(1);

   SECTION ( "random float inside range" ) {
      float rand_f = randomcpp::uniform(0, 0.5);
      REQUIRE(rand_f < 0.5);
      REQUIRE(rand_f > 0.0);
   }

   SECTION ( "random reset produces repeatable results" ) {
      randomcpp::reset();
      int rand_1 = randomcpp::randint(0, 100);
      randomcpp::reset();
      int rand_2 = randomcpp::randint(0, 100);
      REQUIRE(rand_1 == rand_2);
   }

   SECTION ( "different seeds, different results" ) {
      randomcpp::seed(2);
      int rand_1 = randomcpp::randint(0, 100);
      randomcpp::seed(3);
      int rand_2 = randomcpp::randint(0, 100);
      REQUIRE(rand_1 != rand_2);
   }

   SECTION ( "random vector all within range" ) {
      randomcpp::seed(2);
      auto rand_set = randomcpp::sample(0, 100, 20);
      // Check the size
      REQUIRE(rand_set.size() == 20);
      // Check the range
      for (auto i : rand_set) {
         CHECK(i <= 100);
         CHECK(i >= 0);
      }
   }

   SECTION ( "random vector all unique" ) {
      randomcpp::reset();
      auto rand_set = randomcpp::sample(0, 100, 20, true);
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
      randomcpp::reset();
      REQUIRE_THROWS(randomcpp::sample(0, 5, 20, true));
   }
}