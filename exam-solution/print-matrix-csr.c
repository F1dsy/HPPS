#include "matlib.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s FILE\n", argv[0]);
    exit(1);
  }

  const char* file = argv[1];

  struct matrix_csr* X = read_matrix_csr(file);

  if (X == NULL) {
    fprintf(stderr, "Failed to read CSR matrix from %s\n", file);
    exit(1);
  }

  int rows = matrix_csr_n(X);
  int cols = matrix_csr_m(X);

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (j != 0) {
        printf(" ");
      }
      printf("%f", matrix_csr_idx(X, i, j));
    }
    printf("\n");
  }
  free_matrix_csr(X);
}
