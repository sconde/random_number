#include <algorithm>
#include <array>
#include <cstddef>
#include <fstream>
#include <iostream>

#include "rng.h"

#define FILL_TYPE 1

#define N (1 << 10)
int main() {
  RNG rng;
  std::array<double, N> rng01{};
  std::array<double, N> rng11{};

  std::cout << "In main..." << std::endl;
  std::ofstream myfile("example.txt", std::ios::out);

#if FILL_TYPE == 0
  for (size_t i = 0; i < N; i++) {
    rng01[i] = rng.u01();
    rng11[i] = rng.u11();
  }
#elif FILL_TYPE == 1
  // Assuming rng01 and rng11 are some kind of containers that support iterators
  std::generate(rng01.begin(), rng01.end(), [&]() { return rng.u01(); });
  std::generate(rng11.begin(), rng11.end(), [&]() { return rng.u11(); });
#endif

  for (int i = 0; i < N; ++i) {
    myfile << rng01[i] << "   " << rng11[i] << "\n";
  }

  myfile.close();
  std::cout << "...leaving main!!!" << "\n";
  return 0;
}  // main