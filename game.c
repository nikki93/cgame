#include "game.h"

#include <stdlib.h>
#include <stdbool.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Freeimage.h>

#include "system.h"

#include "sprite.h"
#include "transform.h"
#include "test/keyboard_controlled.h"

static bool quit = false; /* exit main loop if true */

/* ------------------------------------------------------------------------- */

static void _test_init()
{
    Entity block, player;
    unsigned int i, n_blocks;

    srand(time(NULL));

    /* add some blocks */

    n_blocks = rand() % 50;
    for (i = 0; i < n_blocks; ++i)
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

    keyboard_controlled_add(player);
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
    system_init_all();

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
    FILE *file;

    glfwPollEvents();

    /* quit on escape key */
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        game_quit();

    /* save/load */
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
    {
        file = fopen("test.sav", "w");
        system_save_all(file);
        fclose(file);
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        file = fopen("test.sav", "r");
        system_load_all(file);
        fclose(file);
    }
}

static void _game_update(float dt)
{
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
    double last_time, curr_time, dt;

    _game_init();

    last_time = glfwGetTime();
    while (!quit && !glfwWindowShouldClose(window))
    {
        _game_events();

        curr_time = glfwGetTime();
        dt = curr_time - last_time;
        last_time = curr_time;
        /* printf("fps: %f\n", 1.0f / dt); */
        _game_update(dt);

        _game_draw();
    }

    _game_deinit();
}

void game_quit()
{
    quit = true;
}

