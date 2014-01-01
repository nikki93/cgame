#ifndef KEYBOARD_CONTROLLED_H_B4CPIANU
#define KEYBOARD_CONTROLLED_H_B4CPIANU

#include <stdio.h>

#include "entity.h"
#include "script.h"

SCRIPT(keyboard_controlled,

        EXPORT void keyboard_controlled_add(Entity ent);
        EXPORT void keyboard_controlled_remove(Entity ent);

      )

void keyboard_controlled_update_all(float dt);
void keyboard_controlled_save_all(FILE *file);
void keyboard_controlled_load_all(FILE *file);

#endif

