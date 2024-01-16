#include "matlib.h"
#include "timing.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(int argc, char** argv) {
  if (argc != 4) {
    fprintf(stderr, "Usage: %s XFILE YFILE RUNS\n", argv[0]);
    exit(1);
  }

  struct matrix_dense* X = read_matrix_dense(argv[1]);
  if (X == NULL) {
    fprintf(stderr, "Cannot read matrix from %s: %s\n", argv[1], strerror(errno));
    exit(1);
  }

  struct vector* y = read_vector(argv[2]);
  if (y == NULL) {
    fprintf(stderr, "Cannot read vector from %s: %s\n", argv[2], strerror(errno));
    exit(1);
  }

  int runs = atoi(argv[3]);
  uint64_t bef = microseconds();
  for (int r = 0; r < runs; r++) {
    struct vector* z = mul_mTv(X, y);
    free_vector(z);
  }
  uint64_t aft = microseconds();
  printf("Mean seconds: %f\n", (double)(aft-bef)/runs/1000000);
  free_matrix_dense(X);
  free_vector(y);
}
