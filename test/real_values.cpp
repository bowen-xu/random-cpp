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
      REQUIRE(max > 0.9999f);
      REQUIRE(min < 0.0015f);
   }
}