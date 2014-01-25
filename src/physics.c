#include "physics.h"

#include <chipmunk.h>

#include "entitypool.h"
#include "transform.h"

static cpSpace *space;

typedef struct PhysicsInfo PhysicsInfo;
struct PhysicsInfo
{
    EntityPoolElem pool_elem;

    cpBody *body;
    cpShape *shape;
};

static EntityPool *pool;

/* cpVect <-> Vec2 conversion */
static inline cpVect cpv_of_vec2(Vec2 v) { return cpv(v.x, v.y); }
static inline Vec2 vec2_of_cpv(cpVect v) { return vec2(v.x, v.y); }

/* ------------------------------------------------------------------------- */

void physics_set_gravity(Vec2 g)
{
    cpSpaceSetGravity(space, cpv_of_vec2(g));
}

void physics_add_dynamic(Entity ent, Scalar mass)
{
    PhysicsInfo *info;

    if (entitypool_get(pool, ent))
        return; /* already has physics */

    info = entitypool_add(pool, ent);
    info->body = cpSpaceAddBody(space, cpBodyNew(mass, 1.0));

    cpBodySetPos(info->body, cpv_of_vec2(transform_get_position(ent)));
    cpBodySetAngle(info->body, transform_get_rotation(ent));
}
void physics_add_static(Entity ent)
{
    PhysicsInfo *info;

    if (entitypool_get(pool, ent))
        return; /* already has physics */

    info = entitypool_add(pool, ent);
    info->body = NULL;
}
void physics_remove(Entity ent)
{
    PhysicsInfo *info = entitypool_get(pool, ent);

    if (!info)
        return;

    if (info->body)
    {
        cpSpaceRemoveBody(space, info->body);
        cpBodyFree(info->body);
    }
    if (info->shape)
    {
        cpSpaceRemoveShape(space, info->shape);
        cpShapeFree(info->shape);
    }

    entitypool_remove(pool, ent);
}

void physics_add_box_shape(Entity ent, Scalar l, Scalar b, Scalar r, Scalar t)
{
    PhysicsInfo *info;
    cpBody *body;
    cpBB bb;

    info = entitypool_get(pool, ent);
    assert(info);

    bb = cpBBNew(l, b, r, t);

    if ((body = info->body))
        cpBodySetMoment(body, cpMomentForBox2(cpBodyGetMass(body), bb));
    else
        body = space->staticBody;

    info->shape = cpSpaceAddShape(space, cpBoxShapeNew2(body, bb));
    cpShapeSetFriction(info->shape, 1);
}

void physics_freeze_rotation(Entity ent)
{
    PhysicsInfo *info = entitypool_get(pool, ent);
    assert(info);
    cpBodySetMoment(info->body, INFINITY);
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

    cpSpaceFree(space);

    for (info = entitypool_begin(pool), end = entitypool_end(pool);
            info != end; ++info)
    {
        cpBodyFree(info->body);
        cpShapeFree(info->shape);
    }

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

