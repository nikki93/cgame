#ifndef KEYBOARD_CONTROLLED_H_B4CPIANU
#define KEYBOARD_CONTROLLED_H_B4CPIANU

#include "scalar.h"
#include "saveload.h"
#include "entity.h"
#include "script_export.h"

SCRIPT(keyboard_controlled,

       EXPORT void keyboard_controlled_add(Entity ent);
       EXPORT void keyboard_controlled_remove(Entity ent);

    )

void keyboard_controlled_update_all(Scalar dt);
void keyboard_controlled_save_all(Serializer *s);
void keyboard_controlled_load_all(Deserializer *s);

#endif

