// Compare vectors with tolerance.

#include "matlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char** argv) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s XFILE YFILE\n", argv[0]);
    exit(1);
  }

  struct vector* x = read_vector(argv[1]);
  if (x == NULL) {
    fprintf(stderr, "Cannot read vector from %s\n", argv[1]);
    exit(1);
  }

  struct vector* y = read_vector(argv[2]);
  if (y == NULL) {
    fprintf(stderr, "Cannot read vector from %s\n", argv[2]);
    exit(1);
  }

  int n = vector_n(x);
  int m = vector_n(x);

  if (n != m) {
    fprintf(stderr, "%d != %d\n", n, m);
    exit(1);
  }

  for (int i = 0; i < n; i++) {
    double a = vector_idx(x,i);
    double b = vector_idx(y,i);
    if (fabs(a - b)/fmin(a,b) > 0.01) {
      fprintf(stderr, "Mismatch at %d: %f != %f\n",
              i, a, b);
      return 1;
    }
  }

  free_vector(x);
  free_vector(y);
}
