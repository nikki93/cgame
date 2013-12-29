#ifndef ENTITY_H_GUOK04WE
#define ENTITY_H_GUOK04WE

#include <stdlib.h>
#include <stdio.h>

#include "script.h"

#define ENTITY_MAX 256

SCRIPT(entity,

        typedef unsigned int Entity;

        Entity entity_new();

      )

/* script_end */

void entity_save(Entity *ent, FILE *file);  /* save/load just the id */
void entity_load(Entity *ent, FILE *file);

#endif


