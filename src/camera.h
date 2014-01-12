#ifndef CAMERA_H_3PIJHFMW
#define CAMERA_H_3PIJHFMW

#include "saveload.h"
#include "entity.h"
#include "mat3.h"
#include "script_export.h"

/* for now there's just one camera */

SCRIPT(camera,

        EXPORT void camera_add(Entity ent);
        EXPORT void camera_remove();
        EXPORT Entity camera_get();

        EXPORT void camera_set_viewport_size(Vec2 dim);
        EXPORT Mat3 camera_get_inverse_view_matrix();

      )

const Mat3 *camera_get_inverse_view_matrix_ptr(); /* for quick GLSL binding */

void camera_init();
void camera_update_all();
void camera_save_all(Serializer *s);
void camera_load_all(Deserializer *s);

#endif

