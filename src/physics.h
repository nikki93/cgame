#ifndef PHYSICS_H_QZ7KUPT1
#define PHYSICS_H_QZ7KUPT1

#include "scalar.h"
#include "entity.h"
#include "vec2.h"

SCRIPT(physics,

        /* global */

        EXPORT void physics_set_gravity(Vec2 g);


        /* add/remove body */

        EXPORT void physics_add_static(Entity ent);
        EXPORT void physics_add_dynamic(Entity ent, Scalar mass);
        EXPORT void physics_remove(Entity ent);


        /* add/remove shape */

        EXPORT void physics_add_box_shape(Entity ent, Scalar l, Scalar b,
            Scalar r, Scalar t);


        /* dynamics */

        EXPORT void physics_freeze_rotation(Entity ent);

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

