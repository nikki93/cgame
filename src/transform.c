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

    Entity parent; /* root if entity_nil */
    Array *children; /* empty if NULL */

    Mat3 mat_cache; /* remember to update this! */
    Mat3 worldmat_cache; /* cached on parent-child update */
    bool updated; /* used in parent-child update to avoid repeats */
};

static EntityPool *pool;

/* ------------------------------------------------------------------------- */

static void _update_cache(Transform *transform)
{
    transform->mat_cache = mat3_scaling_rotation_translation(
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

    transform->parent = entity_nil;
    transform->children = NULL;
}
void transform_remove(Entity ent)
{
    Transform *transform = entitypool_get(pool, ent);
    assert(transform);
    if (transform && transform->children)
        array_free(transform->children);
    entitypool_remove(pool, ent);
}

void transform_attach(Entity parent, Entity child)
{
    Transform *p, *c;

    p = entitypool_get(pool, parent);
    assert(p);
    c = entitypool_get(pool, child);
    assert(c);

    c->parent = parent;

    if (!p->children)
        p->children = array_new(Entity);
    array_add_val(Entity, p->children) = child;
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

Vec2 transform_local_to_world(Entity ent, Vec2 v)
{
    Transform *transform = entitypool_get(pool, ent);
    assert(transform);
    return mat3_transform(transform->worldmat_cache, v);
}

/* ------------------------------------------------------------------------- */

static void _free_children_arrays()
{
    Transform *transform, *end;
    for (transform = entitypool_begin(pool), end = entitypool_end(pool);
         transform != end; ++transform)
        if (transform->children)
            array_free(transform->children);
}

void transform_init()
{
    pool = entitypool_new(Transform);
}
void transform_deinit()
{
    _free_children_arrays();
    entitypool_free(pool);
}

void transform_clear()
{
    _free_children_arrays();
    entitypool_clear(pool);
}

static void _update(Transform *transform)
{
    Transform *parent;

    if (transform->updated)
        return;

    if (transform->parent != entity_nil) /* child */
    {
        parent = entitypool_get(pool, transform->parent);
        assert(parent);
        _update(parent);
        transform->worldmat_cache = mat3_mul(parent->worldmat_cache,
                                             transform->mat_cache);
    }
    else /* root */
        transform->worldmat_cache = transform->mat_cache;

    transform->updated = true;
}

void transform_update_all()
{
    Transform *transform, *end;

    /* don't precalculate end pointer here because it changes as we remove */
    for (transform = entitypool_begin(pool);
         transform != entitypool_end(pool); )
        if (entity_destroyed(transform->pool_elem.ent))
            transform_remove(transform->pool_elem.ent);
        else
        {
            transform->updated = false;
            ++transform;
        }

    for (transform = entitypool_begin(pool), end = entitypool_end(pool);
         transform != end; ++transform)
        _update(transform);
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

