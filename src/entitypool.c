#include "entitypool.h"

#include <stdlib.h>

#include "entitymap.h"
#include "array.h"
#include "error.h"

struct EntityPool
{
    /* just a map of indices into an array, -1 if doesn't exist */
    EntityMap *emap;
    Array *array;
};

EntityPool *entitypool_new_(size_t object_size)
{
    EntityPool *pool = malloc(sizeof(EntityPool));

    pool->emap = entitymap_new(-1);
    pool->array = array_new_(object_size);

    return pool;
}
void entitypool_free(EntityPool *pool)
{
    array_free(pool->array);
    entitymap_free(pool->emap);
    free(pool);
}

void *entitypool_add(EntityPool *pool, Entity ent)
{
    EntityPoolElem *elem;

    if ((elem = entitypool_get(pool, ent)))
        return elem;

    /* add element to array and set id in map */
    elem = array_add(pool->array);
    elem->ent = ent;
    entitymap_set(pool->emap, ent, array_length(pool->array) - 1);
    return elem;
}
void entitypool_remove(EntityPool *pool, Entity ent)
{
    int i;
    EntityPoolElem *elem;

    i = entitymap_get(pool->emap, ent);
    if (i >= 0)
    {
        /* remove may swap with last element, so fix that mapping */
        if (array_quick_remove(pool->array, i))
        {
            elem = array_get(pool->array, i);
            entitymap_set(pool->emap, elem->ent, i);
        }

        /* remove mapping */
        entitymap_set(pool->emap, ent, -1);
    }
}
void *entitypool_get(EntityPool *pool, Entity ent)
{
    int i;

    i = entitymap_get(pool->emap, ent);
    if (i >= 0)
        return array_get(pool->array, i);
    return NULL;
}

void *entitypool_begin(EntityPool *pool)
{
    return array_begin(pool->array);
}
void *entitypool_end(EntityPool *pool)
{
    return array_end(pool->array);
}
void *entitypool_nth(EntityPool *pool, unsigned int n)
{
    return array_get(pool->array, n);
}

unsigned int entitypool_size(EntityPool *pool)
{
    return array_length(pool->array);
}

void entitypool_clear(EntityPool *pool)
{
    entitymap_clear(pool->emap);
    array_clear(pool->array);
}

void entitypool_sort(EntityPool *pool,
                     int (*compar)(const void *, const void *))
{
    unsigned int i, n;
    EntityPoolElem *elem;

    array_sort(pool->array, compar);

    /* remap Entity -> index */
    n = array_length(pool->array);
    for (i = 0; i < n; ++i)
    {
        elem = array_get(pool->array, i);
        entitymap_set(pool->emap, elem->ent, i);
    }
}

void entitypool_elem_save(EntityPool *pool, void *elem, Store *s)
{
    EntityPoolElem **p;

    /* save Entity id */
    p = elem;
    entity_save(&(*p)->ent, "pool_elem", s);
}
void entitypool_elem_load(EntityPool *pool, void *elem, Store *s)
{
    Entity ent;
    EntityPoolElem **p;

    /* load Entity id, add element with that key */
    error_assert(entity_load(&ent, "pool_elem", entity_nil, s),
                 "saved EntityPoolElem entry must exist");
    p = elem;
    *p = entitypool_add(pool, ent);
    (*p)->ent = ent;
}

