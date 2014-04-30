#ifndef SPRITE_H
#define SPRITE_H

#include "saveload.h"
#include "entity.h"
#include "vec2.h"
#include "script_export.h"

SCRIPT(sprite,

       EXPORT void sprite_add(Entity ent);
       EXPORT void sprite_remove(Entity ent);

       /* size to draw in world units, centered at transform position */
       EXPORT void sprite_set_size(Entity ent, Vec2 size);
       EXPORT Vec2 sprite_get_size(Entity ent);

       /* bottom left corner of atlas region in pixels */
       EXPORT void sprite_set_texcell(Entity ent, Vec2 texcell);
       EXPORT Vec2 sprite_get_texcell(Entity ent);

       /* size of atlas region in pixels */
       EXPORT void sprite_set_texsize(Entity ent, Vec2 texsize);
       EXPORT Vec2 sprite_get_texsize(Entity ent);

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

