#include "physics.h"

#include <chipmunk.h>

#include "transform.h"

static cpSpace *space;

typedef struct PhysicsInfo PhysicsInfo;
struct PhysicsInfo
{
    Entity ent;

    cpBody *body;
    cpShape *shape;
};

#define PHY_MAX 1024
PhysicsInfo physics_infos[PHY_MAX] = { 0 };

static inline cpVect cpv_of_vec2(Vec2 v) { return cpv(v.x, v.y); }
static inline Vec2 vec2_of_cpv(cpVect v) { return vec2(v.x, v.y); }

/* ------------------------------------------------------------------------- */

void physics_set_gravity(Vec2 g)
{
    cpSpaceSetGravity(space, cpv_of_vec2(g));
}

void physics_add_dynamic(Entity ent, Scalar mass)
{
    physics_infos[ent].body = cpSpaceAddBody(space, cpBodyNew(mass, 1.0));
    cpBodySetPos(physics_infos[ent].body,
            cpv_of_vec2(transform_get_position(ent)));
    cpBodySetAngle(physics_infos[ent].body, transform_get_rotation(ent));

    if (physics_infos[ent].shape)
        cpShapeSetBody(physics_infos[ent].shape, physics_infos[ent].body);
}
void physics_add_static(Entity ent)
{
}

void physics_add_box_shape(Entity ent, Scalar l, Scalar b, Scalar r, Scalar t)
{
    cpBody *body;
    cpBB bb;

    bb = cpBBNew(l, b, r, t);

    if ((body = physics_infos[ent].body))
        cpBodySetMoment(body, cpMomentForBox2(cpBodyGetMass(body), bb));
    else
        body = space->staticBody;

    physics_infos[ent].shape =
        cpSpaceAddShape(space, cpBoxShapeNew2(body, bb));
    cpShapeSetFriction(physics_infos[ent].shape, 1);
}

void physics_set_velocity(Entity ent, Vec2 vel)
{
    cpBodySetVel(physics_infos[ent].body, cpv_of_vec2(vel));
}

/* ------------------------------------------------------------------------- */

void physics_init()
{
    space = cpSpaceNew();
    cpSpaceSetGravity(space, cpv(0, -9.8));
}

void physics_deinit()
{
    cpSpaceFree(space);

    for (unsigned int i = 0; i < PHY_MAX; ++i)
    {
        cpBodyFree(physics_infos[i].body);
        cpShapeFree(physics_infos[i].shape);
    }
}

void physics_update_all(Scalar dt)
{
    cpSpaceStep(space, dt);

    for (unsigned int i = 0; i < PHY_MAX; ++i)
        if (physics_infos[i].body)
        {
            transform_set_position(i,
                    vec2_of_cpv(cpBodyGetPos(physics_infos[i].body)));
            transform_set_rotation(i, cpBodyGetAngle(physics_infos[i].body));
        }
}

void physics_save_all()
{
}
void physics_load_all()
{
}

