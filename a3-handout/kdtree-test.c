#include "kdtree.h"
#include "io.h"
#include "assert.h"
#include <stdlib.h>

// function for testing if kdtree_create makes the right kdtree and if kdtree_knn works as expected

int main(int argc, char **argv)
{
    assert(argc == 2);

    FILE *points_f = fopen(argv[1], "r");
    assert(points_f != NULL);

    int n_points = -1;
    int d;
    double *points = read_points(points_f, &n_points, &d);

    fclose(points_f);

    printf("Success\n");
    struct kdtree *tree = kdtree_create(d, n_points, points);
    printf("Success\n");

    kdtree_svg(100.0, fopen("kdtree-test-svg.svg", "w"), tree);

    // int *results = kdtree_knn(tree, 2, queries[0]);
    // for (int i = 0; i < 2; i++)
    // {
    // printf("result index: %d\n", results[i]);
    // }

    return 0;
}
