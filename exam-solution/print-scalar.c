#include "matlib.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s FILE\n", argv[0]);
    exit(1);
  }

  const char* file = argv[1];

  struct scalar* s = read_scalar(file);

  if (s == NULL) {
    fprintf(stderr, "Failed to read scalar from %s\n", file);
    exit(1);
  }

  printf("%f\n", scalar_value(s));
  free_scalar(s);
}
