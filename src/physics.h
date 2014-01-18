#ifndef PHYSICS_H_QZ7KUPT1
#define PHYSICS_H_QZ7KUPT1

#include "scalar.h"
#include "entity.h"
#include "vec2.h"

SCRIPT(physics,

        EXPORT void physics_set_gravity(Vec2 g);

        EXPORT void physics_add_static(Entity ent);
        EXPORT void physics_add_dynamic(Entity ent, Scalar mass);

        EXPORT void physics_add_box_shape(Entity ent, Scalar l, Scalar b,
            Scalar r, Scalar t);

        EXPORT void physics_set_velocity(Entity ent, Vec2 vel);

      )

void physics_init();
void physics_deinit();
void physics_update_all(Scalar dt);
void physics_save_all();
void physics_load_all();

#endif

