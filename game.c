#include "game.h"

#include <stdlib.h>
#include <stdbool.h>

#include "glew_glfw.h"
#include "dirs.h"
#include "system.h"

#include "test/test.h"

GLFWwindow *window;

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
    glClearColor(1.f, 1.f, 1.f, 1.f);

    /* init systems */
    system_init_all();

    /* init test */
    test_init();
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
        file = fopen(usr_path("test.sav"), "w");
        system_save_all(file);
        fclose(file);
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        file = fopen(usr_path("test.sav"), "r");
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
    glClear(GL_COLOR_BUFFER_BIT);

    system_draw_all();

    glfwSwapBuffers(window);
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
    while (!quit && !glfwWindowShouldClose(window))
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

