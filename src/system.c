#include "system.h"

#include <stdbool.h>
#include <stdio.h>

#include "entity.h"
#include "script.h"
#include "timing.h"
#include "input.h"
#include "transform.h"
#include "camera.h"
#include "texture.h"
#include "sprite.h"
#include "text.h"
#include "console.h"
#include "scratch.h"
#include "physics.h"

#include "test/keyboard_controlled.h"

static void _key_down(KeyCode key)
{
    script_key_down(key);
}
static void _key_up(KeyCode key)
{
    script_key_up(key);
}

static void _mouse_down(MouseCode mouse)
{
    script_mouse_down(mouse);
}
static void _mouse_up(MouseCode mouse)
{
    script_mouse_up(mouse);
}

void system_init()
{
    input_init();
    entity_init();
    transform_init();
    camera_init();
    texture_init();
    text_init();
    sprite_init();
    console_init();
    physics_init();
    script_init();

    input_add_key_down_callback(_key_down);
    input_add_key_up_callback(_key_up);

    input_add_mouse_down_callback(_mouse_down);
    input_add_mouse_up_callback(_mouse_up);
}

void system_deinit()
{
    script_deinit();
    physics_deinit();
    console_deinit();
    sprite_deinit();
    text_deinit();
    texture_deinit();
    transform_deinit();
    entity_deinit();
    input_deinit();
}

void system_clear()
{
    keyboard_controlled_clear();

    script_clear();

    physics_clear();
    sprite_clear();
    camera_clear();
    transform_clear();
    entity_clear();
}

void system_update_all()
{
    timing_update();

    scratch_update();
    script_update_all();

    keyboard_controlled_update_all();

    physics_update_all();
    transform_update_all();
    camera_update_all();
    sprite_update_all();

    entity_update_all();
}

void system_draw_all()
{
    script_draw_all();
    sprite_draw_all();
    text_draw_all();
}

/* do it this way so we save/load in the same order */
static void _saveload_all(void *s, bool save)
{
#define saveload(sys)                                   \
    if (save) sys##_save_all(s); else sys##_load_all(s)

    entity_load_all_begin();

    saveload(timing);
    saveload(transform);
    saveload(camera);
    saveload(sprite);
    saveload(physics);

    saveload(keyboard_controlled);

    saveload(script);

    entity_load_all_end();
}

void system_save_all(Serializer *s)
{
    _saveload_all(s, true);
}

void system_load_all(Deserializer *s)
{
    _saveload_all(s, false);
}

