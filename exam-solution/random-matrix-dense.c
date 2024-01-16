#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include "timing.h"

int main(int argc, char** argv) {
  if (argc != 4) {
    fprintf(stderr, "Usage: %s ROWS COLUMNS FILE\n", argv[0]);
    return 1;
  }

  int rows = atoi(argv[1]);
  int columns = atoi(argv[2]);
  FILE *f = fopen(argv[3], "wb");
  if (f == NULL) {
    fprintf(stderr, "Cannot open %s: %s\n", argv[1], strerror(errno));
    exit(1);
  }
  srand(microseconds()); // Initialise RNG.

  fprintf(f, "MDNS");
  assert(fwrite(&rows, sizeof(int), 1, f) == 1);
  assert(fwrite(&columns, sizeof(int), 1, f) == 1);
  for (int i = 0; i < rows*columns; i++) {
    double x = rand() / ((double)RAND_MAX);
    assert(fwrite(&x, sizeof(double), 1, f) == 1);
  }
}
