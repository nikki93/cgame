#ifndef TRANSFORM_H_SKXSVJO1
#define TRANSFORM_H_SKXSVJO1

#include "entity.h"

typedef struct Vec2 Vec2;
struct Vec2 { float x; float y; };

#define vec2(x, y) ((Vec2) { x, y })

void transform_add(Entity ent);
void transform_remove(Entity ent);

void transform_set_origin(Entity ent, Vec2 origin);
Vec2 transform_get_origin(Entity ent);

void transform_save_all();
void transform_load_all();

#endif

