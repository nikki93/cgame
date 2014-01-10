#ifndef KEYBOARD_CONTROLLED_C_4OQTGXF9
#define KEYBOARD_CONTROLLED_C_4OQTGXF9

#include "keyboard_controlled.h"

#include <stdbool.h>
#include <math.h>

#include "glew_glfw.h"
#include "saveload.h"
#include "transform.h"
#include "game.h"

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
    if (ent == kc_entity)
        kc_exists = false;
}

void keyboard_controlled_update_all(float dt)
{
    Vec2 dpos = vec2(0, 0), sca;
    float rot, aspect;

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

        if (glfwGetKey(game_window, GLFW_KEY_LEFT) == GLFW_PRESS)
            dpos = vec2_add(dpos, vec2(-5 * dt, 0));
        if (glfwGetKey(game_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            dpos = vec2_add(dpos, vec2( 5 * dt, 0));
        if (glfwGetKey(game_window, GLFW_KEY_UP) == GLFW_PRESS)
            dpos = vec2_add(dpos, vec2(0,  5 * dt));
        if (glfwGetKey(game_window, GLFW_KEY_DOWN) == GLFW_PRESS)
            dpos = vec2_add(dpos, vec2(0, -5 * dt));

        if (glfwGetKey(game_window, GLFW_KEY_N) == GLFW_PRESS)
            rot += 0.35 * M_PI * dt;
        if (glfwGetKey(game_window, GLFW_KEY_M) == GLFW_PRESS)
            rot -= 0.35 * M_PI * dt;

        if (glfwGetKey(game_window, GLFW_KEY_K) == GLFW_PRESS)
        {
            sca.x += 12 * dt;
            sca.y = aspect * sca.x;
        }
        if (sca.x > 12 * dt && glfwGetKey(game_window, GLFW_KEY_I) == GLFW_PRESS)
        {
            sca.x -= 12 * dt;
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

#endif

