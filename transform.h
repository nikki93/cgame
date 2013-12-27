#ifndef TRANSFORM_H_SKXSVJO1
#define TRANSFORM_H_SKXSVJO1

#include "vec2.h"
#include "mat3.h"
#include "entity.h"

/*
 * can set scale, rotation, position -- transformations applied in that order
 */

/* script_begin */

void transform_add(Entity ent);
void transform_remove(Entity ent);

void transform_set_position(Entity ent, Vec2 pos);
Vec2 transform_get_position(Entity ent);
void transform_translate(Entity ent, Vec2 trans);

void transform_set_rotation(Entity ent, float rot);
float transform_get_rotation(Entity ent);
void transform_rotate(Entity ent, float rot);

void transform_set_scale(Entity ent, Vec2 scale);
Vec2 transform_get_scale(Entity ent);

Mat3 transform_get_world_matrix(Entity ent);

/* script_end */

void transform_save_all();
void transform_load_all();

#endif

