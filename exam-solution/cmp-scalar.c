// Compare scalars with tolerance.

#include "matlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char** argv) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s XFILE YFILE\n", argv[0]);
    exit(1);
  }

  struct scalar* x = read_scalar(argv[1]);
  if (x == NULL) {
    fprintf(stderr, "Cannot read scalar from %s\n", argv[1]);
    exit(1);
  }

  struct scalar* y = read_scalar(argv[2]);
  if (y == NULL) {
    fprintf(stderr, "Cannot read scalar from %s\n", argv[2]);
    exit(1);
  }

  double a = scalar_value(x);
  double b = scalar_value(y);
  if (fabs(a - b)/fmin(a,b) > 0.01) {
    fprintf(stderr, "Mismatch: %f != %f\n",
            a, b);
    return 1;
  }

  free_scalar(x);
  free_scalar(y);
}
