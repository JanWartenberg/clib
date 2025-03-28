#include <stdlib.h>

#include "rand.h"

// Returns random int between 0 and range_max.
int rand_int(int range_max) {
  if (range_max <= 0) {
    return 0; // Or handle the error as appropriate
  }

  // Ensure range_max is positive before calculation
  unsigned int scaled_rand =
      (unsigned int)(((double)rand() / RAND_MAX) * range_max);
  return (int)scaled_rand;
}

// Returns random int between range_min and range_max.
int rand_int_minmax(int range_min, int range_max) {
  return range_min + rand() % (range_max - range_min + 1);
}

// Returns random int between max/10 and max - 1,
//  e.g. if max is 100, it returns between 10 and 99,
// and if max is 1000, it returns between 100 and 999.
int rand_int_in_upper_range(int max) {
  int lower = max / 10; // For max=1000, lower will be 100.
  int upper = max;      // Upper-bound is max itself.
  return rand_int_minmax(lower, upper);
}

int rand_int_in_upper_half(int max) {
  int lower = max / 2;
  int upper = max;
  return rand_int_minmax(lower, upper);
}
