#ifndef GAME_H
#define GAME_H

#include "glew_glfw.h"
#include "script_export.h"
#include "vec2.h"
#include "color.h"

extern GLFWwindow *game_window;

/* top-level cgame entry point */
void game_run(int argc, char **argv);

/* get argc, argv as passed to game_run(...) */
int game_get_argc();
char **game_get_argv();

SCRIPT(game,

       EXPORT void game_set_bg_color(Color c);
       EXPORT Vec2 game_get_window_size(); /* width, height in pixels */

       EXPORT void game_quit();

    )

#endif

