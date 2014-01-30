#include "camera.h"

#include <stdbool.h>

#include "saveload.h"
#include "transform.h"

static bool camera_exists = false;
static Entity camera_entity;

static Mat3 inverse_view_matrix;

/* ------------------------------------------------------------------------- */

void camera_add(Entity ent)
{
    transform_add(ent);

    camera_exists = true;
    camera_entity = ent;
}
void camera_remove()
{
    camera_exists = false;
}
Entity camera_get()
{
    return camera_entity;
}

void camera_set_viewport_size(Vec2 dim)
{
    if (camera_exists)
        transform_set_scale(camera_entity, vec2_scalar_mul(dim, 0.5f));
}

Mat3 camera_get_inverse_view_matrix()
{
    return inverse_view_matrix;
}

const Mat3 *camera_get_inverse_view_matrix_ptr()
{
    return &inverse_view_matrix;
}

/* ------------------------------------------------------------------------- */

void camera_init()
{
    inverse_view_matrix = mat3_identity();
}

void camera_update_all()
{
    if (camera_exists)
    {
        if (entity_destroyed(camera_entity))
        {
            camera_remove();
            return;
        }

        inverse_view_matrix = mat3_inverse(
            transform_get_world_matrix(camera_entity));
    }
}

void camera_save_all(Serializer *s)
{
    bool_save(&camera_exists, s);
    entity_save(&camera_entity, s);
    mat3_save(&inverse_view_matrix, s);
}
void camera_load_all(Deserializer *s)
{
    bool_load(&camera_exists, s);
    entity_load(&camera_entity, s);
    mat3_load(&inverse_view_matrix, s);
}

