#pragma once

#include <algorithm> // std::random_shuffle
#include <vector>
#include <random>
#include <stdexcept>
#include <iterator> // std::distance

namespace randomcpp {

// Bookkeeping functions:
/*
 * Initialize the random number generator.
 *    If a is omitted or None, the current system time is used.
 *    If a is an int, it is used directly.
 */
void seed();
void seed(unsigned a);

/*
 * Reseed the random generator with the stored seed.
 */
void reset();

// Functions for integers:
/*
 * Return a randomly selected element from range(start, stop, step).
 */
int randrange(int stop);
int randrange(int start, int stop, int step=1);

/*
 * Return a random integer N such that a <= N <= b. Alias for randrange(a, b+1).
 */
int randint(int a, int b);

// Functions for sequences:
/*
 * Return a random element from the non-empty sequence seq. If seq is empty, raises logic_error.
 */
template <typename TContainer>
typename TContainer::value_type choice(TContainer const & container) {
   auto begin(container.begin());
   auto size(std::distance(begin, container.end()));
   if (!size) {
      throw std::logic_error("Cannot choose from an empty sequence");
   }
   auto rand_index(randrange(size));
   std::advance(begin, rand_index);
   return *begin;
}

/*
 * Return a random element from the non-empty sequence seq. If seq is empty, raises logic_error.
 */
template <typename T, std::size_t N>
T choice(T const (&array)[N]) {
   if (!N) {
      throw std::logic_error("Cannot choose from an empty sequence");
   }
   auto rand_index(randrange(N));
   return array[rand_index];
}

/*
 * Shuffle the sequence x in place.
 *   Note that for even rather small len(x), the total number of permutations of x is larger than the period of most random number generators;
 *   this implies that most permutations of a long sequence can never be generated.
 */
template <typename TContainer>
void shuffle(TContainer * container) {
   std::random_shuffle(container->begin(), container->end());
}

/*
 * Shuffle the sequence x in place.
 *   Note that for even rather small len(x), the total number of permutations of x is larger than the period of most random number generators;
 *   this implies that most permutations of a long sequence can never be generated.
 */
template <typename T, std::size_t N>
void shuffle(T (*array)[N]) {
   for (unsigned i= N-1; i > 0; i--) {
      unsigned j = randrange(i + 1);
      std::swap((*array)[i], (*array)[j]);
   }
}

// Other functions
/*
 * Return value has a <probability_> chance of being true
 */
bool probability(float probability_);


/*
 * Return a k length list of unique (or not) elements chosen from the range.
 */
std::vector<int> sample(int a, int b, unsigned k, bool unique = false);

/*
 * Return a random floating point number N such that a <= N <= b.
 * Assumes: a <= b
 * TODO: support (b <= N <= a for b < a).
 */
float uniform(float a, float b);

/*
 * Gaussian distribution. mu is the mean, and sigma is the standard deviation.
 */
float gauss(float mu, float sigma);

} // namespace randomcpp
