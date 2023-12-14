#include "kdtree.h"
#include "sort.h"
#include "util.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <string.h>

struct node
{
  // Index of this node's point in the corresponding 'indexes' array.
  int point_index;

  // Axis along which this node has been splot.
  int axis;

  // The left child of the node; NULL if none.
  struct node *left;

  // The right child of the node; NULL if none.
  struct node *right;
};

struct kdtree
{
  int d;
  const double *points;
  struct node *root;
};

struct sortAux
{
  int d;
  int axis;
  const double *points;
};

int compare(const void *a, const void *b, void *aux)
{
  // Cast a to int pointer and dereference.
  int i = *(const int *)a;
  // Cast b to int pointer and dereference.
  int j = *(const int *)b;
  // Cast aux to struct
  struct sortAux *sortAux = aux;
  const double *points = sortAux->points;
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

// I need to implement this
struct node *kdtree_create_node(int d, const double *points,
                                int depth, int n, int *indexes)
{
  // printf("n: %d\n", n);
  struct node *node = malloc(sizeof(struct node));
  int axis = depth % d;
  node->axis = axis;
  if (n == 0)
  {
    return NULL;
  }

  int left_indexes_n = n / 2;
  int right_indexes_n = n % 2 == 1 ? n / 2 : n / 2 - 1;

  // Sort indexes
  struct sortAux sortAux = {.d = d, .axis = axis, .points = points};
  hpps_quicksort(
      indexes, n, sizeof(int), compare,
      &sortAux);

  int median = indexes[n / 2];
  int left_indexes[left_indexes_n];
  int right_indexes[right_indexes_n];

  memcpy(left_indexes, indexes, sizeof(int) * left_indexes_n);
  memcpy(right_indexes, indexes + right_indexes_n, sizeof(int) * right_indexes_n);

  node->point_index = median;
  node->left = kdtree_create_node(d, points, depth + 1, left_indexes_n, left_indexes);
  node->right = kdtree_create_node(d, points, depth + 1, right_indexes_n, right_indexes);
  return node;
}

struct kdtree *kdtree_create(int d, int n, const double *points)
{
  struct kdtree *tree = malloc(sizeof(struct kdtree));
  tree->d = d;
  tree->points = points;

  int *indexes = malloc(sizeof(int) * n);

  for (int i = 0; i < n; i++)
  {
    indexes[i] = i;
  }

  tree->root = kdtree_create_node(d, points, 0, n, indexes);

  free(indexes);

  return tree;
}

// I need to implement this
void kdtree_free_node(struct node *node)
{
  if (node->left != NULL)
  {
    kdtree_free_node(node->left);
  }
  if (node->right != NULL)
  {
    kdtree_free_node(node->right);
  }
  free(node);
}

void kdtree_free(struct kdtree *tree)
{
  kdtree_free_node(tree->root);
  free(tree);
}

// I need to implement this
void kdtree_knn_node(const struct kdtree *tree, int k, const double *query,
                     int *closest, double *radius,
                     const struct node *node)
{
  // Update Closest
  if (node == NULL)
  {
    return;
  }

  insert_if_closer(k, tree->d, tree->points, closest, query, node->point_index);

  // Calculate diff and radius
  double node_point_value = tree->points[node->point_index + node->axis];
  double diff = node_point_value - query[node->axis];

  for (int i = k; 0 < i; i--)
  {
    if (closest[i] == -1)
    {
      continue;
    }
    *radius = distance(tree->d, &tree->points[closest[i] + node->axis], &query[node->axis]);
    break;
  }

  // Recursively search the appropriate subtree
  if (diff >= 0 || *radius > fabs(diff))
  {
    kdtree_knn_node(tree, k, query, closest, radius, node->left);
  }
  if (diff <= 0 || *radius > fabs(diff))
  {
    kdtree_knn_node(tree, k, query, closest, radius, node->right);
  }
}

int *kdtree_knn(const struct kdtree *tree, int k, const double *query)
{
  int *closest = malloc(k * sizeof(int));
  double radius = INFINITY;

  for (int i = 0; i < k; i++)
  {
    closest[i] = -1;
  }

  kdtree_knn_node(tree, k, query, closest, &radius, tree->root);

  return closest;
}

void kdtree_svg_node(double scale, FILE *f, const struct kdtree *tree,
                     double x1, double y1, double x2, double y2,
                     const struct node *node)
{
  if (node == NULL)
  {
    return;
  }

  double coord = tree->points[node->point_index * 2 + node->axis];
  if (node->axis == 0)
  {
    // Split the X axis, so vertical line.
    fprintf(f, "<line x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" stroke-width=\"1\" stroke=\"black\" />\n",
            coord * scale, y1 * scale, coord * scale, y2 * scale);
    kdtree_svg_node(scale, f, tree,
                    x1, y1, coord, y2,
                    node->left);
    kdtree_svg_node(scale, f, tree,
                    coord, y1, x2, y2,
                    node->right);
  }
  else
  {
    // Split the Y axis, so horizontal line.
    fprintf(f, "<line x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" stroke-width=\"1\" stroke=\"black\" />\n",
            x1 * scale, coord * scale, x2 * scale, coord * scale);
    kdtree_svg_node(scale, f, tree,
                    x1, y1, x2, coord,
                    node->left);
    kdtree_svg_node(scale, f, tree,
                    x1, coord, x2, y2,
                    node->right);
  }
}

void kdtree_svg(double scale, FILE *f, const struct kdtree *tree)
{
  assert(tree->d == 2);
  kdtree_svg_node(scale, f, tree, 0, 0, 1, 1, tree->root);
}
