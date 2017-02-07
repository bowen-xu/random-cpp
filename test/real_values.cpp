#include <catch.hpp>

#include <randomcpp.hpp>
#include <map>

static void display_histogram(std::map<int, int> const & hist) {
   for (auto const & i : hist) {
      std::printf("%d: %s\n", i.first, std::string(i.second / 15, '*').c_str());
   }
}

static int key_for_max_value(std::map<int, int> const & hist) {
   std::pair<int, int> max{0, -1};
   for (auto const & i : hist) {
      if (i.second > max.second) {
         max = i;
      }
   }
   return max.first;
}

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

   SECTION ( "triangular dist (default)" ) {
      randomcpp::reset();
      std::map<int, int> histogram;
      for (int n=0; n < 5000; n++) {
         ++histogram[std::round(randomcpp::triangular()*10)];
      }
      //display_histogram(histogram);
      REQUIRE(key_for_max_value(histogram) == 5);
   }

   SECTION ( "triangular dist (alternate range)" ) {
      randomcpp::reset();
      std::map<int, int> histogram;
      for (int n=0; n < 5000; n++) {
         ++histogram[std::round(randomcpp::triangular(0.0, 10.0))];
      }
      //display_histogram(histogram);
      REQUIRE(key_for_max_value(histogram) == 5);
   }

   SECTION ( "triangular dist (alternate mode)" ) {
      randomcpp::reset();
      std::map<int, int> histogram;
      for (int n=0; n < 5000; n++) {
         ++histogram[std::round(randomcpp::triangular(0.0, 1.0, 0.2)*10)];
      }
      //display_histogram(histogram);
      REQUIRE(key_for_max_value(histogram) == 2);
   }

   SECTION ( "betavariate dist" ) {
      randomcpp::reset();
      std::map<int, int> histogram;
      for (int n=0; n < 5000; n++) {
         ++histogram[std::round(randomcpp::betavariate(2.0, 5.0)*10)];
      }
      //display_histogram(histogram);
      REQUIRE(key_for_max_value(histogram) == 2);
   }

   SECTION ( "betavariate dist (alt)" ) {
      randomcpp::reset();
      std::map<int, int> histogram;
      for (int n=0; n < 5000; n++) {
         ++histogram[std::round(randomcpp::betavariate(2, 2)*10)];
      }
      //display_histogram(histogram);
      REQUIRE(key_for_max_value(histogram) == 5);
   }

   SECTION ( "expovariate dist (lambda=9)" ) {
      randomcpp::reset();
      float mean = 0;
      for (int n=0; n < 5000; n++) {
         mean += randomcpp::expovariate(1.0f/9);
      }
      mean /= 5000;
      REQUIRE(std::round(mean) == 9.0f);
   }

   SECTION ( "gammavariate dist" ) {
      randomcpp::reset();
      std::map<int, int> histogram;
      for (int n=0; n < 5000; n++) {
         ++histogram[std::round(randomcpp::gammavariate(1.0, 1.0)*10)];
      }
      //display_histogram(histogram);
      REQUIRE(key_for_max_value(histogram) == 1);
   }
}