#include "pool.h"

#include <stdlib.h>
#include <string.h>

/* reallocate and notify if moved */
static void _realloc(Pool *pool, unsigned int new_capacity)
{
    unsigned int i;
    void *old_pool;
    char *curr;

    old_pool = pool->pool;

    pool->capacity = new_capacity;
    pool->pool = realloc(pool->pool, pool->capacity * pool->obj_size);

    /* moved? */
    if (pool->move_callback && old_pool != pool->pool)
        for (i = 0, curr = pool->pool; i < pool->num;
                ++i, curr += pool->obj_size)
            pool->move_callback(curr);
}

void pool_init(Pool *pool, size_t obj_size, void (*move_callback) (void *))
{
    pool->obj_size = obj_size;
    pool->capacity = 2;
    pool->pool = malloc(pool->capacity * pool->obj_size);
    pool->num = 0;
    pool->move_callback = move_callback;
}

void pool_deinit(Pool *pool)
{
    free(pool->pool);
    pool->pool = NULL;
    pool->num = 0;
}

void *pool_new_obj(Pool *pool)
{
    /* maybe grow */
    if (pool->num + 1 > pool->capacity)
        _realloc(pool, pool->capacity << 1);

    /* return spot at end */
    ++pool->num;
    return ((char *) pool->pool) + pool->obj_size * (pool->num - 1);
}

void pool_reset(Pool *pool, unsigned int num)
{
    free(pool->pool);

    pool->capacity = pool->num = num;
    pool->pool = malloc(pool->capacity * pool->obj_size);
}

void pool_free_obj(Pool *pool, void *obj)
{
    void *ret = NULL, *end;

    /* replace with object at end */
    end = ((char *) pool->pool) + pool->obj_size * (pool->num - 1);
    if (obj != end && pool->num > 1)
    {
        memcpy(obj, end, pool->obj_size);
        if (pool->move_callback)
            pool->move_callback(obj);
    }

    /* maybe shrink */
    if (--pool->num << 2 < pool->capacity)
        _realloc(pool, pool->capacity >> 1);
}

