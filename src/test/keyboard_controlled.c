#include "keyboard_controlled.h"

#include <stdbool.h>

#include "timing.h"
#include "input.h"
#include "saveload.h"
#include "transform.h"

static bool kc_exists = false;
static Entity kc_entity;

void keyboard_controlled_add(Entity ent)
{
    transform_add(ent);

    kc_exists = true;
    kc_entity = ent;
}
void keyboard_controlled_remove(Entity ent)
{
    if (entity_eq(ent, kc_entity))
        kc_exists = false;
}

void keyboard_controlled_clear()
{
    kc_exists = false;
}

void keyboard_controlled_update_all()
{
    Vec2 dpos = vec2(0, 0), sca;
    Scalar rot, aspect;

    if (kc_exists)
    {
        if (entity_destroyed(kc_entity))
        {
            keyboard_controlled_remove(kc_entity);
            return;
        }

        rot = transform_get_rotation(kc_entity);
        sca = transform_get_scale(kc_entity);
        aspect = sca.y / sca.x;

        if (input_key_down(KC_LEFT))
            dpos = vec2_add(dpos, vec2(-5 * timing_dt, 0));
        if (input_key_down(KC_RIGHT))
            dpos = vec2_add(dpos, vec2( 5 * timing_dt, 0));
        if (input_key_down(KC_UP))
            dpos = vec2_add(dpos, vec2(0,  5 * timing_dt));
        if (input_key_down(KC_DOWN))
            dpos = vec2_add(dpos, vec2(0, -5 * timing_dt));

        if (input_key_down(KC_N))
            rot += 0.35 * SCALAR_PI * timing_dt;
        if (input_key_down(KC_M))
            rot -= 0.35 * SCALAR_PI * timing_dt;

        if (input_key_down(KC_K))
        {
            sca.x += 12 * timing_dt;
            sca.y = aspect * sca.x;
        }
        if (sca.x > 12 * timing_dt && input_key_down(KC_I))
        {
            sca.x -= 12 * timing_dt;
            sca.y = aspect * sca.x;
        }

        dpos = vec2_rot(dpos, rot);
        transform_translate(kc_entity, dpos);
        transform_set_rotation(kc_entity, rot);
        transform_set_scale(kc_entity, sca);
    }
}

void keyboard_controlled_save_all(Serializer *s)
{
    bool_save(&kc_exists, s);
    entity_save(&kc_entity, s);
}
void keyboard_controlled_load_all(Deserializer *s)
{
    bool_load(&kc_exists, s);
    entity_load(&kc_entity, s);
}

