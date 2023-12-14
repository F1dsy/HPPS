#include <math.h>
#include <stdio.h>
#include "util.h"

// Test for distance
int main()
{

  // Defines the coordinates of the first point
  double x[3] = {1, 2, 3};

  // Defines the coordinates of the second point
  double y[3] = {4, 5, 6};

  // Calculates the Euclidean distance between the two points
  double sum = distance(3, x, y); // Corrected to match the function signature

  // Prints the distance to the console
  printf("The Euclidean distance between the points is %f\n", sum); // Corrected to print 'sum'

  // Test for insert_if_closer
  int k = 3; // We are only interested in the closest point to (0,0)
  int d = 2; // We use 2D for the simplicity

  double points[] = {5.0, 5.0, 1.0, 1.0, 3.0, 3.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0}; // Our points
  int closest[k];                                                                 // This array stores the index of the closest points
  for (int i = 0; i < k; i++)
  {
    closest[i] = -1;
  }

  double query[] = {0.0, 0.0}; // Position

  for (int i = 0; i < 6; i++)
  { // Testing each point
    insert_if_closer(k, d, points, closest, query, i);
    
  }
  for (int i = 0; i < k; i++)
  {
    printf("%d \n", closest[i]);
  }

  return 0;
}
