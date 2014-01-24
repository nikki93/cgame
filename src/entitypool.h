#ifndef ENTITYPOOL_H_3UYEFMSH
#define ENTITYPOOL_H_3UYEFMSH

#include "entitymap.h"
#include "array.h"

/*
 * continuous in memory, may be relocated/shuffled so be careful
 */

typedef struct EntityPool EntityPool;
#define ENTITYPOOL_HEAD \
    Entity ent

EntityPool *entitypool_new_(size_t object_size);
#define entitypool_new(type) entitypool_new_(sizeof(type))
void entitypool_free(EntityPool *pool);

void *entitypool_add(EntityPool *pool, Entity ent);
void entitypool_remove(EntityPool *pool, Entity ent);
void *entitypool_get(EntityPool *pool, Entity ent);

void *entitypool_ptr(EntityPool *pool); /* array of all objects */
unsigned int entitypool_size(EntityPool *pool);

void entitypool_clear(EntityPool *pool);

#endif

