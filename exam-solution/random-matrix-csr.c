#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include "timing.h"

int main(int argc, char** argv) {
  if (argc != 6) {
    fprintf(stderr, "Usage: %s ROWS COLUMNS DENSITY1 DENSITY2 FILE\n", argv[0]);
    return 1;
  }

  int rows = atoi(argv[1]);
  int cols = atoi(argv[2]);
  double density1 = atof(argv[3]);
  double density2 = atof(argv[4]);
  FILE *f = fopen(argv[5], "wb");
  if (f == NULL) {
    fprintf(stderr, "Cannot open %s: %s\n", argv[1], strerror(errno));
    exit(1);
  }
  srand(microseconds()); // Initialise RNG.

  // We do this by generating a dense matrix in memory and then write
  // only the nonzero elements.  This means we cannot construct truly
  // astronomically large matrices, but it's probably fine.
  double *elements = calloc(rows*cols, sizeof(double));

  int nnz = 0;
  for (int i = 0; i < rows; i++) {
    double density = density1 + (i/((double)rows-1))*(density2-density1);
    for (int j = 0; j < cols; j++) {
      if ((rand() / ((double)RAND_MAX)) < density) {
        elements[i*cols+j] = (rand() / ((double)RAND_MAX));
        nnz++;
      } else {
        elements[i*cols+j] = 0;
      }
    }
  }
  // Store nonzero elements.
  double *v = calloc(nnz, sizeof(double));
  int *v_col = calloc(nnz, sizeof(int));
  int *row_start = calloc((unsigned)rows, sizeof(int));
  int p = 0;
  for (int i = 0; i < rows; i++) {
    row_start[i] = p;
    for (int j = 0; j < cols; j++) {
      if (elements[i*cols+j] != 0) {
        v[p] = elements[i*cols+j];
        v_col[p] = j;
        p++;
      }
    }
  }

  fprintf(f, "MCSR");
  assert(fwrite(&rows, sizeof(int), 1, f) == 1);
  assert(fwrite(&cols, sizeof(int), 1, f) == 1);
  assert(fwrite(&nnz, sizeof(int), 1, f) == 1);
  assert(fwrite(v, sizeof(double), nnz, f) == (size_t)nnz);
  assert(fwrite(v_col, sizeof(int), nnz, f) == (size_t)nnz);
  assert(fwrite(row_start, sizeof(int), rows, f) == (size_t)rows);

  free(elements);
  free(v);
  free(v_col);
  free(row_start);
}
