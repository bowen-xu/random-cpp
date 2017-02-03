#include <catch.hpp>

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

TEST_CASE( "Test for random sequences", "[sequences]" ) {
   randomcpp::seed(1);

   SECTION ( "choice from vector" ) {
      std::vector<int> sequence;
      REQUIRE_THROWS_AS(randomcpp::choice(sequence), std::logic_error);
   }

   SECTION ( "choice from integer vector" ) {
      std::vector<int> sequence;
      for (int i=5; i < 10; i++) {
         sequence.push_back(i);
      }

      for (unsigned i=0; i < 100; ++i) {
         auto choice = randomcpp::choice(sequence);
         REQUIRE(choice >= 5);
         REQUIRE(choice <= 9);
      }
   }

   SECTION ( "choice from float vector" ) {
      std::vector<float> sequence;
      for (int i=5; i < 50; i++) {
         sequence.push_back(i + 0.5f);
      }

      for (unsigned i=0; i < 100; ++i) {
         auto choice = randomcpp::choice(sequence);
         REQUIRE(choice >= 5.5);
         REQUIRE(choice <= 49.5);
         REQUIRE(std::round(choice) - choice == 0.5f);
      }
   }

   SECTION ( "choice from double vector" ) {
      std::vector<double> sequence;
      for (int i=5; i < 50; i++) {
         sequence.push_back(i + 0.8);
      }

      for (unsigned i=0; i < 100; ++i) {
         auto choice = randomcpp::choice(sequence);
         REQUIRE(choice >= 5.8);
         REQUIRE(choice <= 49.8);
      }
   }

   SECTION ( "choice from dumb array" ) {
      int sequence[] = {4, 6, 8, 10, 12, 14};

      for (unsigned i=0; i < 50; ++i) {
         auto choice = randomcpp::choice(sequence);
         REQUIRE(choice >= 4);
         REQUIRE(choice <= 14);
         REQUIRE(choice % 2 == 0);
      }
   }

   SECTION ( "choice from array" ) {
      std::array<int, 5> sequence;
      for (int i=0; i < 5; i++) {
         sequence[i] = i;
      }

      for (unsigned i=0; i < 100; ++i) {
         auto choice = randomcpp::choice(sequence);
         REQUIRE(choice >= 0);
         REQUIRE(choice <= 4);
      }
   }

   SECTION ( "choice from single element" ) {
      std::vector<double> sequence;
      double eight = 8.888;
      sequence.push_back(eight);

      for (unsigned i=0; i < 100; ++i) {
         REQUIRE(eight == randomcpp::choice(sequence));
      }
   }

   SECTION ( "choice from integer list" ) {
      std::list<int> sequence;
      for (int i=5; i < 10; i++) {
         sequence.push_back(i);
      }

      for (unsigned i=0; i < 100; ++i) {
         auto choice = randomcpp::choice(sequence);
         REQUIRE(choice >= 5);
         REQUIRE(choice <= 9);
      }
   }

   SECTION ( "choice from integer forward_list" ) {
      std::forward_list<int> sequence;
      for (int i=5; i < 10; i++) {
         sequence.push_front(i);
      }

      for (unsigned i=0; i < 100; ++i) {
         auto choice = randomcpp::choice(sequence);
         REQUIRE(choice >= 5);
         REQUIRE(choice <= 9);
      }
   }

   SECTION ( "choice from integer set" ) {
      std::set<int> sequence;
      for (int i=5; i < 10; i++) {
         sequence.insert(i);
      }

      for (unsigned i=0; i < 100; ++i) {
         auto choice = randomcpp::choice(sequence);
         REQUIRE(choice >= 5);
         REQUIRE(choice <= 9);
      }
   }

   SECTION ( "choice from integer unordered_set" ) {
      std::unordered_set<int> sequence;
      for (int i=5; i < 10; i++) {
         sequence.insert(i);
      }

      for (unsigned i=0; i < 100; ++i) {
         auto choice = randomcpp::choice(sequence);
         REQUIRE(choice >= 5);
         REQUIRE(choice <= 9);
      }
   }

   SECTION ( "choice from integer map" ) {
      std::map<int, int> sequence;
      for (int i=5; i < 10; i++) {
         sequence.insert(std::make_pair(i, 42));
      }

      for (unsigned i=0; i < 100; ++i) {
         auto choice = randomcpp::choice(sequence);
         REQUIRE(choice.second == 42);
         REQUIRE(choice.first >= 5);
         REQUIRE(choice.first <= 9);
      }
   }

   SECTION ( "choice from integer unordered_map" ) {
      std::unordered_map<int, int> sequence;
      for (int i=5; i < 10; i++) {
         sequence.insert(std::make_pair(i, 24));
      }

      for (unsigned i=0; i < 100; ++i) {
         auto choice = randomcpp::choice(sequence);
         REQUIRE(choice.second == 24);
         REQUIRE(choice.first >= 5);
         REQUIRE(choice.first <= 9);
      }
   }

   SECTION ( "choice from string" ) {
      std::string sequence("crazy");

      for (unsigned i=0; i < 100; ++i) {
         auto choice = randomcpp::choice(sequence);
         REQUIRE(sequence.find(choice) != std::string::npos);
      }
   }

   SECTION ( "shuffle integer vector" ) {
      std::vector<int> sequence;
      for (int i=5; i < 10; i++) {
         sequence.push_back(i);
      }

      randomcpp::shuffle(&sequence);
      int i=5;
      for (auto const & r : sequence) {
         REQUIRE(r != i++);
      }
   }

   SECTION ( "shuffle string" ) {
      std::string sequence("crazy");
      std::string sequence_old(sequence);

      REQUIRE(sequence == sequence_old);
      randomcpp::shuffle(&sequence);
      REQUIRE(sequence != sequence_old);
   }

   SECTION ( "shuffle integer dumb array" ) {
      int sequence[] = {6, 8, 10, 12, 14, 16, 18, 20, 22};

      randomcpp::shuffle(&sequence);
      int i=6;
      for (auto const & r : sequence) {
         REQUIRE(r != i);
         i += 2;
      }
   }

   SECTION ( "sample is unique" ) {
      std::string sequence("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890");

      std::set<char> uniqueness_checker;
      for (unsigned i=0; i < 100; ++i) {
         uniqueness_checker.clear();
         auto sub_seq(randomcpp::sample(sequence, 4));
         for (auto const & el : sub_seq) {
            // Set maintains uniqueness for this sequence
            REQUIRE(uniqueness_checker.insert(el).second);
         }
      }
   }

   SECTION ( "sample integer vector" ) {
      std::vector<int> sequence;
      for (int i=0; i < 60; i+=3) {
         sequence.push_back(i);
      }

      for (unsigned i=0; i < 100; ++i) {
         auto sub_seq(randomcpp::sample(sequence, 12));
         for (auto const & el : sub_seq) {
            auto itr(std::find(sequence.begin(), sequence.end(), el));
            auto end(sequence.end());
            REQUIRE(itr != end);
         }
      }
   }

   SECTION ( "sample float vector" ) {
      std::vector<float> sequence;
      for (int i=0; i < 60; i+=3) {
         sequence.push_back(i*1.5f);
      }

      for (unsigned i=0; i < 100; ++i) {
         auto sub_seq(randomcpp::sample(sequence, 12));
         for (auto const & el : sub_seq) {
            auto itr(std::find(sequence.begin(), sequence.end(), el));
            auto end(sequence.end());
            REQUIRE(itr != end);
         }
      }
   }

   SECTION ( "sample integer list" ) {
      std::list<int> sequence;
      for (int i=0; i < 60; i+=4) {
         sequence.push_back(i);
      }

      for (unsigned i=0; i < 100; ++i) {
         auto sub_seq(randomcpp::sample(sequence, 12));
         for (auto const & el : sub_seq) {
            auto itr(std::find(sequence.begin(), sequence.end(), el));
            auto end(sequence.end());
            REQUIRE(itr != end);
         }
      }
   }

   SECTION ( "sample string" ) {
      std::string sequence("a suPeR LoNg String!");

      for (unsigned i=0; i < 100; ++i) {
         auto sub_seq(randomcpp::sample(sequence, 4));
         for (auto const & el : sub_seq) {
            auto itr(std::find(sequence.begin(), sequence.end(), el));
            auto end(sequence.end());
            REQUIRE(itr != end);
         }
      }
   }
}