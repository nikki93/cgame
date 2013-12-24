#ifndef KEYBOARD_CONTROLLED_H_B4CPIANU
#define KEYBOARD_CONTROLLED_H_B4CPIANU

#include <stdio.h>

#include "entity.h"

/* script_begin */

void keyboard_controlled_add(Entity ent);
void keyboard_controlled_remove(Entity ent);

/* script_end */

void keyboard_controlled_update_all(float dt);
void keyboard_controlled_save_all(FILE *file);
void keyboard_controlled_load_all(FILE *file);

#endif

