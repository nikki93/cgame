#include "game.h"

#include <stdlib.h>
#include <stdbool.h>

#include "glew_glfw.h"
#include "dirs.h"
#include "system.h"

#include "test/test.h"

#include "entity.h"
#include "transform.h"
#include "sprite.h"

GLFWwindow *game_window;

static bool quit = false; /* exit main loop if true */

/* ------------------------------------------------------------------------- */

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
    game_window = glfwCreateWindow(800, 600, "cgame", NULL, NULL);

    /* activate OpenGL context */
    glfwMakeContextCurrent(game_window);

    /* initialize GLEW */
    glewExperimental = GL_TRUE;
    glewInit();
    glGetError(); /* see http://www.opengl.org/wiki/OpenGL_Loading_Library */

    /* some GL settings */
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
    glClearColor(1.f, 1.f, 1.f, 1.f);

    /* init systems */
    system_init();

    /* init test */
    test_init();
}

static void _game_deinit()
{
    /* deinit systems */
    system_deinit();

    /* deinit glfw */
    glfwTerminate();
}

static void _game_events()
{
    FILE *file;

    glfwPollEvents();

    /* quit on escape key */
    if (glfwGetKey(game_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        game_quit();

    /* save/load */
    if (glfwGetKey(game_window, GLFW_KEY_O) == GLFW_PRESS)
    {
        file = fopen(usr_path("test.sav"), "w");
        system_save_all(file);
        fclose(file);
    }
    if (glfwGetKey(game_window, GLFW_KEY_P) == GLFW_PRESS)
    {
        file = fopen(usr_path("test.sav"), "r");
        system_load_all(file);
        fclose(file);
    }

    if (glfwGetKey(game_window, GLFW_KEY_0) == GLFW_PRESS)
        entity_destroy(0);
    if (glfwGetKey(game_window, GLFW_KEY_1) == GLFW_PRESS)
        entity_destroy(1);
    if (glfwGetKey(game_window, GLFW_KEY_2) == GLFW_PRESS)
        entity_destroy(2);
    if (glfwGetKey(game_window, GLFW_KEY_3) == GLFW_PRESS)
        entity_destroy(3);
    if (glfwGetKey(game_window, GLFW_KEY_4) == GLFW_PRESS)
        entity_destroy(4);
    if (glfwGetKey(game_window, GLFW_KEY_5) == GLFW_PRESS)
        entity_destroy(5);
    if (glfwGetKey(game_window, GLFW_KEY_6) == GLFW_PRESS)
        entity_destroy(6);
    if (glfwGetKey(game_window, GLFW_KEY_7) == GLFW_PRESS)
        entity_destroy(7);
    if (glfwGetKey(game_window, GLFW_KEY_8) == GLFW_PRESS)
        entity_destroy(8);
    if (glfwGetKey(game_window, GLFW_KEY_9) == GLFW_PRESS)
        entity_destroy(9);

    if (glfwGetKey(game_window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        Entity block = entity_create();

        transform_add(block);
        transform_set_position(block,
                vec2((rand() % 25) - 12, (rand() % 9) - 4));

        sprite_add(block);
        sprite_set_cell(block, vec2(32.0f, 32.0f));
        sprite_set_size(block, vec2(32.0f, 32.0f));
    }
}

static void _game_update(float dt)
{
    system_update_all(dt);
}

static void _game_draw()
{
    glClear(GL_COLOR_BUFFER_BIT);

    system_draw_all();

    glfwSwapBuffers(game_window);
}

static void _fps_display()
{
    static const double display_period = 5.0f; /* fps display update period */
    static unsigned int nframes = 0;
    static double last_time = 0.0, curr_time;
    double interval;

    ++nframes;

    curr_time = glfwGetTime();
    interval = curr_time - last_time;
    if (interval > display_period)
    {
        printf("fps: %f\n", nframes / interval);

        nframes = 0;
        last_time = curr_time;
    }
}

void game_run()
{
    double last_time, curr_time, dt;

    _game_init();

    last_time = glfwGetTime();
    while (!quit && !glfwWindowShouldClose(game_window))
    {
        _game_events();

        curr_time = glfwGetTime();
        dt = curr_time - last_time;
        last_time = curr_time;
        _game_update(dt);

        _game_draw();

        _fps_display();
    }

    _game_deinit();
}

void game_quit()
{
    quit = true;
}

