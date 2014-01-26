#ifndef PHYSICS_H_QZ7KUPT1
#define PHYSICS_H_QZ7KUPT1

#include <stdbool.h>

#include "scalar.h"
#include "entity.h"
#include "vec2.h"

SCRIPT(physics,

        /* global */

        EXPORT void physics_set_gravity(Vec2 g);
        EXPORT Vec2 physics_get_gravity();
        EXPORT void physics_set_simulation_frequency(Scalar freq);
        EXPORT Scalar physics_get_simulation_frequency();


        /* add/remove body */

        EXPORT void physics_add(Entity ent);
        EXPORT void physics_remove(Entity ent);


        /* add/remove shape -- add functions return shape index */

        typedef enum ShapeType ShapeType;
        enum ShapeType
        {
            ST_CIRCLE,
            ST_POLYGON,
        };

        EXPORT unsigned int physics_add_circle_shape(Entity ent, Scalar r,
                Vec2 offset);
        EXPORT unsigned int physics_add_box_shape(Entity ent, Scalar l,
                Scalar b, Scalar r, Scalar t);


        /* dynamics */

        EXPORT void physics_set_static(Entity ent, bool stat);
        EXPORT bool physics_get_static(Entity ent);

        EXPORT void physics_set_mass(Entity ent, Scalar mass);
        EXPORT Scalar physics_get_mass(Entity ent);

        EXPORT void physics_set_freeze_rotation(Entity ent, bool freeze);
        EXPORT bool physics_get_freeze_rotation(Entity ent);

        EXPORT void physics_set_velocity(Entity ent, Vec2 vel);

        EXPORT void physics_reset_forces(Entity ent);
        EXPORT void physics_apply_force(Entity ent, Vec2 force);
        EXPORT void physics_apply_force_at(Entity ent, Vec2 force, Vec2 at);


      )

void physics_init();
void physics_deinit();
void physics_update_all(Scalar dt);
void physics_save_all();
void physics_load_all();

#endif

