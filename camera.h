#ifndef CAMERA_H_3PIJHFMW
#define CAMERA_H_3PIJHFMW

#include <stdio.h>

#include "entity.h"
#include "mat3.h"
#include "script.h"

/* for now there's just one camera */

SCRIPT(camera,

        void camera_add(Entity ent);
        void camera_remove();
        Entity camera_get();

        void camera_set_viewport_size(Vec2 dim);
        Mat3 camera_get_inverse_view_matrix();

      )

Mat3 *camera_get_inverse_view_matrix_ptr(); /* for quick GLSL binding */

void camera_init();
void camera_update_all();
void camera_save_all(FILE *file);
void camera_load_all(FILE *file);

#endif

