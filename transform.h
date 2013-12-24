#ifndef TRANSFORM_H_SKXSVJO1
#define TRANSFORM_H_SKXSVJO1

#include "entity.h"

/* script_begin */

typedef struct Vec2 Vec2;
struct Vec2 { float x; float y; };

/* script_end */

#define vec2(x, y) ((Vec2) { x, y })

/* script_begin */

void transform_add(Entity ent);
void transform_remove(Entity ent);

void transform_set_position(Entity ent, Vec2 origin);
Vec2 transform_get_position(Entity ent);

/* script_end */

void transform_save_all();
void transform_load_all();

#endif

