#include "transform.h"

#include <stdio.h>

typedef struct Transform Transform;
struct Transform
{
    struct Vec2 origin;
};

static unsigned int max_entity = 0;
static Transform transforms[ENTITY_MAX];

/* ------------------------------------------------------------------------- */

void transform_add(Entity ent)
{
    transforms[ent].origin = vec2(0.0f, 0.0f);

    if (ent > max_entity)
        max_entity = ent;
}
void transform_remove(Entity ent)
{
}

void transform_set_position(Entity ent, Vec2 origin)
{
    transforms[ent].origin = origin;
}
Vec2 transform_get_position(Entity ent)
{
    return transforms[ent].origin;
}

void transform_save_all(FILE *file)
{
    fwrite(&max_entity, sizeof(max_entity), 1, file);
    fwrite(transforms, sizeof(Transform), max_entity + 1, file);
}

void transform_load_all(FILE *file)
{
    fread(&max_entity, sizeof(max_entity), 1, file);
    fread(transforms, sizeof(Transform), max_entity + 1, file);
}

