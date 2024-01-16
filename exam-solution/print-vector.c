#include "matlib.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s FILE\n", argv[0]);
    exit(1);
  }

  const char* file = argv[1];

  struct vector* v = read_vector(file);

  if (v == NULL) {
    fprintf(stderr, "Failed to read vector from %s\n", file);
    exit(1);
  }

  int n = vector_n(v);

  for (int i = 0; i < n; i++) {
    printf("%f\n", vector_idx(v, i));
  }
  free_vector(v);
}
