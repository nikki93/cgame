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

static Entity fps_text;

static void _fps_init()
{
    fps_text = entity_create();
    transform_add(fps_text);
    gui_text_add(fps_text);
    gui_text_set_str(fps_text, "fps: ...");
    gui_text_set_color(fps_text, color(0.0f, 0.4f, 0.1f, 1.0f));
}

static void _fps_update()
{
    static char buf[32];
    static const double display_period = 2.5f; /* fps display update period */
    static unsigned int nframes = 0;
    static double last_time = 0.0, curr_time;
    static bool first = true;
    int nchars;
    double interval;
    Vec2 win_size;

    ++nframes;

    curr_time = glfwGetTime();
    interval = curr_time - last_time;
    if (first || interval > display_period)
    {
        nchars = !first ? sprintf(buf, "fps: %.2f", nframes / interval)
            : sprintf(buf, "fps: ...");
        gui_text_set_str(fps_text, buf);

        win_size = game_get_window_size();
        transform_set_position(fps_text, vec2(win_size.x - 10 * nchars,
                                              -win_size.y + 12));

        nframes = 0;
        last_time = curr_time;
    }

    first = false;
}

/* ------------------------------------------------------------------------- */

static void _glfw_error_callback(int error, const char *desc)
{
    fprintf(stderr, "glfw: %s\n", desc);
}

static void _game_key_down(KeyCode key)
{
    switch (key)
    {
        case KC_ESCAPE: /* escape: quit */
            game_quit();
            break;

        case KC_E: /* e: toggle edit */
            timing_set_paused(!edit_get_enabled());
            edit_set_enabled(!edit_get_enabled());
            break;

        case KC_P: /* p: toggle pause */
            timing_set_paused(!timing_get_paused());
            break;

        case KC_C: /* c: clear */
            system_clear();
            console_puts("cleared");
            break;

        case KC_S: /* s: save */
            console_printf("saving to '%s'\n", usr_path("test.sav"));
            Serializer *s = serializer_open_file(usr_path("test.sav"));
            system_save_all(s);
            serializer_close(s);
            break;

        case KC_L: /* l: load */
            console_printf("loading from '%s'\n", usr_path("test.sav"));
            Deserializer *d = deserializer_open_file(usr_path("test.sav"));
            system_load_all(d);
            deserializer_close(d);
            break;

        case KC_R: /* shift+r: run scratch buffer */
            if (input_key_down(KC_LEFT_SHIFT))
                scratch_run();
            break;

        default:
            break;
    }
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
    _fps_init();

    /* bind event callbacks */
    input_add_key_down_callback(_game_key_down);

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
    glClear(GL_COLOR_BUFFER_BIT);

    system_draw_all();

    glfwSwapBuffers(game_window);
}

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

        _fps_update();
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

