#ifndef ENTITYPOOL_H_3UYEFMSH
#define ENTITYPOOL_H_3UYEFMSH

#include "entitymap.h"
#include "array.h"
#include "saveload.h"

/*
 * continuous in memory, may be relocated/shuffled so be careful
 */

typedef struct EntityPool EntityPool;

/*
 * this struct must be at the top of pool elements, eg.
 * 
 *      struct Data
 *      {
 *          EntityPoolElem pool_elem;
 *
 *          ...
 *      }
 *
 *  values in it are managed by EntityPool
 */
typedef struct EntityPoolElem EntityPoolElem;
struct EntityPoolElem
{
    Entity ent; /* key for this element */
};

EntityPool *entitypool_new_(size_t object_size);
#define entitypool_new(type) entitypool_new_(sizeof(type))
void entitypool_free(EntityPool *pool);

void *entitypool_add(EntityPool *pool, Entity ent);
void entitypool_remove(EntityPool *pool, Entity ent);
void *entitypool_get(EntityPool *pool, Entity ent);

void *entitypool_begin(EntityPool *pool);
void *entitypool_end(EntityPool *pool); /* one-past-end */
unsigned int entitypool_size(EntityPool *pool);

void entitypool_clear(EntityPool *pool);

/* elem must be /pointer to/ pointer to element */
void entitypool_elem_save(EntityPool *pool, void *elem, Serializer *s);
void entitypool_elem_load(EntityPool *pool, void *elem, Deserializer *s);

#endif

