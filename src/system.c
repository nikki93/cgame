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
#include "physics.h"

#include "test/keyboard_controlled.h"

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

    saveload(entity);

    saveload(timing);
    saveload(transform);
    saveload(camera);
    saveload(sprite);
    saveload(physics);

    saveload(keyboard_controlled);

    saveload(script);
}

void system_save_all(Serializer *s)
{
    _saveload_all(s, true);
}

void system_load_all(Deserializer *s)
{
    _saveload_all(s, false);
}

