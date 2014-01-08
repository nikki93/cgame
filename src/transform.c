#include "transform.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "array.h"
#include "entitymap.h"
#include "saveload.h"

typedef struct Transform Transform;
struct Transform
{
    Entity ent;

    Vec2 position;
    float rotation;
    Vec2 scale;

    Mat3 worldmat_cache; /* remember to update this! */
};

static EntityMap *emap;
static Array *transforms;

/* ------------------------------------------------------------------------- */

static void _update_cache(Transform *transform)
{
    transform->worldmat_cache = mat3_scaling_rotation_translation(
            transform->scale,
            transform->rotation,
            transform->position
            );
}

void transform_add(Entity ent)
{
    Transform *transform;

    if (entitymap_get(emap, ent) >= 0)
        return; /* already has a transform */

    transform = array_add(transforms);
    transform->ent = ent;
    transform->position = vec2(0.0f, 0.0f);
    transform->rotation = 0.0f;
    transform->scale = vec2(1.0f, 1.0f);

    entitymap_set(emap, ent, array_length(transforms) - 1);
}
void transform_remove(Entity ent)
{
    int i;

    if ((i = entitymap_get(emap, ent)) >= 0)
    {
        if (array_quick_remove(transforms, i))
            entitymap_set(emap, array_get_val(Transform, transforms, i).ent, i);
        entitymap_set(emap, ent, -1);
    }
}

#define GET \
    int i = entitymap_get(emap, ent); \
    assert(i >= 0); \
    Transform *transform = array_get(transforms, i);

void transform_set_position(Entity ent, Vec2 pos)
{
    GET;
    transform->position = pos;
    _update_cache(transform);
}
Vec2 transform_get_position(Entity ent)
{
    GET;
    return transform->position;
}
void transform_translate(Entity ent, Vec2 trans)
{
    GET;
    transform->position = vec2_add(transform->position, trans);
    _update_cache(transform);
}

void transform_set_rotation(Entity ent, float rot)
{
    GET;
    transform->rotation = rot;
    _update_cache(transform);
}
float transform_get_rotation(Entity ent)
{
    GET;
    return transform->rotation;
}
void transform_rotate(Entity ent, float rot)
{
    GET;
    transform->rotation += rot;
    _update_cache(transform);
}

void transform_set_scale(Entity ent, Vec2 scale)
{
    GET;
    transform->scale = scale;
    _update_cache(transform);
}
Vec2 transform_get_scale(Entity ent)
{
    GET;
    return transform->scale;
}

Mat3 transform_get_world_matrix(Entity ent)
{
    GET;
    return transform->worldmat_cache;
}

/* ------------------------------------------------------------------------- */

void transform_init()
{
    emap = entitymap_new(-1);
    transforms = array_new(Transform);
}
void transform_deinit()
{
    array_free(transforms);
    entitymap_free(emap);
}

void transform_update_all()
{
    unsigned int i;
    Transform *transform;

    for (i = 0; i < array_length(transforms); )
    {
        transform = array_get(transforms, i);
        if (entity_destroyed(transform->ent))
            transform_remove(transform->ent);
        else
            ++i;
    }
}

void transform_save_all(FILE *file)
{
    unsigned int i, n;
    Transform *transform;

    n = array_length(transforms);

    uint_save(&n, file);
    for (i = 0; i < n; ++i)
    {
        transform = array_get(transforms, i);
        entity_save(&transform->ent, file);
        vec2_save(&transform->position, file);
        scalar_save(&transform->rotation, file);
        vec2_save(&transform->scale, file);
        mat3_save(&transform->worldmat_cache, file);
    }
}
void transform_load_all(FILE *file)
{
    unsigned int i, n;
    Transform *transform;

    uint_load(&n, file);
    array_reset(transforms, n);

    entitymap_clear(emap);

    for (i = 0; i < n; ++i)
    {
        transform = array_get(transforms, i);
        entity_load(&transform->ent, file);
        vec2_load(&transform->position, file);
        scalar_load(&transform->rotation, file);
        vec2_load(&transform->scale, file);
        mat3_load(&transform->worldmat_cache, file);

        entitymap_set(emap, transform->ent, i);
    }
}

