#ifndef GAME_H
#define GAME_H

#include "glew_glfw.h"
#include "script_export.h"
#include "scalar.h"
#include "vec2.h"

extern GLFWwindow *game_window;

void game_run(int argc, char **argv);

int game_get_argc();
char **game_get_argv();

SCRIPT(game,

       EXPORT void game_set_bg_color(Scalar a, Scalar g, Scalar b);
       EXPORT Vec2 game_get_window_size(); /* width, height in pixels */

       EXPORT void game_quit();

    )

#endif

