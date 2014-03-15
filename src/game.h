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

       /*
        * screen-space coordinate systems:
        *
        *    unit: (0, 0) middle, (1, 1) top-right
        *    pixels: (0, 0) top-left, game_get_window_size() bottom right
        *
        */
       EXPORT Vec2 game_get_window_size(); /* width, height in pixels */
       EXPORT Vec2 game_unit_to_pixels(Vec2 p);
       EXPORT Vec2 game_pixels_to_unit(Vec2 p);

       EXPORT void game_quit();

    )

#endif

