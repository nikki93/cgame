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

void system_check_messages_all()
{
    sprite_check_messages_all();
}

void system_update_all(float dt)
{
    sprite_update_all();
    entity_update_all();
}

void system_draw_all()
{
    sprite_draw_all();
}

