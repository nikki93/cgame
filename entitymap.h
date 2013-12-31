#ifndef ENTITYMAP_H_YGLNG310
#define ENTITYMAP_H_YGLNG310

#include <stdbool.h>

#include "entity.h"

typedef struct EntityMap EntityMap;
extern void *entitymap_nil; /* != NULL, can use this as default_val
                               if NULL is also a valid value */

EntityMap *entitymap_new(void *nil); /* nil is the value used to delete or
                                        returned if not found -- like Lua */
void entitymap_clear(EntityMap *emap);
void entitymap_free(EntityMap *emap);

void entitymap_set(EntityMap *emap, Entity ent, /* if val == nil from */
        void *val);                             /* entitymap_new(), delete */
void *entitymap_get(EntityMap *emap, Entity ent); /* return nil from
                                                     entitymap_new() if not
                                                     found */
Entity entitymap_get_max(EntityMap *emap); /* get 1 + max key, 0 if empty */

/* convenience macros to remove the ugly looking casts */
#define entitymap_get_type(type, emap, ent) \
    ((type) entitymap_get(emap, ent))

#endif

