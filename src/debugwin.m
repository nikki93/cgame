#include "glew_glfw.h"

#define GLFW_EXPOSE_NATIVE_COCOA
#define GLFW_EXPOSE_NATIVE_NSGL
#include <GLFW/glfw3native.h>

#include "game.h"

void debugwin_init()
{
    /* always on top */
    id ns = glfwGetCocoaWindow(game_window);
    [ns setLevel:NSFloatingWindowLevel];

    /* top-left positioning */
    glfwSetWindowPos(game_window, 9, 53);
}

