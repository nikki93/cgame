#include "transform.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "entitypool.h"
#include "saveload.h"

typedef struct Transform Transform;
struct Transform
{
    EntityPoolElem pool_elem;

    Vec2 position;
    Scalar rotation;
    Vec2 scale;

    Mat3 worldmat_cache; /* remember to update this! */
};

static EntityPool *pool;

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

    if (entitypool_get(pool, ent))
        return;

    transform = entitypool_add(pool, ent);
    transform->position = vec2(0.0f, 0.0f);
    transform->rotation = 0.0f;
    transform->scale = vec2(1.0f, 1.0f);
}
void transform_remove(Entity ent)
{
    entitypool_remove(pool, ent);
}

void transform_set_position(Entity ent, Vec2 pos)
{
    Transform *transform = entitypool_get(pool, ent);
    assert(transform);
    transform->position = pos;
    _update_cache(transform);
}
Vec2 transform_get_position(Entity ent)
{
    Transform *transform = entitypool_get(pool, ent);
    assert(transform);
    return transform->position;
}
void transform_translate(Entity ent, Vec2 trans)
{
    Transform *transform = entitypool_get(pool, ent);
    assert(transform);
    transform->position = vec2_add(transform->position, trans);
    _update_cache(transform);
}

void transform_set_rotation(Entity ent, Scalar rot)
{
    Transform *transform = entitypool_get(pool, ent);
    assert(transform);
    transform->rotation = rot;
    _update_cache(transform);
}
Scalar transform_get_rotation(Entity ent)
{
    Transform *transform = entitypool_get(pool, ent);
    assert(transform);
    return transform->rotation;
}
void transform_rotate(Entity ent, Scalar rot)
{
    Transform *transform = entitypool_get(pool, ent);
    assert(transform);
    transform->rotation += rot;
    _update_cache(transform);
}

void transform_set_scale(Entity ent, Vec2 scale)
{
    Transform *transform = entitypool_get(pool, ent);
    assert(transform);
    transform->scale = scale;
    _update_cache(transform);
}
Vec2 transform_get_scale(Entity ent)
{
    Transform *transform = entitypool_get(pool, ent);
    assert(transform);
    return transform->scale;
}

Mat3 transform_get_world_matrix(Entity ent)
{
    Transform *transform = entitypool_get(pool, ent);
    assert(transform);
    return transform->worldmat_cache;
}

/* ------------------------------------------------------------------------- */

void transform_init()
{
    pool = entitypool_new(Transform);
}
void transform_deinit()
{
    entitypool_free(pool);
}

void transform_update_all()
{
    Transform *transform;

    /* don't precalculate end pointer here because it changes as we remove */
    for (transform = entitypool_begin(pool);
            transform != entitypool_end(pool); )
        if (entity_destroyed(transform->pool_elem.ent))
            transform_remove(transform->pool_elem.ent);
        else
            ++transform;
}

void transform_save_all(Serializer *s)
{
    unsigned int n;
    Transform *transform, *end;

    n = entitypool_size(pool);
    uint_save(&n, s);

    for (transform = entitypool_begin(pool), end = entitypool_end(pool);
            transform != end; ++transform)
    {
        entitypool_elem_save(pool, &transform, s);
        vec2_save(&transform->position, s);
        scalar_save(&transform->rotation, s);
        vec2_save(&transform->scale, s);
        mat3_save(&transform->worldmat_cache, s);
    }
}
void transform_load_all(Deserializer *s)
{
    unsigned int n;
    Transform *transform;

    entitypool_clear(pool);

    uint_load(&n, s);

    while (n--)
    {
        entitypool_elem_load(pool, &transform, s);
        vec2_load(&transform->position, s);
        scalar_load(&transform->rotation, s);
        vec2_load(&transform->scale, s);
        mat3_load(&transform->worldmat_cache, s);
    }
}

