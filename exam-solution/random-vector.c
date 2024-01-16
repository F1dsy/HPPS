#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include "timing.h"

int main(int argc, char** argv) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s ELEMS FILE\n", argv[0]);
    exit(1);
  }

  int elems = atoi(argv[1]);

  FILE* f = fopen(argv[2], "wr");
  assert(f != NULL);
  if (f == NULL) {
    fprintf(stderr, "Cannot open %s: %s\n", argv[1], strerror(errno));
    exit(1);
  }
  srand(microseconds()); // Initialise RNG.

  fprintf(f, "VDNS");
  assert(fwrite(&elems, sizeof(int), 1, f) == 1);
  for (int i = 0; i < elems; i++) {
    double x = rand() / ((double)RAND_MAX);
    assert(fwrite(&x, sizeof(double), 1, f) == 1);
  }
  fclose(f);
}
