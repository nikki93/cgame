#ifndef SPRITE_H
#define SPRITE_H

#include "saveload.h"
#include "entity.h"
#include "vec2.h"
#include "script_export.h"

SCRIPT(sprite,

       EXPORT void sprite_add(Entity ent);
       EXPORT void sprite_remove(Entity ent);

       EXPORT void sprite_set_cell(Entity ent, Vec2 cell);
       EXPORT void sprite_set_size(Entity ent, Vec2 size);

       /* lower depth drawn on top */
       EXPORT void sprite_set_depth(Entity ent, int depth);
       EXPORT int sprite_get_depth(Entity ent);

    )

void sprite_init();
void sprite_deinit();
void sprite_update_all();
void sprite_draw_all();
void sprite_save_all(Serializer *s);
void sprite_load_all(Deserializer *s);

#endif

