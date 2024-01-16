#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "util.h"

void nbody(int n, struct particle *ps, int steps)
{
  for (int s = 0; s < steps; s++)
  {
#pragma omp parallel for
    for (int32_t i = 0; i < n; i++)
    {
      // Sum all the forces from all other particles on the i'th particle
      struct vec3 a = {0, 0, 0};
      for (int32_t j = 0; j < n; j++)
      {
        struct vec3 f = force(ps[i].pos, ps[j].pos, ps[j].mass);
        a.x += f.x;
        a.y += f.y;
        a.z += f.z;
      }
      // Update the velocity of the i'th particles with the new acceleration
      ps[i].vel.x += a.x;
      ps[i].vel.y += a.y;
      ps[i].vel.z += a.z;
    }
    // Update the position of all the particles with the new position
    for (int32_t i = 0; i < n; i++)
    {
      ps[i].pos.x += ps[i].vel.x;
      ps[i].pos.y += ps[i].vel.y;
      ps[i].pos.z += ps[i].vel.z;
    }
  }
}

int main(int argc, char **argv)
{
  int steps = 1;
  if (argc < 3)
  {
    printf("Usage: \n");
    printf("%s <input> <output> [steps]\n", argv[0]);
    return 1;
  }
  else if (argc > 3)
  {
    steps = atoi(argv[3]);
  }

  int32_t n;
  struct particle *ps = read_particles(argv[1], &n);

  double bef = seconds();
  nbody(n, ps, steps);
  double aft = seconds();
  printf("Time to simulate: %f\n", aft - bef);
  write_particles(argv[2], n, ps);
  free(ps);
}
