#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <stdint.h>
#include <errno.h>
#include <assert.h>

#include "record.h"
#include "id_query.h"

int *compare(void *data1, void *data2)
{
    struct index_record *ir1 = (struct index_record *)data1;
    struct index_record *ir2 = (struct index_record *)data2;

    return (ir1->osm_id - ir2->osm_id);
}

void sort_by_osm_id(struct indexed_data *data)
{
    qsort(data->irs, data->n, sizeof(struct index_record), compare);
}

struct index_record
{
    int64_t osm_id;
    const struct record *record;
};

struct indexed_data
{
    struct index_record *irs;
    int n;
};

struct indexed_data *mk_indexed(struct record *rs, int n)
{
    struct indexed_data *indexed_data = malloc(sizeof(struct indexed_data));
    struct index_record *index_record = malloc(sizeof(struct index_record) * n);
    for (int i = 0; i < n; i++)
    {
        index_record->record = &rs[i];
        index_record->osm_id = rs[i].osm_id;
    }
    sort_by_osm_id(indexed_data);
    indexed_data->irs = index_record;
    indexed_data->n = n;
    return indexed_data;
}

void free_indexed(struct indexed_data *data)
{
}
const struct record *lookup_indexed(struct indexed_data *data, int64_t needle)
{
    return bsearch(needle, data->irs, data->n, sizeof(struct index_record), compare);
}

int main(int argc, char **argv)
{
    return id_query_loop(argc, argv,
                         (mk_index_fn)mk_indexed,
                         (free_index_fn)free_indexed,
                         (lookup_fn)lookup_indexed);
}
