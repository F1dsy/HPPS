#include "util.h"
#include <math.h>
#include <stdio.h>
#include <assert.h>

double distance(int d, const double *x, const double *y)
{
  double sum = 0;
  for (int i = 0; i < d; i++)
  {
    double diff = x[i] - y[i];
    sum += diff * diff;
  }
  return sqrt(sum);
}

int insert_if_closer(int k, int d,
                     const double *points, int *closest, const double *query,
                     int candidate)
{
  for (int i = 0; i < k; i++)
  {
    if (closest[i] == -1)
    {
      closest[i] = candidate;
      return 1;
    }
    double candidate_distance = distance(d, &points[candidate * d], query);
    double current_closest_distance = distance(d, &points[closest[i] * d], query);
    if (candidate_distance < current_closest_distance)
    {

      for (int j = k - 1; j > i; j--)
      {

        closest[j] = closest[j - 1];
      }
      closest[i] = candidate;
      return 1;
    }
  }
  return 0;
}
