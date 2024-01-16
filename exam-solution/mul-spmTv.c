#include "matlib.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  if (argc != 4) {
    fprintf(stderr, "Usage: %s ZFILE XFILE YFILE\n", argv[0]);
    exit(1);
  }

  struct matrix_csr* X = read_matrix_csr(argv[2]);
  if (X == NULL) {
    fprintf(stderr, "Cannot read matrix from %s\n", argv[2]);
    exit(1);
  }

  struct vector* y = read_vector(argv[3]);
  if (y == NULL) {
    fprintf(stderr, "Cannot read vector from %s\n", argv[3]);
    exit(1);
  }

  struct vector* z = mul_spmTv(X, y);
  free_matrix_csr(X);
  free_vector(y);
  write_vector(argv[1], z);
  free_vector(z);
}
