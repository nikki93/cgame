#ifndef ENTITY_H
#define ENTITY_H

#include <stdbool.h>

#include "saveload.h"
#include "script_export.h"

SCRIPT(entity,

       typedef struct Entity Entity;
       struct Entity { unsigned int id; };
       EXPORT extern Entity entity_nil; /* no valid Entity has this value */

       EXPORT Entity entity_create(); /* claim an unused Entity id */
       EXPORT void entity_destroy(Entity ent); /* release an Entity id */
       EXPORT bool entity_destroyed(Entity ent);

       EXPORT bool entity_eq(Entity e, Entity f);

       /* save/load is just for ids */
       EXPORT void entity_save(Entity *ent, Serializer *s);
       EXPORT void entity_load(Entity *ent, Deserializer *s);

       /*
        * if set true for any entity, only those entities will be
        * saved for which it is set true
        *
        * entities for which this is set false will not be saved
        */
       EXPORT void entity_set_save_filter(Entity ent, bool filter);
       EXPORT bool entity_get_save_filter(Entity ent);
       EXPORT void entity_clear_save_filters();

    )

void entity_init();
void entity_deinit();
void entity_clear();
void entity_update_all();

void entity_load_all_begin();
void entity_load_all_end();

/* C inline stuff */

#define entity_eq(e, f) ((e).id == (f).id)

#endif

