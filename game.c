#include "game.h"

#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Freeimage.h>

#include "system.h"

#include "sprite.h"
#include "transform.h"

static GLFWwindow *window;

static int quit = 0; /* exit main loop if 1 */

static Entity player;

static void _test_init()
{
    Entity block;
    unsigned int i;

    /* add 100 blocks */

    for (i = 0; i < 100; ++i)
    {
        block = entity_new();

        transform_add(block);
        transform_set_origin(block,
                vec2((rand() % 25) - 12, (rand() % 9) - 4));

        sprite_add(block);
        sprite_set_cell(block, vec2(32.0f, 32.0f));
        sprite_set_size(block, vec2(32.0f, 32.0f));
    }

    /* add player */

    player = entity_new();

    transform_add(player);
    transform_set_origin(player, vec2(0.0f, 0.0f));

    sprite_add(player);
    sprite_set_cell(player, vec2( 0.0f, 32.0f));
    sprite_set_size(player, vec2(32.0f, 32.0f));
}

static void _test_update(float dt)
{
    static int player_exists = 1;
    Vec2 player_pos;

    if (player_exists)
    {
        player_pos = transform_get_origin(player);

        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            player_pos.x -= 0.5 * dt;
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            player_pos.x += 0.5 * dt;
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            player_pos.y += 0.5 * dt;
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            player_pos.y -= 0.5 * dt;

        transform_set_origin(player, player_pos);

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            entity_new_message(player, MSG_DESTROY, 0);
            player_exists = 0;
        }
    }
}

static void _game_init()
{
    /* initialize glfw */
    glfwInit();

    /* create glfw window */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    window = glfwCreateWindow(800, 600, "cgame", NULL, NULL);

    /* activate OpenGL context */
    glfwMakeContextCurrent(window);

    /* initialize GLEW */
    glewExperimental = GL_TRUE;
    glewInit();
    glGetError(); /* see http://www.opengl.org/wiki/OpenGL_Loading_Library */

    /* some GL settings */
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

    /* init systems */
    sprite_init();

    /* test */
    _test_init();
}

static void _game_deinit()
{
    /* deinit systems */
    system_deinit_all();

    /* deinit glfw */
    glfwTerminate();
}

static void _game_events()
{
    glfwPollEvents();

    /* quit on escape key */
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        game_quit();
}

static void _game_update(float dt)
{
    /* test update */
    _test_update(dt);

    /* update systems */
    system_update_all(dt);
}

static void _game_draw()
{
    glClearColor(1.f, 1.f, 1.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    system_draw_all();

    glfwSwapBuffers(window);
}

void game_run()
{
    _game_init();

    while (!quit && !glfwWindowShouldClose(window))
    {
        _game_events();
        _game_update(0.1);
        _game_draw();
    }

    _game_deinit();
}

void game_quit()
{
    quit = 1;
}

