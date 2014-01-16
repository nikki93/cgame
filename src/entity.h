#ifndef ENTITY_H_GUOK04WE
#define ENTITY_H_GUOK04WE

#include <stdlib.h>
#include <stdbool.h>

#include "saveload.h"
#include "script_export.h"

SCRIPT(entity,

        typedef unsigned int Entity;

        EXPORT Entity entity_create();
        EXPORT void entity_destroy(Entity ent);
        EXPORT bool entity_destroyed(Entity ent);

      )

void entity_save(Entity *ent, Serializer *s);  /* save/load just the id */
void entity_load(Entity *ent, Deserializer *s);

void entity_init();
void entity_deinit();
void entity_update_all();
void entity_save_all(Serializer *s);
void entity_load_all(Deserializer *s);

#endif

