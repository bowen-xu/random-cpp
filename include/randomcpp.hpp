#pragma once

#include <algorithm> // std::random_shuffle
#include <vector>
#include <random>
#include <stdexcept>
#include <set>
#include <iterator> // std::distance
#include <array>

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

namespace _detail {

template<typename T>
void sample_dumb_array(T const* population, T* result, std::size_t n, std::size_t k) {
   std::set<unsigned> selected;
   for (unsigned i = 0; i < k; i++) {
      unsigned j = randrange(n);
      while (!selected.insert(j).second) {
         j = randrange(n);
      }
      *(result + i) = *(population + j);
   }
}

template<typename T>
struct has_resize {
   template<typename U, void (U::*)(std::size_t)>
   struct resize_signature {};

   template<typename U>
   static char Test(resize_signature<U, &U::resize>*);

   template<typename U>
   static int Test(...);

   static const bool value = sizeof(Test<T>(0)) == sizeof(char);
};

} // namespace _detail

/*
 * Return a k length list of unique elements chosen from the population sequence or set. Used for random sampling without replacement.
 *   Returns a new list containing elements from the population while leaving the original population unchanged.
 *   The resulting list is in selection order so that all sub-slices will also be valid random samples.
 *   This allows raffle winners (the sample) to be partitioned into grand prize and second place winners (the subslices).
 */
template <typename TPopulation,
      typename std::enable_if<_detail::has_resize<TPopulation>::value, int>::type = 0>
TPopulation sample(TPopulation const & population, std::size_t k) {
   std::set<unsigned> selected;
   TPopulation result;
   result.resize(k);
   auto size(std::distance(population.begin(), population.end()));
   auto result_itr(result.begin());
   auto population_itr(population.begin());
   for (unsigned i=0; i < k; i++) {
      unsigned j = randrange(size);
      while (!selected.insert(j).second) {
         j = randrange(size);
      }
      population_itr = population.begin();
      std::advance(population_itr, j);
      *result_itr = *population_itr;
      std::advance(result_itr, 1);
   }
   return std::move(result);
}

template <typename TPopulation,
      typename std::enable_if<!_detail::has_resize<TPopulation>::value, int>::type = 0>
TPopulation sample(TPopulation const & population, std::size_t k) {
   std::set<unsigned> selected;
   TPopulation result;
   auto size(std::distance(population.begin(), population.end()));
   auto result_itr(result.begin());
   auto population_itr(population.begin());
   for (unsigned i=0; i < k; i++) {
      unsigned j = randrange(size);
      while (!selected.insert(j).second) {
         j = randrange(size);
      }
      population_itr = population.begin();
      std::advance(population_itr, j);
      result.insert(*population_itr);
   }
   return std::move(result);
}

template <typename T, std::size_t N, std::size_t K>
void sample(T const (&population)[N], T (*result)[K]) {
   _detail::sample_dumb_array<T>(&population[0], &(*result)[0], N, K);
}

template <typename T, std::size_t N, std::size_t K>
void sample(std::array<T, N> const & population, std::array<T, K> * result) {
   _detail::sample_dumb_array<T>(population.data(), result->data(), N, K);
}

// The following functions generate specific real-valued distributions.
//   Function parameters are named after the corresponding variables in the distribution’s equation,
//   as used in common mathematical practice; most of these equations can be found in any statistics text.

/*
 * Return the next random floating point number in the range [0.0, 1.0).
 */
float random();

/*
 * Return a random floating point number N such that a <= N <= b for a <= b and b <= N <= a for b < a.
 *   The end-point value b may or may not be included in the range depending on floating-point rounding in the equation a + (b-a) * random().
 */
float uniform(float a, float b);

/*
 * Return a random floating point number N such that low <= N <= high and with the specified mode between those bounds.
 *   The low and high bounds default to zero and one.
 *   The mode argument defaults to the midpoint between the bounds, giving a symmetric distribution.
 */
float triangular(float low = 0.0, float high = 1.0, float mode = 0.5);

/*
 * Beta distribution. Conditions on the parameters are alpha > 0 and beta > 0. Returned values range between 0 and 1.
 */
float betavariate(float alpha, float beta);

/*
 * Exponential distribution. lambda is 1.0 divided by the desired mean. It should be nonzero.
 *   Returned values range from 0 to positive infinity if lambda is positive,
 *   and from negative infinity to 0 if lambda is negative.
 */
float expovariate(float lambda);

/*
 * Gamma distribution. (Not the gamma function!) Conditions on the parameters are alpha > 0 and beta > 0.
 *   The probability distribution function is:
 *
 *              x ** (alpha - 1) * math.exp(-x / beta)
 *    pdf(x) =  --------------------------------------
 *                math.gamma(alpha) * beta ** alpha
 */
float gammavariate(float alpha, float beta);

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
 * Gaussian distribution. mu is the mean, and sigma is the standard deviation.
 */
float gauss(float mu, float sigma);

} // namespace randomcpp
