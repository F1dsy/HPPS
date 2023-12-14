#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <stdint.h>
#include <errno.h>
#include <assert.h>
#include <math.h>

#include "record.h"
#include "coord_query.h"

struct naive_data
{
  struct record *rs;
  int n;
};

double euclidean_distance(double long1, double lat1, double long2, double lat2)
{
  double lat_diff = pow(lat1 - lat2, 2);
  double long_diff = pow(long1 - long2, 2);
  return sqrt(lat_diff + long_diff);
}

struct naive_data *mk_naive(struct record *rs, int n)
{
  // TODO
  struct naive_data *naive_data = malloc(sizeof(struct naive_data));
  naive_data->rs = rs;
  naive_data->n = n;
  return naive_data;
}

void free_naive(struct naive_data *data)
{
  // TODO
  free(data);
}

const struct record *lookup_naive(struct naive_data *data, double lon, double lat)
{
  // TODO
  struct record *closest_record;
  double closest_record_distance;
  for (int i = 0; i < data->n; i++)
  {
    double current_distance = euclidean_distance(lon, lat, data->rs[i].lon, data->rs[i].lat);
    if (current_distance < closest_record_distance)
    {
      closest_record = &data->rs[i];
    }
  }
}

int main(int argc, char **argv)
{
  return coord_query_loop(argc, argv,
                          (mk_index_fn)mk_naive,
                          (free_index_fn)free_naive,
                          (lookup_fn)lookup_naive);
}
