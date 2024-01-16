#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include "timing.h"

int main(int argc, char** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s FILE\n", argv[0]);
    exit(1);
  }

  FILE* f = fopen(argv[1], "wb");
  if (f == NULL) {
    fprintf(stderr, "Cannot open %s: %s\n", argv[1], strerror(errno));
    exit(1);
  }
  srand(microseconds()); // Initialise RNG.

  fprintf(f, "SCLR");
  double v = rand() / ((double)RAND_MAX);
  assert(fwrite(&v, sizeof(double), 1, f) == 1);
  fclose(f);
}
