#ifndef TRANSFORM_H_SKXSVJO1
#define TRANSFORM_H_SKXSVJO1

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

        EXPORT void transform_set_position(Entity ent, Vec2 pos);
        EXPORT Vec2 transform_get_position(Entity ent);
        EXPORT void transform_translate(Entity ent, Vec2 trans);

        EXPORT void transform_set_rotation(Entity ent, Scalar rot);
        EXPORT Scalar transform_get_rotation(Entity ent);
        EXPORT void transform_rotate(Entity ent, Scalar rot);

        EXPORT void transform_set_scale(Entity ent, Vec2 scale);
        EXPORT Vec2 transform_get_scale(Entity ent);

        EXPORT Mat3 transform_get_world_matrix(Entity ent);

      )

void transform_init();
void transform_deinit();
void transform_update_all();
void transform_save_all(Serializer *s);
void transform_load_all(Deserializer *s);

#endif

