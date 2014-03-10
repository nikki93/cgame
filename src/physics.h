#ifndef PHYSICS_H
#define PHYSICS_H

#include <stdbool.h>

#include "saveload.h"
#include "script_export.h"
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

       typedef enum PhysicsBody PhysicsBody;
       enum PhysicsBody
       {
           PB_STATIC,      /* never (or rarely) moves -- eg. wall */

           PB_KINEMATIC,   /* moves but not dynamic -- eg. moving platform,
                              can be moved through transform system */

           PB_DYNAMIC,     /* moves subject to dynamics -- eg. bowling pin,
                              avoid moving through transform system
                              use forces, velocities, etc. instead */
       };

       EXPORT void physics_add(Entity ent); /* PB_DYNAMIC by default */
       EXPORT void physics_remove(Entity ent);


       /* add/remove shape -- add functions return shape index */

       typedef enum PhysicsShape PhysicsShape;
       enum PhysicsShape
       {
           PS_CIRCLE,
           PS_POLYGON,
       };

       EXPORT unsigned int physics_add_circle_shape(Entity ent, Scalar r,
                                                    Vec2 offset);
       EXPORT unsigned int physics_add_box_shape(Entity ent, Scalar l,
                                                 Scalar b, Scalar r, Scalar t);


       /* dynamics */

       EXPORT void physics_set_type(Entity ent, PhysicsBody type);
       EXPORT PhysicsBody physics_get_type(Entity ent);

       EXPORT void physics_set_mass(Entity ent, Scalar mass);
       EXPORT Scalar physics_get_mass(Entity ent);

       EXPORT void physics_set_freeze_rotation(Entity ent, bool freeze);
       EXPORT bool physics_get_freeze_rotation(Entity ent);

       EXPORT void physics_set_velocity(Entity ent, Vec2 vel);
       EXPORT Vec2 physics_get_velocity(Entity ent);

       EXPORT void physics_reset_forces(Entity ent);
       EXPORT void physics_apply_force(Entity ent, Vec2 force);
       EXPORT void physics_apply_force_at(Entity ent, Vec2 force, Vec2 at);


       /* nearest query */

       typedef struct NearestResult NearestResult;
       struct NearestResult
       {
           Entity ent; /* closest entity or entity_nil if none in range */
           Vec2 p; /* closest point on shape surface */
           Scalar d; /* distance to point, negative if inside */
           Vec2 g; /* gradient of distance function */
       };

       EXPORT NearestResult physics_nearest(Vec2 point, Scalar max_dist);


    )

void physics_init();
void physics_deinit();
void physics_clear();
void physics_update_all();
void physics_save_all(Serializer *s);
void physics_load_all(Deserializer *s);

#endif

