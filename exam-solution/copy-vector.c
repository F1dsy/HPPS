// Test vector IO functions.

#include "matlib.h"
#include <stdio.h>
#include <stdlib.h>

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

  if (write_vector(argv[2], x) != 0) {
    fprintf(stderr, "Could not write vector to %s\n", argv[2]);
    exit(1);
  }
  free_vector(x);
}
