#include "entitymap.h"

#include <stdlib.h>

#define MIN_CAPACITY 2

struct EntityMap
{
    void **arr;
    unsigned int max;      /* 1 + maximum key */
    unsigned int capacity; /* heap size of arr / sizeof(*arr) */
    void *nil;

    /* invariant: max <= capacity && MIN_CAPACITY <= capacity */
};

static char nil_obj;
void *entitymap_nil = &nil_obj;

EntityMap *entitymap_new(void *nil)
{
    unsigned int i;
    EntityMap *emap;

    emap = malloc(sizeof(EntityMap));
    emap->arr = malloc(MIN_CAPACITY * sizeof(void *));
    emap->max = 0;
    emap->capacity = MIN_CAPACITY;
    emap->nil = nil;

    for (i = 0; i < MIN_CAPACITY; ++i)
        emap->arr[i] = nil;

    return emap;
}
void entitymap_free(EntityMap *emap)
{
    free(emap->arr);
    free(emap);
}

static void _grow(EntityMap *emap)
{
    unsigned int new_capacity, i, max;

    /* find next power of 2 (TODO: use log?) */
    max = emap->max;
    for (new_capacity = emap->capacity; new_capacity < max;
            new_capacity <<= 1);

    /* grow, clear new */
    emap->arr = realloc(emap->arr, new_capacity * sizeof(void *));
    for (i = emap->capacity; i < new_capacity; ++i)
        emap->arr[i] = emap->nil;
    emap->capacity = new_capacity;
}
static void _shrink(EntityMap *emap)
{
    unsigned int new_capacity, max_times_4;

    if (emap->capacity <= MIN_CAPACITY)
        return;

    /* halve capacity while max is less than a fourth */
    max_times_4 = emap->max << 2;
    if (max_times_4 >= emap->capacity)
        return;
    for (new_capacity = emap->capacity;
            new_capacity > MIN_CAPACITY && max_times_4 < new_capacity;
            new_capacity >>= 1);
    if (new_capacity < MIN_CAPACITY)
        new_capacity = MIN_CAPACITY;

    emap->arr = realloc(emap->arr, new_capacity * sizeof(void *));
    emap->capacity = new_capacity;
}

void entitymap_set(EntityMap *emap, Entity ent, void *val)
{
    if (val == emap->nil) /* deleting? */
    {
        emap->arr[ent] = val;

        /* possibly move max down and shrink */
        if (emap->max == ent + 1)
        {
            while (emap->max > 0 && emap->arr[emap->max - 1] == emap->nil)
                --emap->max;
            _shrink(emap);
        }
    }
    else
    {
        /* possibly move max up and grow */
        if (ent + 1 > emap->max)
        {
            emap->max = ent + 1;
            if (ent >= emap->capacity)
                _grow(emap);
        }

        emap->arr[ent] = val;
    }
}
void *entitymap_get(EntityMap *emap, Entity ent)
{
    if (ent >= emap->capacity)
        return emap->nil;
    return emap->arr[ent];
}

Entity entitymap_get_max(EntityMap *emap)
{
    return emap->max;
}

