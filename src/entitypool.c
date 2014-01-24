#include "entitypool.h"

#include "entitymap.h"
#include "array.h"

struct EntityPool
{
    EntityMap *emap;
    Array *array;
};

typedef struct EntityPoolElem EntityPoolElem;
struct EntityPoolElem { ENTITYPOOL_HEAD; };

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
}

void *entitypool_add(EntityPool *pool, Entity ent)
{
    entitymap_set(pool->emap, ent, array_length(pool->array));
    return array_add(pool->array);
}
void entitypool_remove(EntityPool *pool, Entity ent)
{
    int i;

    i = entitymap_get(pool->emap, ent);
    if (i >= 0)
    {
        if (array_quick_remove(pool->array, i))
            entitymap_set(pool->emap,
                    ((EntityPoolElem *) array_get(pool->array, i))->ent, i);
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

void *entitypool_ptr(EntityPool *pool)
{
    return array_get(pool->array, 0);
}
unsigned int entitypool_size(EntityPool *pool)
{
    return array_length(pool->array);
}

void entitypool_clear(EntityPool *pool)
{
    entitymap_clear(pool->emap);
    array_reset(pool->array, 0);
}

