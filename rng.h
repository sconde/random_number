#ifndef RNG_H_
#define RNG_H_

#define RNG_BLOCKS (1 << 12)
constexpr int RNG_STATE_SIZE = 624;

static constexpr double RNG_VARIANCE = 1;
static constexpr double SQRT3 = 1.7320508075688773;

struct RNG {
 public:
  RNG(unsigned int* seeds);
  RNG();

  double u01();       // uniform random number between 0 and 1
  double u11();       // uniform random number between -1 and 1
  double gaussian();  // Gaussian random number with zero mean and unit variance

 private:
  void init(unsigned int* seeds);

  void make_random();            // Generates a new batch of random numbers.
  void make_random(int thread);  // Generates a new batch of random numbers.

  double* data[RNG_BLOCKS];
  int* next_block;

  bool* have_spare_gaussian;
  double* gaussians[2];

  unsigned int* state[RNG_STATE_SIZE];

  const int num_threads;
};

#endif /* RNG_H_ */
