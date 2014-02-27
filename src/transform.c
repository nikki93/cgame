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

static void _detach(Transform *p, Transform *c)
{
    Entity *child, *end;

    c->parent = entity_nil;
    for (child = array_begin(p->children), end = array_end(p->children);
         child != end; ++child)
        if (*child == c->pool_elem.ent)
        {
            array_quick_remove(p->children,
                               child - (Entity *) array_begin(p->children));
            return;
        }
}

static void _detach_all(Transform *t)
{
    Entity *child, *end;
    Transform *p, *c;
    assert(t);

    /* our parent */
    if (t->parent != entity_nil)
    {
        p = entitypool_get(pool, t->parent);
        assert(p);
        _detach(p, t);
    }

    /* our children */
    if (t->children)
    {
        for (child = array_begin(t->children), end = array_end(t->children);
             child != end; ++child)
        {
            c = entitypool_get(pool, *child);
            assert(c);
            c->parent = entity_nil;
        }
        array_free(t->children);
        t->children = NULL;
    }
}

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
    if (transform)
        _detach_all(transform);
    entitypool_remove(pool, ent);
}

void transform_set_parent(Entity ent, Entity parent)
{
    Transform *t, *oldp, *newp;

    t = entitypool_get(pool, ent);
    assert(t);

    if (t->parent == parent)
        return; /* already set */

    /* detach from old */
    if (t->parent != entity_nil)
    {
        oldp = entitypool_get(pool, t->parent);
        assert(oldp);
        _detach(oldp, t);
    }

    /* attach to new */
    t->parent = parent;
    if (parent != entity_nil)
    {
        newp = entitypool_get(pool, parent);
        assert(newp);
        if (!newp->children)
            newp->children = array_new(Entity);
        array_add_val(Entity, newp->children) = ent;
    }
}
unsigned int transform_get_num_children(Entity ent)
{
    Transform *transform = entitypool_get(pool, ent);
    assert(transform);
    return transform->children ? array_length(transform->children) : 0;
}
Entity *transform_get_children(Entity ent)
{
    Transform *transform = entitypool_get(pool, ent);
    assert(transform);
    return array_begin(transform->children);
}
void transform_detach_all(Entity ent)
{
    Transform *transform = entitypool_get(pool, ent);
    assert(transform);
    _detach_all(transform);
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

static void _children_save(Transform *t, Serializer *s)
{
    int n;
    Entity *child, *end;

    if (t->children)
    {
        n = array_length(t->children);
        int_save(&n, s);
        for (child = array_begin(t->children), end = array_end(t->children);
             child != end; ++child)
            entity_save(child, s);
    }
    else
    {
        n = -1;
        int_save(&n, s);
    }
}
static void _children_load(Transform *t, Deserializer *s)
{
    int n;
    Entity *child;

    int_load(&n, s);
    if (n >= 0)
    {
        t->children = array_new(Entity);
        while (n--)
        {
            child = array_add(t->children);
            entity_load(child, s);
        }
    }
    else
        t->children = NULL;
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

        entity_save(&transform->parent, s);
        _children_save(transform, s);

        mat3_save(&transform->mat_cache, s);
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

        entity_load(&transform->parent, s);
        _children_load(transform, s);

        mat3_load(&transform->mat_cache, s);
    }
}

