#include "system.h"

#include "sprite.h"
#include "transform.h"

void system_init_all()
{
    sprite_init();
}

void system_deinit_all()
{
    sprite_deinit();
    entity_deinit();
}

void system_update_all(float dt)
{
    /* check messages */
    sprite_check_messages_all();

    /* update */
    sprite_update_all();
    entity_update_all();
}

void system_draw_all()
{
    sprite_draw_all();
}

