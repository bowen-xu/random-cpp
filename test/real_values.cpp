#include <catch.hpp>

#include <randomcpp.hpp>

TEST_CASE( "Test for random real values", "[real values]" ) {
   randomcpp::seed(1);

   SECTION ( "random() is always in correct range" ) {
      float min = 5, max = 0;
      for (unsigned i=0; i < 1000; ++i) {
         auto r(randomcpp::random());
         REQUIRE(r >= 0.0f);
         REQUIRE(r < 1.0f);
         min = std::min(min, r);
         max = std::max(max, r);
      }
      // Require one of the values was close to 0, 1
      REQUIRE(max > 0.9999f);
      REQUIRE(min < 0.0015f);
   }

   SECTION ( "uniform range is correct (a<b)" ) {
      float high = 2.5;
      for (float low=0.5; low < 10.0; low += 0.8) {
         float min = high, max = low;
         for (unsigned i=0; i < 100; i++) {
            auto r(randomcpp::uniform(low, high));
            REQUIRE(r <= high);
            REQUIRE(r >= low);
            min = std::min(min, r);
            max = std::max(max, r);
         }
         // Require one of the values was close to high/low
         REQUIRE(max > (high - 0.5));
         REQUIRE(min < (min + 0.5));
         high += 1.3;
      }
   }

   SECTION ( "uniform range is correct (a>b)" ) {
      float high = 2.5;
      for (float low=0.5; low < 10.0; low += 0.8) {
         float min = high, max = low;
         for (unsigned i=0; i < 100; i++) {
            auto r(randomcpp::uniform(high, low));
            REQUIRE(r <= high);
            REQUIRE(r >= low);
            min = std::min(min, r);
            max = std::max(max, r);
         }
         // Require one of the values was close to high/low
         REQUIRE(max > (high - 0.5));
         REQUIRE(min < (min + 0.5));
         high += 1.3;
      }
   }
}