#include "camera.h"

#include "saveload.h"
#include "transform.h"
#include "game.h"
#include "assert.h"

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

Vec2 camera_world_to_pixels(Vec2 p)
{
    return game_unit_to_pixels(camera_world_to_unit(p));
}
Vec2 camera_world_to_unit(Vec2 p)
{
    /* use cached inverse view matrix */
    return mat3_transform(inverse_view_matrix, p);
}
Vec2 camera_pixels_to_world(Vec2 p)
{
    return camera_unit_to_world(game_pixels_to_unit(p));
}
Vec2 camera_unit_to_world(Vec2 p)
{
    if (!entity_eq(camera_entity, entity_nil))
        return transform_local_to_world(camera_entity, p);
    return p;
}

/* ------------------------------------------------------------------------- */

void camera_init()
{
    camera_entity = entity_nil;
    inverse_view_matrix = mat3_identity();
}

void camera_update_all()
{
    if (entity_eq(camera_entity, entity_nil))
    {
        inverse_view_matrix = mat3_identity();
    }
    else
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
    bool exists;

    /* check if we're actually saving something */
    exists = !entity_eq(camera_entity, entity_nil)
        && entity_get_save_filter(camera_entity);
    bool_save(&exists, s);

    if (exists)
    {
        entity_save(&camera_entity, s);
        mat3_save(&inverse_view_matrix, s);
    }
}
void camera_load_all(Deserializer *s)
{
    bool exists;

    /* check if we have to actually load anything */
    bool_load(&exists, s);
    if (exists)
    {
        entity_load(&camera_entity, s);
        mat3_load(&inverse_view_matrix, s);
    }
}

