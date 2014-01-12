#ifndef SPRITE_H_L45BNMPA
#define SPRITE_H_L45BNMPA

#include "saveload.h"
#include "entity.h"
#include "vec2.h"
#include "script_export.h"

SCRIPT(sprite,

        EXPORT void sprite_add(Entity ent);
        EXPORT void sprite_remove(Entity ent);

        EXPORT void sprite_set_cell(Entity ent, Vec2 cell);
        EXPORT void sprite_set_size(Entity ent, Vec2 size);

      )

void sprite_init();
void sprite_deinit();
void sprite_update_all();
void sprite_draw_all();
void sprite_save_all(Serializer *s);
void sprite_load_all(Deserializer *s);

#endif

