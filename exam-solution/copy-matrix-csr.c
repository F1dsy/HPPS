// Test CSR matrix IO functions.

#include "matlib.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s XFILE YFILE\n", argv[0]);
    exit(1);
  }

  struct matrix_csr* X = read_matrix_csr(argv[1]);
  if (X == NULL) {
    fprintf(stderr, "Cannot read matrix from %s\n", argv[1]);
    exit(1);
  }


  if (write_matrix_csr(argv[2], X) != 0) {
    fprintf(stderr, "Cannot write matrix to %s\n", argv[2]);
    exit(1);
  }

  free_matrix_csr(X);
}
