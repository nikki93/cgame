#include "timing.h"

#include <glew_glfw.h>

Scalar timing_dt;

static void _dt_update()
{
    static double last_time = -1;
    double curr_time;

    /* first update? */
    if (last_time < 0)
        last_time = glfwGetTime();

    curr_time = glfwGetTime();
    timing_dt = curr_time - last_time;
    last_time = curr_time;
}

void timing_update()
{
    _dt_update();
}

