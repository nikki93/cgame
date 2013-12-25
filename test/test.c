#include "test/test.h"

#include <stdlib.h>
#include <time.h>

#include "entity.h"
#include "transform.h"
#include "sprite.h"
#include "test/keyboard_controlled.h"

void test_c()
{
    Entity block, player;
    unsigned int i, n_blocks;

    /* add some blocks */

    srand(time(NULL));
    n_blocks = rand() % 50;
    for (i = 0; i < n_blocks; ++i)
    {
        block = entity_new();

        transform_add(block);
        transform_set_position(block,
                vec2((rand() % 25) - 12, (rand() % 9) - 4));

        sprite_add(block);
        sprite_set_cell(block, vec2(32.0f, 32.0f));
        sprite_set_size(block, vec2(32.0f, 32.0f));
    }

    /* add player */

    player = entity_new();

    transform_add(player);
    transform_set_position(player, vec2(0.0f, 0.0f));

    sprite_add(player);
    sprite_set_cell(player, vec2( 0.0f, 32.0f));
    sprite_set_size(player, vec2(32.0f, 32.0f));

    keyboard_controlled_add(player);

}

void test_init()
{
    /* everything happens through lua now! check main.lua */
}

