#include "physics.h"

#include <assert.h>
#include <stdlib.h>
#define CP_DATA_POINTER_TYPE Entity
#include <chipmunk.h>

#include "array.h"
#include "entitypool.h"
#include "timing.h"
#include "transform.h"
#include "gfx.h"
#include "dirs.h"
#include "camera.h"
#include "edit.h"

/* per-entity info */
typedef struct PhysicsInfo PhysicsInfo;
struct PhysicsInfo
{
    EntityPoolElem pool_elem;

    PhysicsBody type;

    /* store mass separately to convert to/from PB_DYNAMIC */
    Scalar mass;

    /* used to compute (angular) velocitiy for PB_KINEMATIC */
    cpVect last_pos;
    cpFloat last_ang;

    /* used to keep track of transform <-> physics update */
    unsigned int last_dirty_count;

    cpBody *body;
    Array *shapes;

    bool debug_draw; /* whether to draw shape for debugging */
};

/* per-shape info for each shape attached to a physics entity */
typedef struct ShapeInfo ShapeInfo;
struct ShapeInfo
{
    PhysicsShape type;
    cpShape *shape;
};

static cpSpace *space;
static Scalar period = 1.0 / 60.0; /* 1.0 / simulation_frequency */
static EntityPool *pool;

/* ------------------------------------------------------------------------- */

/* chipmunk utilities */

static inline cpVect cpv_of_vec2(Vec2 v) { return cpv(v.x, v.y); }
static inline Vec2 vec2_of_cpv(cpVect v) { return vec2(v.x, v.y); }

static inline void _remove_body(cpBody *body)
{
    if (cpSpaceContainsBody(space, body))
        cpSpaceRemoveBody(space, body);
    cpBodyFree(body);
}
static inline void _remove_shape(cpShape *shape)
{
    if (cpSpaceContainsShape(space, shape))
        cpSpaceRemoveShape(space, shape);
    cpShapeFree(shape);
}

/* ------------------------------------------------------------------------- */

void physics_set_gravity(Vec2 g)
{
    cpSpaceSetGravity(space, cpv_of_vec2(g));
}
Vec2 physics_get_gravity()
{
    return vec2_of_cpv(cpSpaceGetGravity(space));
}

void physics_set_simulation_frequency(Scalar freq)
{
    period = 1.0 / freq;
}
Scalar physics_get_simulation_frequency()
{
    return period;
}

void physics_add(Entity ent)
{
    PhysicsInfo *info;

    if (entitypool_get(pool, ent))
        return; /* already has physics */

    transform_add(ent);

    info = entitypool_add(pool, ent);

    info->mass = 1.0;
    info->type = PB_DYNAMIC;

    /* create, init cpBody */
    info->body = cpSpaceAddBody(space, cpBodyNew(info->mass, 1.0));
    cpBodySetUserData(info->body, ent); /* for cpBody -> Entity mapping */
    cpBodySetPos(info->body, cpv_of_vec2(transform_get_position(ent)));
    cpBodySetAngle(info->body, transform_get_rotation(ent));
    info->last_dirty_count = transform_get_dirty_count(ent);

    /* initially no shapes */
    info->shapes = array_new(ShapeInfo);

    /* initialize last_pos/last_ang info for kinematic bodies */
    info->last_pos = cpBodyGetPos(info->body);
    info->last_ang = cpBodyGetAngle(info->body);

    info->debug_draw = false;
}

/* remove chipmunk stuff (doesn't remove from pool) */
static void _remove(PhysicsInfo *info)
{
    ShapeInfo *shapeInfo;

    array_foreach(shapeInfo, info->shapes)
        _remove_shape(shapeInfo->shape);
    array_free(info->shapes);

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
        case PS_CIRCLE:
            return cpMomentForCircle(mass, 0,
                                     cpCircleShapeGetRadius(shapeInfo->shape),
                                     cpCircleShapeGetOffset(shapeInfo->shape));

        case PS_POLYGON:
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
    ShapeInfo *shapeInfo;

    if (!info->body)
        return;
    if (array_length(info->shapes) == 0)
        return; /* can't set moment to zero, just leave it alone */

    moment = 0.0;
    array_foreach(shapeInfo, info->shapes)
        moment += _moment(info->body, shapeInfo);
    cpBodySetMoment(info->body, moment);
}

static void _set_type(PhysicsInfo *info, PhysicsBody type)
{
    if (info->type == type)
        return; /* already set */

    info->type = type;
    switch (type)
    {
        case PB_KINEMATIC:
            info->last_pos = cpBodyGetPos(info->body);
            info->last_ang = cpBodyGetAngle(info->body);
            /* fall through */

        case PB_STATIC:
            if (!cpBodyIsStatic(info->body))
            {
                cpSpaceRemoveBody(space, info->body);
                cpSpaceConvertBodyToStatic(space, info->body);
            }
            break;

        case PB_DYNAMIC:
            cpSpaceConvertBodyToDynamic(space, info->body, info->mass, 1.0);
            cpSpaceAddBody(space, info->body);
            _recalculate_moment(info);
            break;
    }
}
void physics_set_type(Entity ent, PhysicsBody type)
{
    PhysicsInfo *info = entitypool_get(pool, ent);
    assert(info);
    _set_type(info, type);
}
PhysicsBody physics_get_type(Entity ent)
{
    PhysicsInfo *info = entitypool_get(pool, ent);
    assert(info);
    return info->type;
}

void physics_set_debug_draw(Entity ent, bool debug)
{
    PhysicsInfo *info = entitypool_get(pool, ent);
    assert(info);
    info->debug_draw = debug;
}
bool physics_get_debug_draw(Entity ent)
{
    PhysicsInfo *info = entitypool_get(pool, ent);
    assert(info);
    return info->debug_draw;
}

static unsigned int _add_shape(Entity ent, PhysicsShape type, cpShape *shape)
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
    cpShapeSetBody(shape, info->body);
    cpSpaceAddShape(space, shape);
    cpShapeSetFriction(shapeInfo->shape, 1);
    cpShapeSetUserData(shapeInfo->shape, ent);

    /* update moment */
    if (!cpBodyIsStatic(info->body))
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
    cpShape *shape = cpCircleShapeNew(NULL, r, cpv_of_vec2(offset));
    return _add_shape(ent, PS_CIRCLE, shape);
}
unsigned int physics_add_box_shape(Entity ent, Scalar l, Scalar b, Scalar r,
                                   Scalar t)
{
    cpShape *shape = cpBoxShapeNew2(NULL, cpBBNew(l, b, r, t));
    return _add_shape(ent, PS_POLYGON, shape);
}

void physics_set_mass(Entity ent, Scalar mass)
{
    PhysicsInfo *info = entitypool_get(pool, ent);
    assert(info);

    if (mass <= SCALAR_EPSILON)
        return;

    cpBodySetMass(info->body, info->mass = mass);
    _recalculate_moment(info);
}
Scalar physics_get_mass(Entity ent)
{
    PhysicsInfo *info = entitypool_get(pool, ent);
    assert(info);
    return info->mass;
}

void physics_set_freeze_rotation(Entity ent, bool freeze)
{
    PhysicsInfo *info = entitypool_get(pool, ent);
    assert(info);

    if (freeze)
        cpBodySetMoment(info->body, SCALAR_INFINITY);
    else
        _recalculate_moment(info);
}
bool physics_get_freeze_rotation(Entity ent)
{
    PhysicsInfo *info = entitypool_get(pool, ent);
    assert(info);

    /* TODO: do this a better way? maybe store separate flag */
    return cpBodyGetMoment(info->body) == SCALAR_INFINITY;
}

void physics_set_position(Entity ent, Vec2 pos)
{
    PhysicsInfo *info = entitypool_get(pool, ent);
    assert(info);
    cpBodySetPos(info->body, cpv_of_vec2(pos));
    cpSpaceReindexShapesForBody(space, info->body);
}
Vec2 physics_get_position(Entity ent)
{
    PhysicsInfo *info = entitypool_get(pool, ent);
    assert(info);
    return vec2_of_cpv(cpBodyGetPos(info->body));
}

void physics_set_velocity(Entity ent, Vec2 vel)
{
    PhysicsInfo *info = entitypool_get(pool, ent);
    assert(info);
    cpBodySetVel(info->body, cpv_of_vec2(vel));
}
Vec2 physics_get_velocity(Entity ent)
{
    PhysicsInfo *info = entitypool_get(pool, ent);
    assert(info);
    return vec2_of_cpv(cpBodyGetVel(info->body));
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

NearestResult physics_nearest(Vec2 point, Scalar max_dist)
{
    cpNearestPointQueryInfo info;
    NearestResult res;

    if (!cpSpaceNearestPointQueryNearest(space, cpv_of_vec2(point), max_dist,
                                         CP_ALL_LAYERS, CP_NO_GROUP, &info))
    {
        /* no result */
        res.ent = entity_nil;
        return res;
    }

    res.ent = cpShapeGetUserData(info.shape);
    res.p = vec2_of_cpv(info.p);
    res.d = info.d;
    res.g = vec2_of_cpv(info.g);
    return res;
}

/* --- init/deinit --------------------------------------------------------- */

static GLuint program;
static GLuint vao;
static GLuint vbo;

void physics_init()
{
    /* init pool */
    pool = entitypool_new(PhysicsInfo);

    /* init cpSpace */
    space = cpSpaceNew();
    cpSpaceSetGravity(space, cpv(0, -9.8));

    /* init draw stuff */
    program = gfx_create_program(data_path("phypoly.vert"),
                                 NULL,
                                 data_path("phypoly.frag"));
    glUseProgram(program);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    gfx_bind_vertex_attrib(program, GL_FLOAT, 2, "position", Vec2, x);
}
void physics_deinit()
{
    PhysicsInfo *info;

    /* clean up draw stuff */
    glDeleteProgram(program);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

    /* remove all bodies, shapes */
    entitypool_foreach(info, pool)
        _remove(info);

    /* deinit cpSpace */
    cpSpaceFree(space);

    /* deinit pool */
    entitypool_free(pool);
}

/* --- update -------------------------------------------------------------- */

/* step the space with fixed time step */
static void _step()
{
    static Scalar remain = 0.0;

    remain += timing_dt;
    while (remain >= period)
    {
        cpSpaceStep(space, period);
        remain -= period;
    }
}

static void _update_kinematics()
{
    PhysicsInfo *info;
    cpVect pos;
    cpFloat ang;
    Scalar invdt;
    Entity ent;

    if (timing_dt <= FLT_EPSILON)
        return;
    invdt = 1 / timing_dt;

    entitypool_foreach(info, pool)
        if (info->type == PB_KINEMATIC)
        {
            ent = info->pool_elem.ent;

            /* move to transform */
            pos = cpv_of_vec2(transform_get_position(ent));
            ang = transform_get_rotation(ent);
            cpBodySetPos(info->body, pos);
            cpBodySetAngle(info->body, ang);
            info->last_dirty_count = transform_get_dirty_count(ent);

            /* update linear, angular velocities based on delta */
            cpBodySetVel(info->body,
                         cpvmult(cpvsub(pos, info->last_pos), invdt));
            cpBodySetAngVel(info->body, (ang - info->last_ang) * invdt);
            cpSpaceReindexShapesForBody(space, info->body);

            /* save current state for next computation */
            info->last_pos = pos;
            info->last_ang = ang;
        }
}
void physics_update_all()
{
    PhysicsInfo *info;
    Entity ent;

    entitypool_remove_destroyed(pool, physics_remove);

    /* simulate */
    if (!timing_get_paused())
    {
        _update_kinematics();
        _step();
    }

    /* synchronize transform <-> physics */
    entitypool_foreach(info, pool)
    {
        ent = info->pool_elem.ent;

        /* if transform is dirtier, move to it, else overwrite it */
        if (transform_get_dirty_count(ent) != info->last_dirty_count)
        {
            cpBodySetVel(info->body, cpvzero);
            cpBodySetAngVel(info->body, 0.0f);
            cpBodySetPos(info->body, cpv_of_vec2(transform_get_position(ent)));
            cpBodySetAngle(info->body, transform_get_rotation(ent));
            cpSpaceReindexShapesForBody(space, info->body);
        }
        else
        {
            transform_set_position(ent, vec2_of_cpv(cpBodyGetPos(info->body)));
            transform_set_rotation(ent, cpBodyGetAngle(info->body));
        }

        info->last_dirty_count = transform_get_dirty_count(ent);
    }
}

/* --- draw ---------------------------------------------------------------- */

static void _circle_draw(PhysicsInfo *info, ShapeInfo *shapeInfo)
{
    /* TODO: fill this in */
}

static void _polygon_draw(PhysicsInfo *info, ShapeInfo *shapeInfo)
{
    unsigned int i, nverts;
    Vec2 *verts;
    Mat3 wmat;
    
    wmat = transform_get_world_matrix(info->pool_elem.ent);
    glUniformMatrix3fv(glGetUniformLocation(program, "wmat"),
                       1, GL_FALSE, (const GLfloat *) &wmat);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    /* copy as Vec2 array */
    nverts = cpPolyShapeGetNumVerts(shapeInfo->shape);
    verts = malloc(nverts * sizeof(Vec2));
    for (i = 0; i < nverts; ++i)
        verts[i] = vec2_of_cpv(cpPolyShapeGetVert(shapeInfo->shape, i));
    glBufferData(GL_ARRAY_BUFFER, nverts * sizeof(Vec2),
                 verts, GL_STREAM_DRAW);
    glDrawArrays(GL_LINE_LOOP, 0, nverts);
    free(verts);
}

void physics_draw_all()
{
    PhysicsInfo *info;
    ShapeInfo *shapeInfo;

    if (!edit_get_enabled())
        return;

    /* bind program, update uniforms */
    glUseProgram(program);
    glUniformMatrix3fv(glGetUniformLocation(program, "inverse_view_matrix"),
                       1, GL_FALSE,
                       (const GLfloat *) camera_get_inverse_view_matrix_ptr());

    /* draw! */
    entitypool_foreach(info, pool)
        if (info->debug_draw)
            array_foreach(shapeInfo, info->shapes)
                switch(shapeInfo->type)
                {
                    case PS_CIRCLE:
                        _circle_draw(info, shapeInfo);
                        break;

                    case PS_POLYGON:
                        _polygon_draw(info, shapeInfo);
                        break;
                }
}

/* --- save/load ----------------------------------------------------------- */

/* chipmunk data save/load helpers */
static void _cpv_save(cpVect *cv, Serializer *s)
{
    Vec2 v = vec2_of_cpv(*cv);
    vec2_save(&v, s);
}
static void _cpv_load(cpVect *cv, Deserializer *s)
{
    Vec2 v;
    vec2_load(&v, s);
    *cv = cpv_of_vec2(v);
}
static void _cpf_save(cpFloat *cf, Serializer *s)
{
    Scalar f = *cf;
    scalar_save(&f, s);
}
static void _cpf_load(cpFloat *cf, Deserializer *s)
{
    Scalar f;
    scalar_load(&f, s);
    *cf = f;
}

/*
 * note that UserData isn't directly save/load'd -- it is restored to
 * the Entity value on load separately
 */

/* some hax to reduce typing for body properties save/load */
#define body_prop_save(type, f, prop)                                   \
    { type v; v = cpBodyGet##prop(info->body); f##_save(&v, s); }
#define body_prop_load(type, f, prop)                                   \
    { type v; f##_load(&v, s); cpBodySet##prop(info->body, v); }
#define body_props_saveload(saveload)                   \
    body_prop_##saveload(cpFloat, _cpf, Mass);          \
    body_prop_##saveload(cpFloat, _cpf, Moment);        \
    /* body_prop_##saveload(cpVect, _cpv, Pos); */      \
    body_prop_##saveload(cpVect, _cpv, Vel);            \
    body_prop_##saveload(cpVect, _cpv, Force);          \
    /* body_prop_##saveload(cpFloat, _cpf, Angle); */   \
    body_prop_##saveload(cpFloat, _cpf, AngVel);        \
    body_prop_##saveload(cpFloat, _cpf, Torque);        \
    body_prop_##saveload(cpFloat, _cpf, VelLimit);      \
    body_prop_##saveload(cpFloat, _cpf, AngVelLimit);   \

/* save/load for just the body in a PhysicsInfo */
static void _body_save(PhysicsInfo *info, Serializer *s)
{
    body_props_saveload(save);
}
static void _body_load(PhysicsInfo *info, Deserializer *s)
{
    Entity ent;
    PhysicsBody type;

    /* create, restore properties */
    ent = info->pool_elem.ent;
    info->body = cpSpaceAddBody(space, cpBodyNew(info->mass, 1.0));
    body_props_saveload(load);
    cpBodySetUserData(info->body, ent);

    /* force type change if non-default */
    type = info->type;
    info->type = PB_DYNAMIC;
    _set_type(info, type);

    /* restore position, angle based on transform */
    cpBodySetPos(info->body, cpv_of_vec2(transform_get_position(ent)));
    cpBodySetAngle(info->body, transform_get_rotation(ent));
    info->last_dirty_count = transform_get_dirty_count(info->pool_elem.ent);
}

/* save/load for special properties of each shape type */

static void _circle_save(PhysicsInfo *info, ShapeInfo *shapeInfo,
                         Serializer *s)
{
    cpFloat radius;
    cpVect offset;

    radius = cpCircleShapeGetRadius(shapeInfo->shape);
    _cpf_save(&radius, s);
    offset = cpCircleShapeGetOffset(shapeInfo->shape);
    _cpv_save(&offset, s);
}
static void _circle_load(PhysicsInfo *info, ShapeInfo *shapeInfo,
                         Deserializer *s)
{
    cpFloat radius;
    cpVect offset;

    _cpf_load(&radius, s);
    _cpv_load(&offset, s);

    shapeInfo->shape = cpCircleShapeNew(info->body, radius, offset);
    cpSpaceAddShape(space, shapeInfo->shape);
}

static void _polygon_save(PhysicsInfo *info, ShapeInfo *shapeInfo,
                          Serializer *s)
{
    unsigned int n, i;
    cpVect v;

    n = cpPolyShapeGetNumVerts(shapeInfo->shape);
    uint_save(&n, s);

    for (i = 0; i < n; ++i)
    {
        v = cpPolyShapeGetVert(shapeInfo->shape, i);
        _cpv_save(&v, s);
    }
}
static void _polygon_load(PhysicsInfo *info, ShapeInfo *shapeInfo,
                          Deserializer *s)
{
    unsigned int n, i;
    cpVect *vs;

    uint_load(&n, s);

    vs = malloc(n * sizeof(cpVect));
    for (i = 0; i < n; ++i)
        _cpv_load(&vs[i], s);
    shapeInfo->shape = cpPolyShapeNew(info->body, n, vs, cpvzero);
    cpSpaceAddShape(space, shapeInfo->shape);
    free(vs);
}

/* some hax to reduce typing for shape properties save/load */
#define shape_prop_save(type, f, prop)                                  \
    { type v; v = cpShapeGet##prop(shapeInfo->shape); f##_save(&v, s); }
#define shape_prop_load(type, f, prop)                                  \
    { type v; f##_load(&v, s); cpShapeSet##prop(shapeInfo->shape, v); }
#define shape_props_saveload(saveload)                          \
    shape_prop_##saveload(bool, bool, Sensor);                  \
    shape_prop_##saveload(cpFloat, _cpf, Elasticity);           \
    shape_prop_##saveload(cpFloat, _cpf, Friction);             \
    shape_prop_##saveload(cpVect, _cpv, SurfaceVelocity);       \
    shape_prop_##saveload(unsigned int, uint, CollisionType);   \
    shape_prop_##saveload(unsigned int, uint, Group);           \
    shape_prop_##saveload(unsigned int, uint, Layers);          \

/* save/load for all shapes in a PhysicsInfo */
static void _shapes_save(PhysicsInfo *info, Serializer *s)
{
    unsigned int n;
    ShapeInfo *shapeInfo;

    n = array_length(info->shapes);
    uint_save(&n, s);

    array_foreach(shapeInfo, info->shapes)
    {
        /* type-specific properties */
        enum_save(&shapeInfo->type, s);
        switch (shapeInfo->type)
        {
            case PS_CIRCLE:
                _circle_save(info, shapeInfo, s);
                break;

            case PS_POLYGON:
                _polygon_save(info, shapeInfo, s);
                break;
        }

        /* common properties */
        shape_props_saveload(save);
    }

}
static void _shapes_load(PhysicsInfo *info, Deserializer *s)
{
    unsigned int n;
    ShapeInfo *shapeInfo;

    uint_load(&n, s);
    info->shapes = array_new(ShapeInfo);
    array_reset(info->shapes, n);

    array_foreach(shapeInfo, info->shapes)
    {
        /* type-specific properties */
        enum_load(&shapeInfo->type, s);
        switch (shapeInfo->type)
        {
            case PS_CIRCLE:
                _circle_load(info, shapeInfo, s);
                break;

            case PS_POLYGON:
                _polygon_load(info, shapeInfo, s);
                break;
        }

        /* common properties */
        shape_props_saveload(load);
        cpShapeSetUserData(shapeInfo->shape, info->pool_elem.ent);
    }
}

/* 
 * save/load for all data in a PhysicsInfo other than the the actual body,
 * shapes is handled here, the rest is done in functions above
 */
void physics_save_all(Serializer *s)
{
    PhysicsInfo *info;

    entitypool_save_foreach(info, pool, s)
    {
        enum_save(&info->type, s);
        scalar_save(&info->mass, s);

        _body_save(info, s);
        _shapes_save(info, s);

        bool_save(&info->debug_draw, s);
    }
}
void physics_load_all(Deserializer *s)
{
    PhysicsInfo *info;

    entitypool_load_foreach(info, pool, s)
    {
        enum_load(&info->type, s);
        scalar_load(&info->mass, s);

        _body_load(info, s);
        _shapes_load(info, s);

        /* set last_pos/last_ang info for kinematic bodies */
        info->last_pos = cpBodyGetPos(info->body);
        info->last_ang = cpBodyGetAngle(info->body);

        bool_load(&info->debug_draw, s);
    }
}

