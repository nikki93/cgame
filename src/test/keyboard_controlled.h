#ifndef KEYBOARD_CONTROLLED_H
#define KEYBOARD_CONTROLLED_H

#include "saveload.h"
#include "entity.h"
#include "script_export.h"

SCRIPT(keyboard_controlled,

       EXPORT void keyboard_controlled_add(Entity ent);
       EXPORT void keyboard_controlled_remove(Entity ent);
       EXPORT bool keyboard_controlled_has(Entity ent);

    )

void keyboard_controlled_update_all();
void keyboard_controlled_save_all(Store *s);
void keyboard_controlled_load_all(Store *s);

#endif

