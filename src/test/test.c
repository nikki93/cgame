#include "test/test.h"

#include <stdlib.h>
#include <time.h>

#include "entity.h"
#include "transform.h"
#include "sprite.h"
#include "camera.h"
#include "test/keyboard_controlled.h"

void test_c()
{
    Entity camera, block, player;
    unsigned int i, n_blocks;

    /* add camera */

    camera = entity_create();

    transform_add(camera);

    camera_add(camera);
    camera_set_viewport_size(vec2(800 / 32.0, 600 / 32.0));

    /* add some blocks */

    srand(time(NULL));
    n_blocks = rand() % 50;
    for (i = 0; i < n_blocks; ++i)
    {
        block = entity_create();

        transform_add(block);
        transform_set_position(block,
                vec2((rand() % 25) - 12, (rand() % 9) - 4));

        sprite_add(block);
        sprite_set_cell(block, vec2(32.0f, 32.0f));
        sprite_set_size(block, vec2(32.0f, 32.0f));
    }

    /* add player */

    player = entity_create();

    transform_add(player);
    transform_set_position(player, vec2(0.0f, 0.0f));

    sprite_add(player);
    sprite_set_cell(player, vec2( 0.0f, 32.0f));
    sprite_set_size(player, vec2(32.0f, 32.0f));

    /* who gets keyboard control? */
    keyboard_controlled_add(camera);
}

void test_init()
{
    /* everything happens through lua now! check main.lua */
}

