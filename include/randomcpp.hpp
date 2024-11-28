#pragma once

#include <algorithm> // std::random_shuffle
#include <vector>
#include <random>
#include <stdexcept>
#include <set>
#include <iterator> // std::distance
#include <array>

namespace randomcpp
{

    inline static unsigned seed_value = 0;
    inline static std::random_device rd;
    inline static std::mt19937 gen(rd());

    inline static float const SG_MAGICCONST = 1.0f + std::log(4.5f);
    inline static float const NV_MAGICCONST = static_cast<float>(4.0f * std::exp(-0.5) / std::sqrt(2.0f));

    static void initialize()
    {
        std::srand(seed_value);
    }

    // Bookkeeping functions:
    /*
     * Initialize the random number generator.
     *    If a is omitted or None, the current system time is used.
     *    If a is an int, it is used directly.
     */
    inline void seed(unsigned a)
    {
        seed_value = a;
        initialize();
    }

    inline void seed()
    {
        seed(static_cast<unsigned>(std::time(0)));
    }

    /*
     * Reseed the random generator with the stored seed.
     */
    inline void reset()
    {
        initialize();
    }

    // Functions for integers:
    /*
     * Return a randomly selected element from range(start, stop, step).
     */
    inline int _randbelow(int n)
    {
        return std::rand() % n;
    }

    inline int randrange(int start, int stop, int step = 1)
    {
        int width = stop - start;
        if (step == 1 && width > 0)
        {
            return start + _randbelow(width);
        }
        if (step == 1)
        {
            throw std::range_error("empty range for randrange()");
        }

        int n;
        if (step > 0)
        {
            n = (width + step - 1) / step;
        }
        else if (step < 0)
        {
            n = (width + step + 1) / step;
        }
        else
        {
            throw std::range_error("zero step for randrange()");
        }

        if (n <= 0)
        {
            throw std::range_error("empty range for randrange()");
        }

        return start + step * _randbelow(n);
    }

    inline int randrange(int stop)
    {
        return randrange(0, stop);
    }

    /*
     * Return a random integer N such that a <= N <= b. Alias for randrange(a, b+1).
     */
    inline int randint(int a, int b)
    {
        return randrange(a, b + 1);
    }

    // Functions for sequences:
    /*
     * Return a random element from the non-empty sequence seq. If seq is empty, raises logic_error.
     */
    template <typename TContainer>
    inline typename TContainer::value_type choice(TContainer const &container)
    {
        auto begin(container.begin());
        auto size(std::distance(begin, container.end()));
        if (!size)
        {
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
    inline T choice(T const (&array)[N])
    {
        if (!N)
        {
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
    inline void shuffle(TContainer *container)
    {
        // #if __cplusplus >= 201103L
        std::shuffle(container->begin(), container->end(), gen);
        // #else
        //     std::random_shuffle(container->begin(), container->end());
        // #endif
    }

    /*
     * Shuffle the sequence x in place.
     *   Note that for even rather small len(x), the total number of permutations of x is larger than the period of most random number generators;
     *   this implies that most permutations of a long sequence can never be generated.
     */
    template <typename T, std::size_t N>
    inline void shuffle(T (*array)[N])
    {
        for (unsigned i = N - 1; i > 0; i--)
        {
            unsigned j = randrange(i + 1);
            std::swap((*array)[i], (*array)[j]);
        }
    }

    namespace _detail
    {

        template <typename T>
        void sample_dumb_array(T const *population, T *result, std::size_t n, std::size_t k)
        {
            std::set<unsigned> selected;
            for (unsigned i = 0; i < k; i++)
            {
                unsigned j = randrange(n);
                while (!selected.insert(j).second)
                {
                    j = randrange(n);
                }
                *(result + i) = *(population + j);
            }
        }

        template <typename T>
        struct has_resize
        {
            template <typename U, void (U::*)(std::size_t)>
            struct resize_signature
            {
            };

            template <typename U>
            static char Test(resize_signature<U, &U::resize> *);

            template <typename U>
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
    inline TPopulation sample(TPopulation const &population, std::size_t k)
    {
        std::set<unsigned> selected;
        TPopulation result;
        result.resize(k);
        auto size(std::distance(population.begin(), population.end()));
        auto result_itr(result.begin());
        auto population_itr(population.begin());
        for (unsigned i = 0; i < k; i++)
        {
            unsigned j = randrange(size);
            while (!selected.insert(j).second)
            {
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
    inline TPopulation sample(TPopulation const &population, std::size_t k)
    {
        std::set<unsigned> selected;
        TPopulation result;
        auto size(std::distance(population.begin(), population.end()));
        auto result_itr(result.begin());
        auto population_itr(population.begin());
        for (unsigned i = 0; i < k; i++)
        {
            unsigned j = randrange(size);
            while (!selected.insert(j).second)
            {
                j = randrange(size);
            }
            population_itr = population.begin();
            std::advance(population_itr, j);
            result.insert(*population_itr);
        }
        return std::move(result);
    }

    template <typename T, std::size_t N, std::size_t K>
    inline void sample(T const (&population)[N], T (*result)[K])
    {
        _detail::sample_dumb_array<T>(&population[0], &(*result)[0], N, K);
    }

    template <typename T, std::size_t N, std::size_t K>
    inline void sample(std::array<T, N> const &population, std::array<T, K> *result)
    {
        _detail::sample_dumb_array<T>(population.data(), result->data(), N, K);
    }

    // The following functions generate specific real-valued distributions.
    //   Function parameters are named after the corresponding variables in the distribution’s equation,
    //   as used in common mathematical practice; most of these equations can be found in any statistics text.

    /*
     * Return the next random floating point number in the range [0.0, 1.0).
     */
    inline float random()
    {
        return static_cast<float>(std::rand()) / RAND_MAX;
    }

    /*
     * Return a random floating point number N such that a <= N <= b for a <= b and b <= N <= a for b < a.
     *   The end-point value b may or may not be included in the range depending on floating-point rounding in the equation a + (b-a) * random().
     */
    inline float uniform(float a, float b)
    {
        return a + (b - a) * random();
    }

    /*
     * Return a random floating point number N such that low <= N <= high and with the specified mode between those bounds.
     *   The low and high bounds default to zero and one.
     *   The mode argument defaults to the midpoint between the bounds, giving a symmetric distribution.
     */

    inline float triangular(float low = 0.0, float high = 1.0, float mode = 0.5)
    {
        auto u(random());
        auto& c = mode;
        if (u > c)
        {
            u = 1.0f - u;
            c = 1.0f - c;
            std::swap(low, high);
        }
        return low + (high - low) * std::pow((u * c), 0.5);
    }

    /*
     * Beta distribution. Conditions on the parameters are alpha > 0 and beta > 0. Returned values range between 0 and 1.
     */
    inline float gammavariate(float alpha, float beta);
    inline float betavariate(float alpha, float beta)
    {
        auto y = gammavariate(alpha, 1.0f);
        if (y != 0.0f)
        {
            y /= y + gammavariate(beta, 1.0f);
        }
        return y;
    }

    /*
     * Exponential distribution. lambda is 1.0 divided by the desired mean. It should be nonzero.
     *   Returned values range from 0 to positive infinity if lambda is positive,
     *   and from negative infinity to 0 if lambda is negative.
     */
    inline float expovariate(float lambda)
    {
        float u;
        do
        {
            u = random();
        } while (u <= 1e-7);
        return -std::log(u) / lambda;
    }

    /*
     * Gamma distribution. (Not the gamma function!) Conditions on the parameters are alpha > 0 and beta > 0.
     *   The probability distribution function is:
     *
     *              x ** (alpha - 1) * math.exp(-x / beta)
     *    pdf(x) =  --------------------------------------
     *                math.gamma(alpha) * beta ** alpha
     */

    inline float gammavariate(float alpha, float beta)
    {
        if (alpha < 0.0f || beta < 0.0f)
        {
            throw std::invalid_argument("gammavariate: alpha and beta must be > 0.0");
        }
        if (alpha > 1.0)
        {
            // Uses R.C.H. Cheng, "The generation of Gamma
            // variables with non-integral shape parameters",
            // Applied Statistics, (1977), 26, No. 1, p71-74
            float ainv = std::sqrt(2.0f * alpha - 1.0f);
            float bbb = alpha - static_cast<float>(std::log(4));
            float ccc = alpha + ainv;

            while (true)
            {
                float u1 = random();
                if (1e-7 < u1 < .9999999)
                {
                    continue;
                }
                float u2 = 1.0f - random();
                float v = std::log(u1 / (1.0f - u1)) / ainv;
                float x = alpha * std::exp(v);
                float z = u1 * u1 * u2;
                float r = bbb + ccc * v - x;
                if (r + SG_MAGICCONST - 4.5 * z >= 0.0 or r >= std::log(z))
                {
                    return x * beta;
                }
            }
        }
        else if (alpha == 1.0)
        {
            // expovariate(1)
            float u = random();
            while (u <= 1e-7)
            {
                u = random();
            }
            return -std::log(u) * beta;
        }
        else /* alpha is between 0 and 1 (exclusive) */
        {
            // Uses ALGORITHM GS of Statistical Computing - Kennedy & Gentle
            while (true)
            {
                float u = random();
                float b = (M_E + alpha) / M_E;
                float p = b * u;
                float x;
                if (p <= 1.0)
                {
                    x = std::pow(p, 1.0f / alpha);
                }
                else
                {
                    x = -std::log((b - p) / alpha);
                }
                float u1 = random();
                if (p > 1.0)
                {
                    if (u1 <= std::pow(x, alpha - 1.0f))
                    {
                        break;
                    }
                }
                else if (u1 <= std::exp(-x))
                {
                    break;
                }
                return x * beta;
            }
        }
    }

    /*
     * Gaussian distribution. mu is the mean, and sigma is the standard deviation.
     *   This is slightly faster than the normalvariate() function defined below.
     */

    inline float gauss(float mu, float sigma)
    {
        std::normal_distribution<float> dist(mu, sigma);
        std::mt19937 gen(rd());
        return dist(gen);
    }

    /*
     * Normal distribution. mu is the mean, and sigma is the standard deviation.
     */

    inline float normalvariate(float mu, float sigma)
    {
        float z;
        while (true)
        {
            float u1 = random();
            float u2 = 1.0f - random();
            z = NV_MAGICCONST * (u1 - 0.5f) / u2;
            float zz = z * z / 4.0f;
            if (zz <= -std::log(u2))
            {
                break;
            }
        }
        return mu + z * sigma;
    }

    /*
     * Circular data distribution. mu is the mean angle, expressed in radians between 0 and 2*pi,
     *   and kappa is the concentration parameter, which must be greater than or equal to zero.
     *   If kappa is equal to zero, this distribution reduces to a uniform random angle over the range 0 to 2*pi.
     */

    inline float vonmisesvariate(float mu, float kappa)
    {
        if (kappa <= 1e-6)
        {
            return 2.0f * M_PI * random();
        }

        float a = 1.0f + std::sqrt(1.0f + 4.0f * kappa * kappa);
        float b = (a - std::sqrt(2.0f * a)) / (2.0f * kappa);
        float r = (1.0f + b * b) / (2.0f * b);

        float f;
        while (true)
        {
            float u1 = random();

            float z = std::cos(M_PI * u1);
            f = (1.0f + r * z) / (r + z);
            float c = kappa * (r - f);
            float u2 = random();

            if (u2 < c * (2.0f - c) || u2 <= c * std::exp(1.0f - c))
            {
                break;
            }
        }

        float u3 = random();
        float theta;
        if (u3 > 0.5f)
        {
            theta = std::fmod(mu, 2.0f * M_PI) + std::acos(f);
        }
        else
        {
            theta = std::fmod(mu, 2.0f * M_PI) - std::acos(f);
        }
        return theta;
    }

    /*
     * Pareto distribution. alpha is the shape parameter.
     */

    inline float paretovariate(float alpha)
    {
        float u = 1.0f - random();
        return 1.0f / std::pow(u, (1.0f / alpha));
    }

    /*
     * Weibull distribution. alpha is the scale parameter and beta is the shape parameter.
     */

    inline float weibullvariate(float alpha, float beta)
    {
        float u = 1.0f - random();
        return alpha * std::pow(-std::log(u), 1.0f / beta);
    }

    // Other functions
    /*
     * Return value has a <probability_> chance of being true
     */
    inline bool probability(float probability_)
    {
        float r = uniform(0.0f, 1.0f);
        return r <= probability_;
    }

    /*
     * Return a k length list of unique (or not) elements chosen from the range.
     */
    inline std::vector<int> sample(int a, int b, unsigned k, bool unique = false)
    {
        if (unique && (b - a) < k)
        {
            throw std::range_error("random vector unique but range is less than count");
        }

        std::vector<int> rand_is;
        while (rand_is.size() < k)
        {
            int rand_i = randint(a, b);
            if (!unique || std::find(rand_is.begin(), rand_is.end(), rand_i) == rand_is.end())
            {
                rand_is.push_back(rand_i);
            }
        }
        return rand_is;
    }

} // namespace randomcpp
