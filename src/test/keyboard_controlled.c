#include "keyboard_controlled.h"

#include <stdbool.h>

#include "timing.h"
#include "input.h"
#include "saveload.h"
#include "transform.h"
#include "gui.h"

/* 
 * this isn't the most ideal way -- should probably just use
 * entity_nil for non-existent...
 */

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

        if (timing_get_paused())
            return;
        if (gui_has_focus())
            return;

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

void keyboard_controlled_save_all(Store *s)
{
    Store *t;

    if (store_child_save(&t, "keyboard_controlled", s))
        if (kc_exists && entity_get_save_filter(kc_entity))
            entity_save(&kc_entity, "kc_entity", t);
}
void keyboard_controlled_load_all(Store *s)
{
    Store *t;

    if (store_child_load(&t, "keyboard_controlled", s))
        if (entity_load(&kc_entity, "kc_entity", kc_entity, t))
            kc_exists = true;
}

