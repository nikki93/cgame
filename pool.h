#ifndef POOL_H_830LVYV6
#define POOL_H_830LVYV6

#include <stddef.h>

/*
 * store objects contiguous in memory -- good for cache performance or
 * sending as OpenGL buffer etc.
 *
 * NOTE: objects may move in memory, register a move_callback to be notified
 */

typedef struct Pool Pool;
struct Pool
{
    void *pool;              /* first object */
    unsigned int num;        /* number of objects in pool */

    /* private stuff */
    unsigned int capacity;   /* number of objects that can fit in pool */
    size_t obj_size;         /* size of each object */
    void (*move_callback) (void *); /* call this if moved on realloc */
};

void pool_init(Pool *pool, size_t obj_size, void (*move_callback) (void *));
void pool_deinit(Pool *pool);

void *pool_new_obj(Pool *pool);
void pool_reset(Pool *pool, unsigned int num); /* resize to 'num' objects,
                                                  old data is lost */
void pool_free_obj(Pool *pool, void *obj);

#endif

