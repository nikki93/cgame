#include "system.h"

#include <stdbool.h>
#include <stdio.h>

#include "entity.h"
#include "script.h"
#include "sprite.h"
#include "transform.h"
#include "test/keyboard_controlled.h"

void system_init_all()
{
    sprite_init();
    script_init_all();
}

void system_deinit_all()
{
    script_deinit_all();
    sprite_deinit();
    entity_deinit();
}

void system_update_all(float dt)
{
    /* check messages */

    sprite_check_messages_all();

    /* update */

    script_update_all(dt);

    keyboard_controlled_update_all(dt);

    sprite_update_all();
    entity_update_all();
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

