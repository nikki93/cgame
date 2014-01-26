#include "physics.h"

#define CP_DATA_POINTER_TYPE Entity
#include <chipmunk.h>

#include "array.h"
#include "entitypool.h"
#include "transform.h"

/* per-entity info */
typedef struct PhysicsInfo PhysicsInfo;
struct PhysicsInfo
{
    EntityPoolElem pool_elem;

    cpBody *body;
    Array *shapes;
};

/* per-shape info for each shape attached to a physics entity */
typedef struct ShapeInfo ShapeInfo;
struct ShapeInfo
{
    ShapeType type;
    cpShape *shape;
};

static cpSpace *space;
static EntityPool *pool;

/* ------------------------------------------------------------------------- */

/* chipmunk utilities */

static inline cpVect cpv_of_vec2(Vec2 v) { return cpv(v.x, v.y); }
static inline Vec2 vec2_of_cpv(cpVect v) { return vec2(v.x, v.y); }

static inline void _remove_body(cpBody *body)
{
    cpSpaceRemoveBody(space, body);
    cpBodyFree(body);
}
static inline void _remove_shape(cpShape *shape)
{
    cpSpaceRemoveShape(space, shape);
    cpShapeFree(shape);
}

/* ------------------------------------------------------------------------- */

void physics_set_gravity(Vec2 g)
{
    cpSpaceSetGravity(space, cpv_of_vec2(g));
}

static inline PhysicsInfo *_add(Entity ent)
{
    PhysicsInfo *info;

    if (entitypool_get(pool, ent))
        return NULL; /* already has physics */

    info = entitypool_add(pool, ent);
    info->shapes = array_new(ShapeInfo);

    return info;
}
void physics_add_dynamic(Entity ent, Scalar mass)
{
    PhysicsInfo *info = _add(ent);

    if (info)
    {
        info->body = cpSpaceAddBody(space, cpBodyNew(mass, 1.0));

        cpBodySetUserData(info->body, ent);

        cpBodySetPos(info->body, cpv_of_vec2(transform_get_position(ent)));
        cpBodySetAngle(info->body, transform_get_rotation(ent));
    }
}
void physics_add_static(Entity ent)
{
    PhysicsInfo *info = _add(ent);

    if (info)
        info->body = NULL;
}

/* remove chipmunk stuff (doesn't remove from pool) */
static void _remove(PhysicsInfo *info)
{
    ShapeInfo *shapeInfo, *end;

    for (shapeInfo = array_begin(info->shapes), end = array_end(info->shapes);
            shapeInfo != end; ++shapeInfo)
        _remove_shape(shapeInfo->shape);

    array_free(info->shapes);
    if (info->body)
        _remove_body(info->body);
}
void physics_remove(Entity ent)
{
    PhysicsInfo *info;

    info = entitypool_get(pool, ent);
    if (!info)
        return;

    _remove(info);
    entitypool_remove(pool, ent);
}

/* calculate moment for a single shape */
static Scalar _moment(cpBody *body, ShapeInfo *shapeInfo)
{
    Scalar mass = cpBodyGetMass(body);
    switch (shapeInfo->type)
    {
        case ST_CIRCLE:
            return cpMomentForCircle(mass, 0,
                    cpCircleShapeGetRadius(shapeInfo->shape),
                    cpCircleShapeGetOffset(shapeInfo->shape));

        case ST_POLYGON:
            return cpMomentForPoly(mass,
                    cpPolyShapeGetNumVerts(shapeInfo->shape),
                    ((cpPolyShape *) shapeInfo->shape)->verts,
                    cpvzero);
    }
}

/* recalculate moment for whole body by adding up shape moments */
static void _recalculate_moment(PhysicsInfo *info)
{
    Scalar moment;
    ShapeInfo *shapeInfo, *end;

    if (!info->body)
        return;

    moment = 0.0;
    for (shapeInfo = array_begin(info->shapes), end = array_end(info->shapes);
            shapeInfo != end; ++shapeInfo)
        moment += _moment(info->body, shapeInfo);

    cpBodySetMoment(info->body, moment);
}

static unsigned int _add_shape(Entity ent, ShapeType type, cpShape *shape)
{
    PhysicsInfo *info;
    ShapeInfo *shapeInfo;

    info = entitypool_get(pool, ent);
    assert(info);

    /* init ShapeInfo */
    shapeInfo = array_add(info->shapes);
    shapeInfo->type = type;
    shapeInfo->shape = shape;

    /* init cpShape */
    cpShapeSetBody(shape, info->body ? info->body : space->staticBody);
    cpSpaceAddShape(space, shape);
    cpShapeSetFriction(shapeInfo->shape, 1);
    cpShapeSetUserData(shapeInfo->shape, ent);

    /* update moment */
    if (info->body)
    {
        if (array_length(info->shapes) > 1)
            cpBodySetMoment(info->body, _moment(info->body, shapeInfo)
                    + cpBodyGetMoment(info->body));
        else
            cpBodySetMoment(info->body, _moment(info->body, shapeInfo));
    }

    return array_length(info->shapes) - 1;
}
unsigned int physics_add_circle_shape(Entity ent, Scalar r,
        Vec2 offset)
{
    return _add_shape(ent, ST_CIRCLE, cpCircleShapeNew(NULL, r,
                cpv_of_vec2(offset)));
}
unsigned int physics_add_box_shape(Entity ent, Scalar l, Scalar b, Scalar r,
        Scalar t)
{
    return _add_shape(ent, ST_POLYGON, cpBoxShapeNew2(NULL,
                cpBBNew(l, b, r, t)));
}

void physics_set_freeze_rotation(Entity ent, bool freeze)
{
    PhysicsInfo *info = entitypool_get(pool, ent);
    assert(info);

    if (freeze)
        cpBodySetMoment(info->body, INFINITY);
    else
        _recalculate_moment(info);
}

void physics_set_velocity(Entity ent, Vec2 vel)
{
    PhysicsInfo *info = entitypool_get(pool, ent);
    assert(info);
    cpBodySetVel(info->body, cpv_of_vec2(vel));
}

void physics_reset_forces(Entity ent)
{
    PhysicsInfo *info = entitypool_get(pool, ent);
    assert(info);
    cpBodyResetForces(info->body);
}
void physics_apply_force(Entity ent, Vec2 force)
{
    physics_apply_force_at(ent, force, vec2_zero);
}
void physics_apply_force_at(Entity ent, Vec2 force, Vec2 at)
{
    PhysicsInfo *info = entitypool_get(pool, ent);
    assert(info);
    cpBodyApplyForce(info->body, cpv_of_vec2(force), cpv_of_vec2(at));
}

/* ------------------------------------------------------------------------- */

void physics_init()
{
    pool = entitypool_new(PhysicsInfo);

    space = cpSpaceNew();
    cpSpaceSetGravity(space, cpv(0, -9.8));
}
void physics_deinit()
{
    PhysicsInfo *info, *end;

    for (info = entitypool_begin(pool), end = entitypool_end(pool);
            info != end; ++info)
        _remove(info);

    cpSpaceFree(space);

    entitypool_free(pool);
}

void physics_update_all(Scalar dt)
{
    PhysicsInfo *info, *end;

    for (info = entitypool_begin(pool); info != entitypool_end(pool); )
        if (entity_destroyed(info->pool_elem.ent))
            physics_remove(info->pool_elem.ent);
        else
            ++info;

    cpSpaceStep(space, dt);

    for (info = entitypool_begin(pool), end = entitypool_end(pool);
            info != end; ++info)
        if (info->body)
        {
            transform_set_position(info->pool_elem.ent,
                    vec2_of_cpv(cpBodyGetPos(info->body)));
            transform_set_rotation(info->pool_elem.ent,
                    cpBodyGetAngle(info->body));
        }
}

void physics_save_all()
{
}
void physics_load_all()
{
}

