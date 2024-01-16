#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <sys/time.h>
#include "util.h"

double seconds(void)
{
  struct timeval tv;
  gettimeofday(&tv, NULL); // The NULL is for timezone information.
  return tv.tv_sec + tv.tv_usec / 1000000.0;
}

double dist(struct vec3 a, struct vec3 b)
{
  double dx = a.x - b.x;
  double dy = a.y - b.y;
  double dz = a.z - b.z;

  return sqrt(dx * dx + dy * dy + dz * dz);
}

struct vec3 force(struct vec3 pi, struct vec3 pj, double mj)
{
  static const double epsilon = 1;

  double d = dist(pi, pj);
  double d_sqr = d * d;
  double inv_d = 1.0 / sqrt(d_sqr + epsilon * epsilon);
  double inv_d3 = inv_d * inv_d * inv_d;

  struct vec3 f;
  f.x = (pj.x - pi.x) * mj * inv_d3;
  f.y = (pj.y - pi.y) * mj * inv_d3;
  f.z = (pj.z - pi.z) * mj * inv_d3;
  return f;
}

struct particle *read_particles(const char *fname, int32_t *n)
{
  // Open the file
  FILE *file = fopen(fname, "r");
  assert(file != NULL);

  // Read the first int32_t to the the number of particles in file
  int32_t read = fread(n, sizeof(int32_t), 1, file);
  assert(read == 1);

  // Allocate memory for particles
  struct particle *data = malloc(*n * sizeof(struct particle));

  // Read particles from file
  read = fread(data, sizeof(struct particle), *n, file);
  assert(read == *n);
  fclose(file);
  return data;
}

void write_particles(const char *fname, int32_t n, struct particle *ps)
{
  // Open the file
  FILE *file = fopen(fname, "w");
  assert(file != NULL);

  // Write the first int32_t to the the number of particles in file
  int32_t write = fwrite(&n, sizeof(int32_t), 1, file);
  assert(write == 1);

  // Write particles from file
  write = fwrite(ps, sizeof(struct particle), n, file);

  assert(write == n);

  fclose(file);
}
