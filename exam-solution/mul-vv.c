#include "matlib.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  if (argc != 4) {
    fprintf(stderr, "Usage: %s ZFILE XFILE YFILE\n", argv[0]);
    exit(1);
  }

  struct vector* x = read_vector(argv[2]);
  if (x == NULL) {
    fprintf(stderr, "Cannot read vector from %s\n", argv[2]);
    exit(1);
  }

  struct vector* y = read_vector(argv[3]);
  if (y == NULL) {
    fprintf(stderr, "Cannot read vector from %s\n", argv[3]);
    exit(1);
  }

  struct scalar* z = mul_vv(x, y);
  free_vector(x);
  free_vector(y);
  write_scalar(argv[1], z);
  free_scalar(z);
}
