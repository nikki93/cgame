#include "game.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "glew_glfw.h"
#include "dirs.h"
#include "saveload.h"
#include "system.h"
#include "console.h"
#include "gui.h"
#include "transform.h"
#include "input.h"
#include "timing.h"
#include "scratch.h"
#include "edit.h"

#ifdef CGAME_DEBUG_WINDOW
#include "debugwin.h"
#endif

#include "test/test.h"

GLFWwindow *game_window;

static bool quit = false; /* exit main loop if true */
static int sargc = 0;
static char **sargv;

/* ------------------------------------------------------------------------- */

static void _glfw_error_callback(int error, const char *desc)
{
    fprintf(stderr, "glfw: %s\n", desc);
}

static void _game_init()
{
    /* initialize glfw */
    glfwSetErrorCallback(_glfw_error_callback);
    glfwInit();

    /* create glfw window */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    game_window = glfwCreateWindow(800, 600, "cgame", NULL, NULL);
#ifdef CGAME_DEBUG_WINDOW
    debugwin_init();
#endif

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

    /* random seed */
    srand(time(NULL));

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
    glfwPollEvents();

    if (glfwWindowShouldClose(game_window))
        game_quit();
}

static void _game_update()
{
    system_update_all();
}

static void _game_draw()
{
    static bool first = true;

    /* don't draw first frame -- allow a full update */
    if (first)
    {
        first = false;
        return;
    }

    glClear(GL_COLOR_BUFFER_BIT);
    system_draw_all();
    glfwSwapBuffers(game_window);
}

/* ------------------------------------------------------------------------- */

void game_run(int argc, char **argv)
{
    sargc = argc;
    sargv = argv;

    _game_init();

    while (!quit)
    {
        _game_events();
        _game_update();
        _game_draw();
    }

    _game_deinit();
}

void game_set_bg_color(Color c)
{
    glClearColor(c.r, c.g, c.b, 1.0);
}

Vec2 game_get_window_size()
{
    int w, h;
    glfwGetWindowSize(game_window, &w, &h);
    return vec2(w, h);
}
Vec2 game_unit_to_pixels(Vec2 p)
{
    Vec2 hw = vec2_scalar_mul(game_get_window_size(), 0.5f);
    p.y *= -1.0f;
    p = vec2_add(vec2_mul(p, hw), hw);
    return p;
}
Vec2 game_pixels_to_unit(Vec2 p)
{
    Vec2 hw = vec2_scalar_mul(game_get_window_size(), 0.5f);
    p = vec2_div(vec2_sub(p, hw), hw);
    p.y *= -1.0f;
    return p;
}

void game_quit()
{
    quit = true;
}

int game_get_argc()
{
    return sargc;
}
char **game_get_argv()
{
    return sargv;
}

