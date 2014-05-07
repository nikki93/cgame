#include "transform.h"

#include <stdbool.h>
#include <stdio.h>

#include "error.h"
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

    unsigned int dirty_count;
};

static EntityPool *pool;

/* ------------------------------------------------------------------------- */

static void _update_child(Transform *parent, Entity ent)
{
    Transform *transform;
    Entity *child;

    transform = entitypool_get(pool, ent);
    error_assert(transform);
    transform->worldmat_cache = mat3_mul(parent->worldmat_cache,
                                         transform->mat_cache);
    if (transform->children)
        array_foreach(child, transform->children)
            _update_child(transform, *child);
}
static void _modified(Transform *transform)
{
    Transform *parent;
    Entity *child;
    
    ++transform->dirty_count;

    transform->mat_cache = mat3_scaling_rotation_translation(
        transform->scale,
        transform->rotation,
        transform->position
        );

    /* update our world matrix */
    parent = entitypool_get(pool, transform->parent);
    if (parent)
        transform->worldmat_cache = mat3_mul(parent->worldmat_cache,
                                             transform->mat_cache);
    else
        transform->worldmat_cache = transform->mat_cache;

    /* update children world matrices */
    if (transform->children)
        array_foreach(child, transform->children)
            _update_child(transform, *child);
}

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

    _modified(c);
}

static void _detach_all(Transform *t)
{
    Entity *child;
    Transform *p, *c;
    error_assert(t);

    /* our parent */
    if (!entity_eq(t->parent, entity_nil))
    {
        p = entitypool_get(pool, t->parent);
        error_assert(p);
        _detach(p, t);
    }

    /* our children -- unset each child's parent then clear children array */
    if (t->children)
    {
        array_foreach(child, t->children)
        {
            c = entitypool_get(pool, *child);
            error_assert(c);
            c->parent = entity_nil;
            _modified(c);
        }
        array_free(t->children);
        t->children = NULL;
    }

    _modified(t);
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

    _modified(transform);
}
void transform_remove(Entity ent)
{
    Transform *transform = entitypool_get(pool, ent);
    if (transform)
        _detach_all(transform);
    entitypool_remove(pool, ent);
}
bool transform_has(Entity ent)
{
    return entitypool_get(pool, ent) != NULL;
}

void transform_set_parent(Entity ent, Entity parent)
{
    Transform *t, *oldp, *newp;

    if (entity_eq(ent, parent))
        return; /* can't be child of self */

    t = entitypool_get(pool, ent);
    error_assert(t);

    if (entity_eq(t->parent, parent))
        return; /* already set */

    /* detach from old */
    if (!entity_eq(t->parent, entity_nil))
    {
        oldp = entitypool_get(pool, t->parent);
        error_assert(oldp);
        _detach(oldp, t);
    }

    /* attach to new */
    t->parent = parent;
    if (!entity_eq(parent, entity_nil))
    {
        newp = entitypool_get(pool, parent);
        error_assert(newp);
        if (!newp->children)
            newp->children = array_new(Entity);
        array_add_val(Entity, newp->children) = ent;
    }

    _modified(t);
}
Entity transform_get_parent(Entity ent)
{
    Transform *transform = entitypool_get(pool, ent);
    error_assert(transform);
    return transform->parent;
}
unsigned int transform_get_num_children(Entity ent)
{
    Transform *transform = entitypool_get(pool, ent);
    error_assert(transform);
    return transform->children ? array_length(transform->children) : 0;
}
Entity *transform_get_children(Entity ent)
{
    Transform *transform = entitypool_get(pool, ent);
    error_assert(transform);
    return transform->children ? array_begin(transform->children) : NULL;
}
void transform_detach_all(Entity ent)
{
    Transform *transform = entitypool_get(pool, ent);
    error_assert(transform);
    _detach_all(transform);
}
void transform_destroy_rec(Entity ent)
{
    Transform *transform;
    Entity *child;

    transform = entitypool_get(pool, ent);
    if (transform && transform->children)
        array_foreach(child, transform->children)
            transform_destroy_rec(*child);

    entity_destroy(ent);
}

void transform_set_position(Entity ent, Vec2 pos)
{
    Transform *transform = entitypool_get(pool, ent);
    error_assert(transform);
    transform->position = pos;
    _modified(transform);
}
Vec2 transform_get_position(Entity ent)
{
    Transform *transform = entitypool_get(pool, ent);
    error_assert(transform);
    return transform->position;
}
void transform_translate(Entity ent, Vec2 trans)
{
    Transform *transform = entitypool_get(pool, ent);
    error_assert(transform);
    transform->position = vec2_add(transform->position, trans);
    _modified(transform);
}

void transform_set_rotation(Entity ent, Scalar rot)
{
    Transform *transform = entitypool_get(pool, ent);
    error_assert(transform);
    transform->rotation = rot;
    _modified(transform);
}
Scalar transform_get_rotation(Entity ent)
{
    Transform *transform = entitypool_get(pool, ent);
    error_assert(transform);
    return transform->rotation;
}
void transform_rotate(Entity ent, Scalar rot)
{
    Transform *transform = entitypool_get(pool, ent);
    error_assert(transform);
    transform->rotation += rot;
    _modified(transform);
}

void transform_set_scale(Entity ent, Vec2 scale)
{
    Transform *transform = entitypool_get(pool, ent);
    error_assert(transform);
    transform->scale = scale;
    _modified(transform);
}
Vec2 transform_get_scale(Entity ent)
{
    Transform *transform = entitypool_get(pool, ent);
    error_assert(transform);
    return transform->scale;
}

Vec2 transform_get_world_position(Entity ent)
{
    Transform *transform = entitypool_get(pool, ent);
    error_assert(transform);
    return mat3_get_translation(transform->worldmat_cache);
}
Scalar transform_get_world_rotation(Entity ent)
{
    Transform *transform = entitypool_get(pool, ent);
    error_assert(transform);
    return mat3_get_rotation(transform->worldmat_cache);
}
Vec2 transform_get_world_scale(Entity ent)
{
    Transform *transform = entitypool_get(pool, ent);
    error_assert(transform);
    return mat3_get_scale(transform->worldmat_cache);
}

Mat3 transform_get_world_matrix(Entity ent)
{
    Transform *transform;

    if (entity_eq(ent, entity_nil))
        return mat3_identity();

    transform = entitypool_get(pool, ent);
    error_assert(transform);
    return transform->worldmat_cache;
}
Mat3 transform_get_matrix(Entity ent)
{
    Transform *transform;

    if (entity_eq(ent, entity_nil))
        return mat3_identity();

    transform = entitypool_get(pool, ent);
    error_assert(transform);
    return transform->mat_cache;
}


Vec2 transform_local_to_world(Entity ent, Vec2 v)
{
    Transform *transform = entitypool_get(pool, ent);
    error_assert(transform);
    return mat3_transform(transform->worldmat_cache, v);
}

unsigned int transform_get_dirty_count(Entity ent)
{
    Transform *transform = entitypool_get(pool, ent);
    error_assert(transform);
    return transform->dirty_count;
}

void transform_set_save_filter_rec(Entity ent, bool filter)
{
    Transform *transform;
    Entity *child;

    entity_set_save_filter(ent, filter);

    transform = entitypool_get(pool, ent);
    error_assert(transform);
    if (transform->children)
        array_foreach(child, transform->children)
            transform_set_save_filter_rec(*child, filter);
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
}
void transform_deinit()
{
    _free_children_arrays();
    entitypool_free(pool);
}

void transform_update_all()
{
    Transform *transform;
    static BBox bbox = { { 0, 0 }, { 0, 0 } };

    entitypool_remove_destroyed(pool, transform_remove);

    /* update edit bbox */
    if (edit_get_enabled())
        entitypool_foreach(transform, pool)
            edit_bboxes_update(transform->pool_elem.ent, bbox);
}

/* save/load for just the children array */
static void _children_save(Transform *t, Serializer *s)
{
    Entity *child;

    if (t->children)
        array_foreach(child, t->children)
            if (entity_get_save_filter(*child))
            {
                loop_continue_save(s);
                entity_save(child, s);
            }
    loop_end_save(s);
}
static void _children_load(Transform *t, Deserializer *s)
{
    Entity *child;

    /* this is a little weird because we want NULL array when no children */
    if (loop_continue_load(s))
    {
        t->children = array_new(Entity);
        do
        {
            child = array_add(t->children);
            entity_load(child, s);
        } while (loop_continue_load(s));
    }
    else
        t->children = NULL;
}

void transform_save_all(Serializer *s)
{
    Transform *transform;

    entitypool_save_foreach(transform, pool, s)
    {
        vec2_save(&transform->position, s);
        scalar_save(&transform->rotation, s);
        vec2_save(&transform->scale, s);

        if (entity_get_save_filter(transform->parent))
            entity_save(&transform->parent, s);
        else
            entity_save(&entity_nil, s);
        _children_save(transform, s);

        mat3_save(&transform->mat_cache, s);
        mat3_save(&transform->worldmat_cache, s);

        uint_save(&transform->dirty_count, s);
    }
}
void transform_load_all(Deserializer *s)
{
    Transform *transform;

    entitypool_load_foreach(transform, pool, s)
    {
        vec2_load(&transform->position, s);
        scalar_load(&transform->rotation, s);
        vec2_load(&transform->scale, s);

        entity_load(&transform->parent, s);
        _children_load(transform, s);

        mat3_load(&transform->mat_cache, s);
        mat3_load(&transform->worldmat_cache, s);

        uint_load(&transform->dirty_count, s);
    }
}

