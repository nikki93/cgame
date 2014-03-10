#ifndef ENTITYMAP_H
#define ENTITYMAP_H

#include "entity.h"

/*
 * map of Entity -> int
 */

typedef struct EntityMap EntityMap;

EntityMap *entitymap_new(int def); /* def is value for unset keys */
void entitymap_clear(EntityMap *emap);
void entitymap_free(EntityMap *emap);

void entitymap_set(EntityMap *emap, Entity ent, int val);
int entitymap_get(EntityMap *emap, Entity ent);

#endif

