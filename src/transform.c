#include "transform.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include "entitypool.h"
#include "array.h"
#include "saveload.h"
#include "bbox.h"
#include "edit.h"

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

    unsigned int dirty_count;
};

static EntityPool *pool;

/* ------------------------------------------------------------------------- */

static void _detach(Transform *p, Transform *c)
{
    Entity *child;

    /* remove child -> parent link */
    c->parent = entity_nil;

    /* search for parent -> child link and remove it */
    array_foreach(child, p->children)
        if (entity_eq(*child, c->pool_elem.ent))
        {
            array_quick_remove(p->children,
                               child - (Entity *) array_begin(p->children));
            return;
        }
}

static void _detach_all(Transform *t)
{
    Entity *child;
    Transform *p, *c;
    assert(t);

    /* our parent */
    if (!entity_eq(t->parent, entity_nil))
    {
        p = entitypool_get(pool, t->parent);
        assert(p);
        _detach(p, t);
    }

    /* our children -- unset each child's parent then clear children array */
    if (t->children)
    {
        array_foreach(child, t->children)
        {
            c = entitypool_get(pool, *child);
            assert(c);
            c->parent = entity_nil;
        }
        array_free(t->children);
        t->children = NULL;
    }
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

    transform->dirty_count = 0;
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

    if (entity_eq(t->parent, parent))
        return; /* already set */

    /* detach from old */
    if (!entity_eq(t->parent, entity_nil))
    {
        oldp = entitypool_get(pool, t->parent);
        assert(oldp);
        _detach(oldp, t);
    }

    /* attach to new */
    t->parent = parent;
    if (!entity_eq(parent, entity_nil))
    {
        newp = entitypool_get(pool, parent);
        assert(newp);
        if (!newp->children)
            newp->children = array_new(Entity);
        array_add_val(Entity, newp->children) = ent;
    }
}
Entity transform_get_parent(Entity ent)
{
    Transform *transform = entitypool_get(pool, ent);
    assert(transform);
    return transform->parent;
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

static void _modified(Transform *transform)
{
    transform->mat_cache = mat3_scaling_rotation_translation(
        transform->scale,
        transform->rotation,
        transform->position
        );
    ++transform->dirty_count;
}

void transform_set_position(Entity ent, Vec2 pos)
{
    Transform *transform = entitypool_get(pool, ent);
    assert(transform);
    transform->position = pos;
    _modified(transform);
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
    _modified(transform);
}

void transform_set_rotation(Entity ent, Scalar rot)
{
    Transform *transform = entitypool_get(pool, ent);
    assert(transform);
    transform->rotation = rot;
    _modified(transform);
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
    _modified(transform);
}

void transform_set_scale(Entity ent, Vec2 scale)
{
    Transform *transform = entitypool_get(pool, ent);
    assert(transform);
    transform->scale = scale;
    _modified(transform);
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

unsigned int transform_get_dirty_count(Entity ent)
{
    Transform *transform = entitypool_get(pool, ent);
    assert(transform);
    return transform->dirty_count;
}

/* ------------------------------------------------------------------------- */

static void _free_children_arrays()
{
    Transform *transform;

    entitypool_foreach(transform, pool)
        if (transform->children)
            array_free(transform->children);
}

void transform_init()
{
    pool = entitypool_new(Transform);
    transform_offset_reset();
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

/*
 * update worldmat_cache to reflect hierarchy
 *
 * use recursion -- transform hierarchies should generally be shallow
 */
static void _update(Transform *transform)
{
    Transform *parent;

    /* already updated? */
    if (transform->updated)
        return;

    if (!entity_eq(transform->parent, entity_nil)) /* child */
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
    Transform *transform;
    static BBox bbox = { { -0.25, -0.25 }, { 0.25, 0.25 } };

    entitypool_remove_destroyed(pool, transform_remove);

    /* update all */
    entitypool_foreach(transform, pool)
        transform->updated = false;
    entitypool_foreach(transform, pool)
        _update(transform);

    /* update edit bbox */
    if (edit_get_enabled())
        entitypool_foreach(transform, pool)
            edit_update_bbox(transform->pool_elem.ent, bbox);
}

/* save/load for just the children array */
static void _children_save(Transform *t, Serializer *s)
{
    int n; /* -1 if t->children is NULL */
    Entity *child;

    if (t->children)
    {
        n = array_length(t->children);
        int_save(&n, s);
        array_foreach(child, t->children)
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
    int n; /* -1 if t->children should be NULL */
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

static Vec2 offset_pos;
static Scalar offset_rot;
static Vec2 offset_scale;

void transform_offset_position(Vec2 pos)
{
    offset_pos = pos;
}
void transform_offset_rotation(Scalar rot)
{
    offset_rot = rot;
}
void transform_offset_scale(Vec2 scale)
{
    offset_scale = scale;
}
void transform_offset_reset()
{
    offset_pos = vec2(0.0f, 0.0f);
    offset_rot = 0.0f;
    offset_scale = vec2(1.0f, 1.0f);
}

/* apply above offsets to given transform */
static void _apply_offset(Transform *t)
{
    /* scale */
    t->position = vec2_mul(t->position, offset_scale);
    t->scale = vec2_mul(t->scale, offset_scale);

    /* rot */
    t->position = vec2_rot(t->position, offset_rot);
    t->rotation += offset_rot;

    /* pos */
    t->position = vec2_add(t->position, offset_pos);
}

void transform_save_all(Serializer *s)
{
    unsigned int n;
    Transform *transform;

    n = entitypool_size(pool);
    uint_save(&n, s);

    entitypool_foreach(transform, pool)
    {
        entitypool_elem_save(pool, &transform, s);
        vec2_save(&transform->position, s);
        scalar_save(&transform->rotation, s);
        vec2_save(&transform->scale, s);

        entity_save(&transform->parent, s);
        _children_save(transform, s);
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

        if (entity_eq(transform->parent, entity_nil))
            _apply_offset(transform); /* offset root transforms */
        _modified(transform);
    }
}

