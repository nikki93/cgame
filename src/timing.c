#include "timing.h"

#include <glew_glfw.h>

Scalar timing_dt;
static Scalar scale;

void timing_set_scale(Scalar s)
{
    scale = s;
}
Scalar timing_get_scale()
{
    return scale;
}

static void _dt_update()
{
    static double last_time = -1;
    double curr_time;

    /* first update? */
    if (last_time < 0)
        last_time = glfwGetTime();

    curr_time = glfwGetTime();
    timing_dt = scale * (curr_time - last_time);
    last_time = curr_time;
}

void timing_update()
{
    _dt_update();
}

void timing_save_all(Serializer *s)
{
    scalar_save(&scale, s);
}
void timing_load_all(Deserializer *s)
{
    scalar_load(&scale, s);
}

