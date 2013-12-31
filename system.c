#include "system.h"

#include <stdbool.h>
#include <stdio.h>

#include "entity.h"
#include "script.h"
#include "transform.h"
#include "camera.h"
#include "sprite.h"
#include "test/keyboard_controlled.h"

void system_init_all()
{
    transform_init();
    camera_init();
    sprite_init();
    script_init_all();
}

void system_deinit_all()
{
    script_deinit_all();
    sprite_deinit();
    transform_deinit();
}

void system_update_all(float dt)
{
    script_update_all(dt);

    keyboard_controlled_update_all(dt);

    camera_update_all();
    sprite_update_all();
}

void system_draw_all()
{
    script_draw_all();
    sprite_draw_all();
}

/* do it this way so we save/load in the same order */
static void _saveload_all(FILE *file, bool save)
{
#define saveload(sys) \
    if (save) sys##_save_all(file); else sys##_load_all(file)

    saveload(transform);
    saveload(camera);
    saveload(sprite);

    saveload(keyboard_controlled);

    saveload(script);
}

void system_save_all(FILE *file)
{
    _saveload_all(file, true);
}

void system_load_all(FILE *file)
{
    _saveload_all(file, false);
}

