#include "camera.h"

#include "saveload.h"
#include "transform.h"

static Entity camera_entity;

static Mat3 inverse_view_matrix;

/* ------------------------------------------------------------------------- */

void camera_add(Entity ent)
{
    transform_add(ent);

    camera_entity = ent;
}
void camera_remove(Entity ent)
{
    if (entity_eq(camera_entity, ent))
        camera_entity = entity_nil;
}
Entity camera_get()
{
    return camera_entity;
}

void camera_set_viewport_size(Entity ent, Vec2 dim)
{
    if (entity_eq(camera_entity, ent))
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
    camera_entity = entity_nil;
    inverse_view_matrix = mat3_identity();
}

void camera_clear()
{
    camera_remove(camera_entity);
}

void camera_update_all()
{
    if (!entity_eq(camera_entity, entity_nil))
    {
        if (entity_destroyed(camera_entity))
        {
            camera_remove(camera_entity);
            return;
        }

        inverse_view_matrix = mat3_inverse(
            transform_get_world_matrix(camera_entity));
    }
}

void camera_save_all(Serializer *s)
{
    entity_save(&camera_entity, s);
    mat3_save(&inverse_view_matrix, s);
}
void camera_load_all(Deserializer *s)
{
    entity_load(&camera_entity, s);
    mat3_load(&inverse_view_matrix, s);
}

