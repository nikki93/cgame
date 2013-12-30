#ifndef GAME_H_W59MMXDF
#define GAME_H_W59MMXDF

#include "glew_glfw.h"
#include "script.h"

extern GLFWwindow *window;

void game_run();

SCRIPT(game,

        void game_quit();

      )

#endif

