#include "input.h"

#include <assert.h>
#include <ctype.h>

#include "array.h"
#include "glew_glfw.h"
#include "game.h"

/* callback lists */
static Array *key_down_cbs;
static Array *key_up_cbs;
static Array *mouse_down_cbs;
static Array *mouse_up_cbs;

/* glfw <-> input conversions */
static int _keycode_to_glfw(KeyCode key) { return key; }
static KeyCode _glfw_to_keycode(int key) { return key; }
static int _mousecode_to_glfw(MouseCode mouse) { return mouse; }
static MouseCode _glfw_to_mousecode(int mouse) { return mouse; }

char input_keycode_to_char(KeyCode key)
{
    assert(KC_ASCII_BEGIN <= key <= KC_ASCII_END);
    return tolower(key);
}
KeyCode input_char_to_keycode(char c)
{
    c = toupper(c);
    assert(KC_ASCII_BEGIN <= c <= KC_ASCII_END);
    return c;
}

bool input_key_down(KeyCode key)
{
    int glfwkey = _keycode_to_glfw(key);
    return glfwGetKey(game_window, glfwkey) == GLFW_PRESS;
}

Vec2 input_get_mouse_pos_pixels()
{
    double x, y;
    glfwGetCursorPos(game_window, &x, &y);
    return vec2(x, y);
}
Vec2 input_get_mouse_pos_unit()
{
    return game_pixels_to_unit(input_get_mouse_pos_pixels());
}

bool input_mouse_down(MouseCode mouse)
{
    int glfwmouse = _mousecode_to_glfw(mouse);
    return glfwGetMouseButton(game_window, glfwmouse) == GLFW_PRESS;
}

void input_add_key_down_callback(KeyCallback f)
{
    array_add_val(KeyCallback, key_down_cbs) = f;
}
void input_add_key_up_callback(KeyCallback f)
{
    array_add_val(KeyCallback, key_up_cbs) = f;
}

void input_add_mouse_down_callback(MouseCallback f)
{
    array_add_val(MouseCallback, mouse_down_cbs) = f;
}
void input_add_mouse_up_callback(MouseCallback f)
{
    array_add_val(MouseCallback, mouse_up_cbs) = f;
}

static void _key_callback(GLFWwindow *window, int key, int scancode,
                          int action, int mods)
{
    KeyCallback *f;

    /* call all registered callbacks */
    switch (action)
    {
        case GLFW_PRESS:
            array_foreach(f, key_down_cbs)
                (*f)(_glfw_to_keycode(key));
            break;

        case GLFW_RELEASE:
            array_foreach(f, key_up_cbs)
                (*f)(_glfw_to_keycode(key));
            break;
    }
}

static void _mouse_callback(GLFWwindow *window, int mouse, int action,
                            int mods)
{
    MouseCallback *f;

    /* call all registered callbacks */
    switch (action)
    {
        case GLFW_PRESS:
            array_foreach(f, mouse_down_cbs)
                (*f)(_glfw_to_mousecode(mouse));
            break;

        case GLFW_RELEASE:
            array_foreach(f, mouse_up_cbs)
                (*f)(_glfw_to_mousecode(mouse));
            break;
    }
}

void input_init()
{
    key_down_cbs = array_new(KeyCallback);
    key_up_cbs = array_new(KeyCallback);
    glfwSetKeyCallback(game_window, _key_callback);

    mouse_down_cbs = array_new(MouseCallback);
    mouse_up_cbs = array_new(MouseCallback);
    glfwSetMouseButtonCallback(game_window, _mouse_callback);
}

void input_deinit()
{
    array_free(mouse_up_cbs);
    array_free(mouse_down_cbs);

    array_free(key_up_cbs);
    array_free(key_down_cbs);
}

