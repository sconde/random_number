#include <array>
#include <cstddef>
#include <fstream>
#include <iostream>

#include "rng.h"

#define N (1 << 10)
int main() {
  RNG rng;
  std::array<double, N> rng01;
  std::array<double, N> rng11;

  std::cout << "In main..." << std::endl;
  std::ofstream myfile("example.txt", std::ios::out);

  for (size_t i = 0; i < N; i++) {
    rng01[i] = rng.u01();
    rng11[i] = rng.u11();
  }

  for (int i = 0; i < N; ++i) {
    myfile << rng01[i] << "   " << rng11[i] << std::endl;
  }

  myfile.close();
  std::cout << "...leaving main!!!" << std::endl;
  return 0;
}  // main