#ifndef GAME_H_W59MMXDF
#define GAME_H_W59MMXDF

#include "glew_glfw.h"
#include "script.h"

extern GLFWwindow *game_window;

void game_run(int argc, char **argv);

int game_get_argc();
char **game_get_argv();

SCRIPT(game,

        EXPORT void game_quit();

      )

#endif

