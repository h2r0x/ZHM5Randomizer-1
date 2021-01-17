#include "RNG.h"

namespace hitman_randomizer {

RNG::RNG() : rng(std::random_device{}()) {}

RNG& RNG::inst() {
  static RNG rng;
  return rng;
}

void RNG::seed(uint32_t seed) { rng.seed(seed); }

std::mt19937* RNG::getEngine() { return &rng; }

}  // namespace hitman_randomizer