#ifndef CAMERA_H
#define CAMERA_H

#include "saveload.h"
#include "entity.h"
#include "vec2.h"
#include "mat3.h"
#include "script_export.h"

/*
 * if no current camera, the (inverse) view matrix is identity, which means
 * the view is a 2x2 unit box at the center of the world
 */

SCRIPT(camera,

       EXPORT void camera_add(Entity ent);
       EXPORT void camera_remove(Entity ent);
       EXPORT bool camera_has(Entity ent);

       /* set camera to use in edit mode -- not saved/loaded */
       EXPORT void camera_set_edit_camera(Entity ent);

       /* set/get currently active camera -- entity_nil if none */
       EXPORT void camera_set_current(Entity ent, bool current);
       EXPORT bool camera_get_current(Entity ent);
       EXPORT void camera_set_current_camera(Entity ent);
       EXPORT Entity camera_get_current_camera();

       /* number of world units to fit vertically on screen */
       EXPORT void camera_set_viewport_height(Entity ent, Scalar height);
       EXPORT Scalar camera_get_viewport_height(Entity ent);

       EXPORT Mat3 camera_get_inverse_view_matrix();

       /* screen-space coordinates <-> world coordinates transformations */
       EXPORT Vec2 camera_world_to_pixels(Vec2 p);
       EXPORT Vec2 camera_world_to_unit(Vec2 p);
       EXPORT Vec2 camera_pixels_to_world(Vec2 p);
       EXPORT Vec2 camera_unit_to_world(Vec2 p);

    )

const Mat3 *camera_get_inverse_view_matrix_ptr(); /* for quick GLSL binding */

void camera_init();
void camera_deinit();
void camera_update_all();
void camera_save_all(Store *s);
void camera_load_all(Store *s);

#endif

