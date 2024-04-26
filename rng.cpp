#include "rng.h"

#include <math.h>
#include <omp.h>

#include <cstdlib>
#include <ctime>

RNG::RNG() : num_threads(omp_get_max_threads()) {
  unsigned int* seeds = new unsigned int[num_threads];

  srand(time(NULL));

  for (int i = 0; i < num_threads; ++i) {
    seeds[i] = arc4random();
  }

  init(seeds);
}

RNG::RNG(unsigned int* seeds) : num_threads(omp_get_max_threads()) {
  init(seeds);
}

void RNG::init(unsigned int* seeds) {
  for (int i = 0; i < RNG_BLOCKS; ++i) data[i] = new double[num_threads];
  next_block = new int[num_threads];
  have_spare_gaussian = new bool[num_threads];
  gaussians[0] = new double[num_threads];
  gaussians[1] = new double[num_threads];
  for (int i = 0; i < RNG_STATE_SIZE; ++i)
    state[i] = new unsigned int[num_threads];

  for (int t = 0; t < num_threads; ++t) {
    have_spare_gaussian[t] = false;
    state[0][t] = seeds[t];
    for (int i = 1; i < RNG_STATE_SIZE; ++i) {
      state[i][t] =
          (int)(1812433253 * (state[i - 1][t] ^ (state[i - 1][t] >> 30)) + i);
    }

    make_random(t);
  }
}

// uniform random number between 0 and 1
double RNG::u01() {
  int t = omp_get_thread_num();
  if (RNG_BLOCKS == next_block[t]) make_random();
  return data[next_block[t]++][t];
}

// uniform random number between -1 and 1
double RNG::u11() {
  int t = omp_get_thread_num();
  if (RNG_BLOCKS == next_block[t]) make_random();
  return data[next_block[t]++][t] * 2 - 1;
}

// Gaussian random number with zero mean and unit variance
double RNG::gaussian() {
  int t = omp_get_thread_num();
  if (have_spare_gaussian[t]) {
    have_spare_gaussian[t] = false;
    return sqrt(RNG_VARIANCE * gaussians[0][t]) * sin(gaussians[1][t]);
  }

  have_spare_gaussian[t] = true;

  gaussians[0][t] = u01();
  if (gaussians[0][t] < 1e-100) gaussians[0][t] = 1e-100;
  gaussians[0][t] = -2 * log(gaussians[0][t]);
  gaussians[1][t] = u01() * M_PI * 2;

  return sqrt(RNG_VARIANCE * gaussians[0][t]) * cos(gaussians[1][t]);
}

// Generates a new batch of random numbers, hopefully in parallel.
void RNG::make_random() { make_random(omp_get_thread_num()); }

void RNG::make_random(int t) {
  for (int n = 0; n < RNG_BLOCKS; ++n) {
    if (n % RNG_STATE_SIZE == 0) {
      for (int i = 0; i < RNG_STATE_SIZE; ++i) {
        int y = (state[i][t] & 0x80000000) +
                (state[(i + 1) % RNG_STATE_SIZE][t] & 0x7fffffff);
        state[i][t] = state[(i + 397) % RNG_STATE_SIZE][t] ^ (y >> 1);
        if (y % 2) {
          state[i][t] ^= 0x9908b0df;
        }
      }
    }

    unsigned int y = state[n % RNG_STATE_SIZE][t];
    y ^= (y >> 11);
    y ^= ((y << 7) & 0x9d2c5680);
    y ^= ((y << 15) & 0xefc60000);
    y ^= (y >> 18);

    data[n][t] = y / double((unsigned int)0xffffffff);
  }

  next_block[t] = 0;
}
