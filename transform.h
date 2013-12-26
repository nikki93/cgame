#ifndef TRANSFORM_H_SKXSVJO1
#define TRANSFORM_H_SKXSVJO1

#include "vec2.h"
#include "entity.h"

/* script_begin */

void transform_add(Entity ent);
void transform_remove(Entity ent);

void transform_set_position(Entity ent, Vec2 pos);
Vec2 transform_get_position(Entity ent);
void transform_set_rotation(Entity ent, float rot);
void transform_rotate(Entity ent, float rot);
float transform_get_rotation(Entity ent);

void transform_get_world_matrix(Entity ent, float cols[3][3]);

/* script_end */

void transform_save_all();
void transform_load_all();

#endif

