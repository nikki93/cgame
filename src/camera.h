#ifndef CAMERA_H
#define CAMERA_H

#include "saveload.h"
#include "entity.h"
#include "vec2.h"
#include "mat3.h"
#include "script_export.h"

/* for now there's just one camera */

SCRIPT(camera,

       EXPORT void camera_add(Entity ent);
       EXPORT void camera_remove(Entity ent);
       EXPORT Entity camera_get(); /* return current camera, or
                                      entity_nil if doesn't exist */

       EXPORT void camera_set_viewport_size(Entity ent, Vec2 dim);
       EXPORT Mat3 camera_get_inverse_view_matrix();

       /* screen-space coordinates <-> world coordinates transformations */
       EXPORT Vec2 camera_world_to_pixels(Vec2 p);
       EXPORT Vec2 camera_world_to_unit(Vec2 p);
       EXPORT Vec2 camera_pixels_to_world(Vec2 p);
       EXPORT Vec2 camera_unit_to_world(Vec2 p);

    )

const Mat3 *camera_get_inverse_view_matrix_ptr(); /* for quick GLSL binding */

void camera_init();
void camera_clear();
void camera_update_all();
void camera_save_all(Serializer *s);
void camera_load_all(Deserializer *s);

#endif

