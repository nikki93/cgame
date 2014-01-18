#include "physics.h"

#include <chipmunk.h>

#include "entitymap.h"
#include "array.h"
#include "transform.h"

static cpSpace *space;

typedef struct PhysicsInfo PhysicsInfo;
struct PhysicsInfo
{
    Entity ent;

    cpBody *body;
    cpShape *shape;
};

static EntityMap *emap;
static Array *infos;

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

    if (entitymap_get(emap, ent) >= 0)
        return; /* already has physics */

    info = array_add(infos);
    info->ent = ent;

    info->body = cpSpaceAddBody(space, cpBodyNew(mass, 1.0));
    cpBodySetPos(info->body, cpv_of_vec2(transform_get_position(ent)));
    cpBodySetAngle(info->body, transform_get_rotation(ent));

    entitymap_set(emap, ent, array_length(infos) - 1);
}
void physics_add_static(Entity ent)
{
    PhysicsInfo *info;

    if (entitymap_get(emap, ent) >= 0)
        return; /* already has physics */

    info = array_add(infos);
    info->ent = ent;

    info->body = NULL;

    entitymap_set(emap, ent, array_length(infos) - 1);
}
void physics_remove(Entity ent)
{
    int i;
    PhysicsInfo *info;

    if ((i = entitymap_get(emap, ent)) >= 0)
    {
        info = array_get(infos, i);

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

        if (array_quick_remove(infos, i))
            entitymap_set(emap,
                    array_get_val(PhysicsInfo, infos, i).ent, i);
        entitymap_set(emap, ent, -1);
    }
}

void physics_add_box_shape(Entity ent, Scalar l, Scalar b, Scalar r, Scalar t)
{
    int i;
    PhysicsInfo *info;
    cpBody *body;
    cpBB bb;

    i = entitymap_get(emap, ent);
    assert(i >= 0);
    info = array_get(infos, i);

    bb = cpBBNew(l, b, r, t);

    if ((body = info->body))
        cpBodySetMoment(body, cpMomentForBox2(cpBodyGetMass(body), bb));
    else
        body = space->staticBody;

    info->shape = cpSpaceAddShape(space, cpBoxShapeNew2(body, bb));
    cpShapeSetFriction(info->shape, 1);
}

#define GET \
    int i = entitymap_get(emap, ent); \
    assert(i >= 0); \
    PhysicsInfo *info = array_get(infos, i);

void physics_freeze_rotation(Entity ent)
{
    GET;
    cpBodySetMoment(info->body, INFINITY);
}

void physics_set_velocity(Entity ent, Vec2 vel)
{
    GET;
    cpBodySetVel(info->body, cpv_of_vec2(vel));
}

void physics_reset_forces(Entity ent)
{
    GET;
    cpBodyResetForces(info->body);
}
void physics_apply_force(Entity ent, Vec2 force)
{
    physics_apply_force_at(ent, force, vec2_zero);
}
void physics_apply_force_at(Entity ent, Vec2 force, Vec2 at)
{
    GET;
    cpBodyApplyForce(info->body, cpv_of_vec2(force), cpv_of_vec2(at));
}

/* ------------------------------------------------------------------------- */

void physics_init()
{
    emap = entitymap_new(-1);
    infos = array_new(PhysicsInfo);

    space = cpSpaceNew();
    cpSpaceSetGravity(space, cpv(0, -9.8));
}
void physics_deinit()
{
    unsigned int n, i;
    PhysicsInfo *info;

    cpSpaceFree(space);

    n = array_length(infos);
    for (i = 0; i < n; ++i)
    {
        info = array_get(infos, i);
        cpBodyFree(info->body);
        cpShapeFree(info->shape);
    }

    array_free(infos);
    entitymap_free(emap);
}

void physics_update_all(Scalar dt)
{
    unsigned int n, i;
    PhysicsInfo *info;

    for (i = 0; i < array_length(infos); )
    {
        info = array_get(infos, i);
        if (entity_destroyed(info->ent))
            physics_remove(info->ent);
        else
            ++i;
    }

    cpSpaceStep(space, dt);

    n = array_length(infos);
    for (i = 0; i < n; ++i)
    {
        info = array_get(infos, i);

        if (info->body)
        {
            transform_set_position(info->ent,
                    vec2_of_cpv(cpBodyGetPos(info->body)));
            transform_set_rotation(info->ent, cpBodyGetAngle(info->body));
        }
    }
}

void physics_save_all()
{
}
void physics_load_all()
{
}

