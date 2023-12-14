// This program is an example of how to use hpps_quicksort() from
// sort.h.
//
// We sort the strings containing in the argv array provided to
// main().  However, instead of creating an sorting the strings
// directly, we sort an array of *indexes*, where each index
// identifies a string in argv.  This shows how we can look up
// "auxiliary" information inside the sorting function.
//
// Usage:
//
// $ ./sort-example troels hpps diku
// ./sort-example
// diku
// hpps
// troels

#include <string.h>
#include <stdio.h>
#include "sort.h"

// int cmp_strings(const void *x, const void *y, void *aux)
// {
//   // Cast x to int pointer and dereference.
//   int i = *(const int *)x;
//   // Cast y to int pointer and dereference.
//   int j = *(const int *)y;
//   // Cast aux to char* pointer.
//   char **argv = aux;
//   // Treat i and j as indexes into argv, fetch the strings, and use
//   // strcmp to compare them.
//   return strcmp(argv[i], argv[j]);
// }

// int main(int argc, char **argv)
// {
//   int indexes[argc];

//   for (int i = 0; i < argc; i++)
//   {
//     indexes[i] = i;
//   }

//   hpps_quicksort(indexes, argc, sizeof(int), cmp_strings, argv);

//   for (int i = 0; i < argc; i++)
//   {
//     puts(argv[indexes[i]]);
//   }
// }

struct sortAux
{
  int d;
  int axis;
  double *points;
};

int compare(const void *a, const void *b, void *aux)
{
  // Cast a to int pointer and dereference.
  int i = *(const int *)a;
  // Cast b to int pointer and dereference.
  int j = *(const int *)b;
  // Cast aux to struct
  struct sortAux *sortAux = aux;
  double *points = sortAux->points;
  int axis = sortAux->axis;
  int d = sortAux->d;

  double first_point = points[d * i + axis];
  double second_point = points[d * j + axis];

  if (first_point < second_point)
  {
    return -1;
  }
  else if (first_point > second_point)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

int main()
{
  const int N = 5;
  int d = 2;
  int axis = 0;
  int indexes[5];
  double points[10] = {1.0, 1.0, 4.0, 4.0, 2.0, 2.0, 3.0, 3.0, 5.0, 5.0};
  for (int i = 0; i < N; i++)
  {
    indexes[i] = i;
  }

  for (int i = 0; i < N; i++)
  {
    printf("Index %d and corresponding value is: ", indexes[i]);
    for (int j = 0; j < d; j++)
    {
      printf("%f ", points[d * indexes[i] + j]);
    }
    printf("\n");
  }

  printf("After Sort \n");
  struct sortAux sortAux = {.d = d, .axis = axis, .points = points};
  hpps_quicksort(indexes, N, sizeof(int), compare, &sortAux);

  for (int i = 0; i < N; i++)
  {
    printf("Index %d and corresponding value is: ", indexes[i]);
    for (int j = 0; j < d; j++)
    {
      printf("%f ", points[d * indexes[i] + j]);
    }
    printf("\n");
  }
  return 0;
}
