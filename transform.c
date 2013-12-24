#include "transform.h"

typedef struct Transform Transform;
struct Transform
{
    struct Vec2 origin;
};

static Transform transforms[ENTITY_MAX];

/* ------------------------------------------------------------------------- */

void transform_add(Entity ent)
{
    transforms[ent].origin = vec2(0.0f, 0.0f);
}
void transform_remove(Entity ent)
{
}

void transform_set_origin(Entity ent, Vec2 origin)
{
    transforms[ent].origin = origin;
}
Vec2 transform_get_origin(Entity ent)
{
    return transforms[ent].origin;
}

