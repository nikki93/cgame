#ifndef ENTITY_H_GUOK04WE
#define ENTITY_H_GUOK04WE

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "script.h"

SCRIPT(entity,

        typedef unsigned int Entity;

        EXPORT Entity entity_create();
        EXPORT void entity_destroy(Entity ent);
        EXPORT bool entity_destroyed(Entity ent);

      )

/* script_end */

void entity_init();
void entity_deinit();
void entity_update_all();
void entity_save(Entity *ent, FILE *file);  /* save/load just the id */
void entity_load(Entity *ent, FILE *file);

#endif


