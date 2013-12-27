#include "transform.h"

#include <stdio.h>
#include <math.h>

#include "saveload.h"

typedef struct Transform Transform;
struct Transform
{
    Vec2 position;
    float rotation;
    Vec2 scale;
};

static unsigned int max_entity = 0;
static Transform transforms[ENTITY_MAX];

/* ------------------------------------------------------------------------- */

void transform_add(Entity ent)
{
    transforms[ent].position = vec2(0.0f, 0.0f);
    transforms[ent].rotation = 0.0f;
    transforms[ent].scale = vec2(1.0f, 1.0f);

    if (ent > max_entity)
        max_entity = ent;
}
void transform_remove(Entity ent)
{
}

void transform_set_position(Entity ent, Vec2 pos)
{
    transforms[ent].position = pos;
}
Vec2 transform_get_position(Entity ent)
{
    return transforms[ent].position;
}
void transform_translate(Entity ent, Vec2 trans)
{
    transforms[ent].position = vec2_add(transforms[ent].position, trans);
}

void transform_set_rotation(Entity ent, float rot)
{
    transforms[ent].rotation = rot;
}
float transform_get_rotation(Entity ent)
{
    return transforms[ent].rotation;
}
void transform_rotate(Entity ent, float rot)
{
    transforms[ent].rotation += rot;
}

void transform_set_scale(Entity ent, Vec2 pos)
{
    transforms[ent].scale = pos;
}
Vec2 transform_get_scale(Entity ent)
{
    return transforms[ent].scale;
}

Mat3 transform_get_world_matrix(Entity ent)
{
    return mat3_scaling_rotation_translation(
            transforms[ent].scale,
            transforms[ent].rotation,
            transforms[ent].position
            );
}

void transform_save_all(FILE *file)
{
    unsigned int i;

    uint_save(&max_entity, file);

    for (i = 0; i <= max_entity; ++i)
    {
        vec2_save(&transforms[i].position, file);
        scalar_save(&transforms[i].rotation, file);
        vec2_save(&transforms[i].scale, file);
    }
}

void transform_load_all(FILE *file)
{
    unsigned int i;

    uint_load(&max_entity, file);

    for (i = 0; i <= max_entity; ++i)
    {
        vec2_load(&transforms[i].position, file);
        scalar_load(&transforms[i].rotation, file);
        vec2_load(&transforms[i].scale, file);
    }
}

