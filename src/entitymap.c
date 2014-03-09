#include "entitymap.h"

#include <stdlib.h>

#define MIN_CAPACITY 2

struct EntityMap
{
    int *arr;
    unsigned int bound;      /* 1 + maximum key */
    unsigned int capacity;   /* number of elements we have heap space for */
    int def;                 /* value for unset keys */

    /*
     * invariants:
     *    bound <= capacity (so that maximum key < capacity)
     *    MIN_CAPACITY <= capacity
     */
};

static void _init(EntityMap *emap)
{
    unsigned int i;

    emap->bound = 0;
    emap->capacity = MIN_CAPACITY;
    emap->arr = malloc(emap->capacity * sizeof(*emap->arr));

    for (i = 0; i < emap->capacity; ++i)
        emap->arr[i] = emap->def;
}

EntityMap *entitymap_new(int def)
{
    EntityMap *emap;

    emap = malloc(sizeof(EntityMap));
    emap->def = def;
    _init(emap);
    return emap;
}
void entitymap_clear(EntityMap *emap)
{
    free(emap->arr);
    _init(emap);
}
void entitymap_free(EntityMap *emap)
{
    free(emap->arr);
    free(emap);
}

static void _grow(EntityMap *emap)
{
    unsigned int new_capacity, i, bound;

    /* find next power of 2 (TODO: use log?) */
    bound = emap->bound;
    for (new_capacity = emap->capacity; new_capacity < bound;
         new_capacity <<= 1);

    /* grow, clear new */
    emap->arr = realloc(emap->arr, new_capacity * sizeof(*emap->arr));
    for (i = emap->capacity; i < new_capacity; ++i)
        emap->arr[i] = emap->def;
    emap->capacity = new_capacity;
}
static void _shrink(EntityMap *emap)
{
    unsigned int new_capacity, bound_times_4;

    if (emap->capacity <= MIN_CAPACITY)
        return;

    /* halve capacity while bound is less than a fourth */
    bound_times_4 = emap->bound << 2;
    if (bound_times_4 >= emap->capacity)
        return;
    for (new_capacity = emap->capacity;
         new_capacity > MIN_CAPACITY && bound_times_4 < new_capacity;
         new_capacity >>= 1);
    if (new_capacity < MIN_CAPACITY)
        new_capacity = MIN_CAPACITY;

    emap->arr = realloc(emap->arr, new_capacity * sizeof(*emap->arr));
    emap->capacity = new_capacity;
}

void entitymap_set(EntityMap *emap, Entity ent, int val)
{
    if (val == emap->def) /* deleting? */
    {
        emap->arr[ent.id] = val;

        /* possibly move bound down and shrink */
        if (emap->bound == ent.id + 1)
        {
            while (emap->bound > 0 && emap->arr[emap->bound - 1] == emap->def)
                --emap->bound;
            _shrink(emap);
        }
    }
    else
    {
        /* possibly move bound up and grow */
        if (ent.id + 1 > emap->bound)
        {
            emap->bound = ent.id + 1;
            if (ent.id >= emap->capacity)
                _grow(emap);
        }

        emap->arr[ent.id] = val;
    }
}
int entitymap_get(EntityMap *emap, Entity ent)
{
    if (ent.id >= emap->capacity)
        return emap->def;
    return emap->arr[ent.id];
}

