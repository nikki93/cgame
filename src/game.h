#ifndef GAME_H_W59MMXDF
#define GAME_H_W59MMXDF

#include "glew_glfw.h"
#include "script.h"

extern GLFWwindow *game_window;

void game_run();

SCRIPT(game,

        EXPORT void game_quit();

      )

#endif

