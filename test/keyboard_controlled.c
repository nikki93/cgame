#ifndef KEYBOARD_CONTROLLED_C_4OQTGXF9
#define KEYBOARD_CONTROLLED_C_4OQTGXF9

#include "keyboard_controlled.h"

#include <stdbool.h>
#include <GLFW/glfw3.h>

#include "transform.h"
#include "game.h"

static bool kc_exists = false;
static Entity kc_entity;

void keyboard_controlled_add(Entity ent)
{
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
    Vec2 pos;

    if (kc_exists)
    {
        pos = transform_get_origin(kc_entity);

        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            pos.x -= 5 * dt;
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            pos.x += 5 * dt;
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            pos.y += 5 * dt;
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            pos.y -= 5 * dt;

        transform_set_origin(kc_entity, pos);
    }
}

void keyboard_controlled_save_all(FILE *file)
{
    fwrite(&kc_exists, sizeof(kc_exists), 1, file);
    fwrite(&kc_entity, sizeof(kc_entity), 1, file);
}
void keyboard_controlled_load_all(FILE *file)
{
    fread(&kc_exists, sizeof(kc_exists), 1, file);
    fread(&kc_entity, sizeof(kc_entity), 1, file);
}

#endif

