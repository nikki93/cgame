#ifndef PHYSICS_H
#define PHYSICS_H

#include <stdbool.h>

#include "saveload.h"
#include "script_export.h"
#include "scalar.h"
#include "entity.h"
#include "vec2.h"
#include "bbox.h"

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

       EXPORT void physics_set_type(Entity ent, PhysicsBody type);
       EXPORT PhysicsBody physics_get_type(Entity ent);

       /* draws this object for one frame */
       EXPORT void physics_debug_draw(Entity ent);


       /* shape */

       /* shapes are indexed 0 - n */
       typedef enum PhysicsShape PhysicsShape;
       enum PhysicsShape
       {
           PS_CIRCLE,
           PS_POLYGON,
       };

       EXPORT unsigned int physics_shape_add_circle(Entity ent, Scalar r,
                                                    Vec2 offset);
       EXPORT unsigned int physics_shape_add_box(Entity ent, BBox b);

       EXPORT unsigned int physics_get_num_shapes(Entity ent);
       EXPORT PhysicsShape physics_shape_get_type(Entity ent, unsigned int i);
       EXPORT void physics_shape_remove(Entity ent, unsigned int i);

       /* -1 if not a PS_POLYGON */
       EXPORT int physics_poly_get_num_verts(Entity ent, unsigned int i);
       /* won't remove if it's a triangle */
       EXPORT void physics_poly_remove_vert(Entity ent, unsigned int i,
                                            unsigned int j);
       EXPORT void physics_poly_insert_vert(Entity ent, unsigned int i,
                                            unsigned int j, Vec2 pos);
       EXPORT void physics_poly_set_vert(Entity ent, unsigned int i,
                                         unsigned int j, Vec2 pos);


       /* dynamics */

       EXPORT void physics_set_mass(Entity ent, Scalar mass);
       EXPORT Scalar physics_get_mass(Entity ent);

       EXPORT void physics_set_freeze_rotation(Entity ent, bool freeze);
       EXPORT bool physics_get_freeze_rotation(Entity ent);

       EXPORT void physics_set_position(Entity ent, Vec2 vel);
       EXPORT Vec2 physics_get_position(Entity ent);
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
void physics_update_all();
void physics_draw_all();
void physics_save_all(Serializer *s);
void physics_load_all(Deserializer *s);

#endif

