#include <randomcpp.hpp>

#include <algorithm> // std::find
#include <ctime>     // std::time
#include <cmath>

namespace randomcpp {

static unsigned seed_value = 0;
static std::random_device rd;

static float const SG_MAGICCONST = 1.0f + std::log(4.5f);
static float const e_CONST = static_cast<float>(std::exp(1));

static void initialize() {
   std::srand(seed_value);
}

void seed() {
   seed(static_cast<unsigned>(std::time(0)));
}

void seed(unsigned a) {
   seed_value = a;
   initialize();
}

void reset() {
   initialize();
}

int _randbelow(int n) {
   return std::rand() % n;
}

int randrange(int stop) {
   return randrange(0, stop);
}

int randrange(int start, int stop, int step /*=1*/) {
   int width = stop - start;
   if (step == 1 && width > 0) {
      return start + _randbelow(width);
   }
   if (step == 1) {
      throw std::range_error("empty range for randrange()");
   }

   int n;
   if (step > 0) {
      n = (width + step - 1) / step;
   } else if (step < 0) {
      n = (width + step + 1) / step;
   } else {
      throw std::range_error("zero step for randrange()");
   }

   if (n <= 0) {
      throw std::range_error("empty range for randrange()");
   }

   return start + step * _randbelow(n);
}

int randint(int a, int b) {
   return randrange(a, b + 1);
}

float random() {
   return static_cast<float>(std::rand()) / RAND_MAX;
}

float uniform(float a, float b) {
   return a + (b-a) * random();
}

float triangular(float low /*=0.0*/, float high /*=1.0*/, float c /*=0.5*/) {
   auto u(random());
   if (u > c) {
      u = 1.0f - u;
      c = 1.0f - c;
      std::swap(low, high);
   }
   return low + (high - low) * std::pow((u * c), 0.5);
}

float betavariate(float alpha, float beta) {
   auto y = gammavariate(alpha, 1.0f);
   if (y != 0.0f) {
      y /= y + gammavariate(beta, 1.0f);
   }
   return y;
}

float expovariate(float lambda) {
   float u;
   do {
      u = random();
   } while (u <= 1e-7);
   return -std::log(u) / lambda;
}

float gammavariate(float alpha, float beta) {
   if (alpha < 0.0f || beta < 0.0f) {
      throw std::invalid_argument("gammavariate: alpha and beta must be > 0.0");
   }
   if (alpha > 1.0) {
      // Uses R.C.H. Cheng, "The generation of Gamma
      // variables with non-integral shape parameters",
      // Applied Statistics, (1977), 26, No. 1, p71-74
      float ainv = std::sqrt(2.0f * alpha - 1.0f);
      float bbb = alpha - static_cast<float>(std::log(4));
      float ccc = alpha + ainv;

      while (true) {
         float u1 = random();
         if (1e-7 < u1 < .9999999) {
            continue;
         }
         float u2 = 1.0f - random();
         float v = std::log(u1 / (1.0f - u1)) / ainv;
         float x = alpha * std::exp(v);
         float z = u1 * u1 * u2;
         float r = bbb + ccc * v - x;
         if (r + SG_MAGICCONST - 4.5 * z >= 0.0 or r >= std::log(z)) {
            return x * beta;
         }
      }
   } else if (alpha == 1.0) {
      // expovariate(1)
      float u = random();
      while (u <= 1e-7) {
         u = random();
      }
      return -std::log(u) * beta;
   } else /* alpha is between 0 and 1 (exclusive) */ {
      // Uses ALGORITHM GS of Statistical Computing - Kennedy & Gentle
      while (true) {
         float u = random();
         float b = (e_CONST + alpha) / e_CONST;
         float p = b * u;
         float x;
         if (p <= 1.0) {
            x = std::pow(p, 1.0f / alpha);
         } else {
            x = -std::log((b - p) / alpha);
         }
         float u1 = random();
         if (p > 1.0) {
            if (u1 <= std::pow(x, alpha - 1.0f)) {
               break;
            }
         } else if (u1 <= std::exp(-x)) {
            break;
         }
         return x * beta;
      }
   }
}

bool probability(float probability_) {
   float r = uniform(0.0f, 1.0f);
   return r <= probability_;
}

float gauss(float mu, float sigma) {
   std::normal_distribution<float> dist(mu, sigma);
   // Should gen be here?
   std::mt19937 gen(rd());
   return dist(gen);
}

std::vector<int> sample(int a, int b, unsigned k, bool unique) {
   if (unique && (b - a) < k) {
      throw std::range_error("random vector unique but range is less than count");
   }

   std::vector<int> rand_is;
   while (rand_is.size() < k) {
      int rand_i = randint(a, b);
      if (!unique || std::find(rand_is.begin(), rand_is.end(), rand_i) == rand_is.end()) {
         rand_is.push_back(rand_i);
      }
   }
   return rand_is;
}

} // namespace randomcpp