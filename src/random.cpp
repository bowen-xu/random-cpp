#include <randomcpp.hpp>

#include <algorithm> // std::find
#include <ctime>     // std::time

namespace randomcpp {

static unsigned seed_value = 0;
static std::random_device rd;

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