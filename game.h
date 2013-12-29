#ifndef GAME_H_W59MMXDF
#define GAME_H_W59MMXDF

#include "script.h"

typedef struct GLFWwindow GLFWwindow;
GLFWwindow *window;

void game_run();

SCRIPT(game,

        void game_quit();

      )

#endif

