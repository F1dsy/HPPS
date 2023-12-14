#include "bruteforce.h"
#include "util.h"
#include <stdlib.h>
#include <assert.h>

int *knn(int k, int d, int n, const double *points, const double *query)
{

  int *closest_indicies = malloc(sizeof(int) * k);

  for (int i = 0; i < k; i++)
  {
    closest_indicies[i] = -1;
  }

  for (int i = 0; i < n; i++)
  {
    insert_if_closer(k, d, points, closest_indicies, query, i);
  }
  return closest_indicies;
}