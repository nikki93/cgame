#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "scalar.h"
#include "vec2.h"
#include "mat3.h"
#include "entity.h"
#include "script_export.h"
#include "saveload.h"

/*
 * can set scale, rotation, position -- transformations applied in that order
 */

SCRIPT(transform,

       EXPORT void transform_add(Entity ent);
       EXPORT void transform_remove(Entity ent);

       /* root transforms have parent = entity_nil */
       EXPORT void transform_set_parent(Entity ent, Entity parent);
       EXPORT Entity transform_get_parent(Entity ent);
       EXPORT unsigned int transform_get_num_children(Entity ent);
       EXPORT Entity *transform_get_children(Entity ent);
       /* detach from parent and all children */
       EXPORT void transform_detach_all(Entity ent);

       EXPORT void transform_set_position(Entity ent, Vec2 pos);
       EXPORT Vec2 transform_get_position(Entity ent);
       EXPORT void transform_translate(Entity ent, Vec2 trans);

       EXPORT void transform_set_rotation(Entity ent, Scalar rot);
       EXPORT Scalar transform_get_rotation(Entity ent);
       EXPORT void transform_rotate(Entity ent, Scalar rot);

       EXPORT void transform_set_scale(Entity ent, Vec2 scale);
       EXPORT Vec2 transform_get_scale(Entity ent);

       EXPORT Mat3 transform_get_world_matrix(Entity ent);

       EXPORT Vec2 transform_local_to_world(Entity ent, Vec2 v);

       EXPORT void transform_offset_position(Vec2 pos);
       EXPORT void transform_offset_rotation(Scalar rot);
       EXPORT void transform_offset_scale(Vec2 scale);
       EXPORT void transform_offset_reset();

    )

void transform_init();
void transform_deinit();
void transform_clear();
void transform_update_all();
void transform_save_all(Serializer *s);
void transform_load_all(Deserializer *s);

#endif

