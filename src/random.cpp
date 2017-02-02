#include <randomcpp/random.hpp>

#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <stdexcept>

/*static*/ unsigned random::seed_value = 0;
/*static*/ std::random_device random::rd;

void random::seed() {
   seed(static_cast<unsigned>(std::time(0)));
}

void random::seed(unsigned a) {
   random::seed_value = a;
   initialize();
}

void random::reset() {
   initialize();
}

int _randbelow(int n) {
   return std::rand() % n;
}

int random::randrange(int stop) {
   return randrange(0, stop);
}

int random::randrange(int start, int stop, int step /*=1*/) {
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

int random::randint(int a, int b) {
   return randrange(a, b+1);
}

bool random::probability(float probability_) {
   float r = uniform(0.0f, 1.0f);
   return r <= probability_;
}

float random::uniform(float a, float b) {
   if (b < a) {
      throw std::runtime_error("Invalid range");
   }
   float range = b - a;
   float r = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
   return r * range - a;
}

float random::gauss(float mu, float sigma) {
   std::normal_distribution<float> dist(mu, sigma);
   // Should gen be here?
   std::mt19937 gen(rd());
   return dist(gen);
}

std::vector<int> random::sample(int a, int b, unsigned k, bool unique) {
   if (unique && (b - a) < k) {
      throw std::runtime_error("random vector unique but range is less than count");
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

/*static*/ void random::initialize() {
   std::srand(seed_value);
}
