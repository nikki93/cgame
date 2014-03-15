#include "input.h"

#include "array.h"
#include "glew_glfw.h"
#include "game.h"

/* callback lists */
static Array *key_down_cbs;
static Array *key_up_cbs;
static Array *mouse_down_cbs;
static Array *mouse_up_cbs;

/* glfw <-> input conversions */
static int _keycode_to_glfw(KeyCode key);
static KeyCode _glfw_to_keycode(int key);
static int _mousecode_to_glfw(MouseCode mouse);
static MouseCode _glfw_to_mousecode(int mouse);

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

/* GLFW-KeyCode associations */

#define assoc_key_all()                                     \
    assoc_key(KC_UNKNOWN, GLFW_KEY_UNKNOWN),                \
                                                            \
    assoc_key(KC_SPACE, GLFW_KEY_SPACE),                    \
    assoc_key(KC_0, GLFW_KEY_0),                            \
    assoc_key(KC_1, GLFW_KEY_1),                            \
    assoc_key(KC_2, GLFW_KEY_2),                            \
    assoc_key(KC_3, GLFW_KEY_3),                            \
    assoc_key(KC_4, GLFW_KEY_4),                            \
    assoc_key(KC_5, GLFW_KEY_5),                            \
    assoc_key(KC_6, GLFW_KEY_6),                            \
    assoc_key(KC_7, GLFW_KEY_7),                            \
    assoc_key(KC_8, GLFW_KEY_8),                            \
    assoc_key(KC_9, GLFW_KEY_9),                            \
    assoc_key(KC_A, GLFW_KEY_A),                            \
    assoc_key(KC_B, GLFW_KEY_B),                            \
    assoc_key(KC_C, GLFW_KEY_C),                            \
    assoc_key(KC_D, GLFW_KEY_D),                            \
    assoc_key(KC_E, GLFW_KEY_E),                            \
    assoc_key(KC_F, GLFW_KEY_F),                            \
    assoc_key(KC_G, GLFW_KEY_G),                            \
    assoc_key(KC_H, GLFW_KEY_H),                            \
    assoc_key(KC_I, GLFW_KEY_I),                            \
    assoc_key(KC_J, GLFW_KEY_J),                            \
    assoc_key(KC_K, GLFW_KEY_K),                            \
    assoc_key(KC_L, GLFW_KEY_L),                            \
    assoc_key(KC_M, GLFW_KEY_M),                            \
    assoc_key(KC_N, GLFW_KEY_N),                            \
    assoc_key(KC_O, GLFW_KEY_O),                            \
    assoc_key(KC_P, GLFW_KEY_P),                            \
    assoc_key(KC_Q, GLFW_KEY_Q),                            \
    assoc_key(KC_R, GLFW_KEY_R),                            \
    assoc_key(KC_S, GLFW_KEY_S),                            \
    assoc_key(KC_T, GLFW_KEY_T),                            \
    assoc_key(KC_U, GLFW_KEY_U),                            \
    assoc_key(KC_V, GLFW_KEY_V),                            \
    assoc_key(KC_W, GLFW_KEY_W),                            \
    assoc_key(KC_X, GLFW_KEY_X),                            \
    assoc_key(KC_Y, GLFW_KEY_Y),                            \
    assoc_key(KC_Z, GLFW_KEY_Z),                            \
                                                            \
    assoc_key(KC_ESCAPE, GLFW_KEY_ESCAPE),                  \
    assoc_key(KC_ENTER, GLFW_KEY_ENTER),                    \
    assoc_key(KC_TAB, GLFW_KEY_TAB),                        \
    assoc_key(KC_BACKSPACE, GLFW_KEY_BACKSPACE),            \
    assoc_key(KC_INSERT, GLFW_KEY_INSERT),                  \
    assoc_key(KC_DELETE, GLFW_KEY_DELETE),                  \
    assoc_key(KC_RIGHT, GLFW_KEY_RIGHT),                    \
    assoc_key(KC_LEFT, GLFW_KEY_LEFT),                      \
    assoc_key(KC_DOWN, GLFW_KEY_DOWN),                      \
    assoc_key(KC_UP, GLFW_KEY_UP),                          \
    assoc_key(KC_PAGE_UP, GLFW_KEY_PAGE_UP),                \
    assoc_key(KC_PAGE_DOWN, GLFW_KEY_PAGE_DOWN),            \
    assoc_key(KC_HOME, GLFW_KEY_HOME),                      \
    assoc_key(KC_END, GLFW_KEY_END),                        \
    assoc_key(KC_CAPS_LOCK, GLFW_KEY_CAPS_LOCK),            \
    assoc_key(KC_SCROLL_LOCK, GLFW_KEY_SCROLL_LOCK),        \
    assoc_key(KC_NUM_LOCK, GLFW_KEY_NUM_LOCK),              \
    assoc_key(KC_PRINT_SCREEN, GLFW_KEY_PRINT_SCREEN),      \
    assoc_key(KC_PAUSE, GLFW_KEY_PAUSE),                    \
    assoc_key(KC_F1, GLFW_KEY_F1),                          \
    assoc_key(KC_F2, GLFW_KEY_F2),                          \
    assoc_key(KC_F3, GLFW_KEY_F3),                          \
    assoc_key(KC_F4, GLFW_KEY_F4),                          \
    assoc_key(KC_F5, GLFW_KEY_F5),                          \
    assoc_key(KC_F6, GLFW_KEY_F6),                          \
    assoc_key(KC_F7, GLFW_KEY_F7),                          \
    assoc_key(KC_F8, GLFW_KEY_F8),                          \
    assoc_key(KC_F9, GLFW_KEY_F9),                          \
    assoc_key(KC_F10, GLFW_KEY_F10),                        \
    assoc_key(KC_F11, GLFW_KEY_F11),                        \
    assoc_key(KC_F12, GLFW_KEY_F12),                        \
    assoc_key(KC_F13, GLFW_KEY_F13),                        \
    assoc_key(KC_F14, GLFW_KEY_F14),                        \
    assoc_key(KC_F15, GLFW_KEY_F15),                        \
    assoc_key(KC_F16, GLFW_KEY_F16),                        \
    assoc_key(KC_F17, GLFW_KEY_F17),                        \
    assoc_key(KC_F18, GLFW_KEY_F18),                        \
    assoc_key(KC_F19, GLFW_KEY_F19),                        \
    assoc_key(KC_F20, GLFW_KEY_F20),                        \
    assoc_key(KC_F21, GLFW_KEY_F21),                        \
    assoc_key(KC_F22, GLFW_KEY_F22),                        \
    assoc_key(KC_F23, GLFW_KEY_F23),                        \
    assoc_key(KC_F24, GLFW_KEY_F24),                        \
    assoc_key(KC_F25, GLFW_KEY_F25),                        \
    assoc_key(KC_KP_0, GLFW_KEY_KP_0),                      \
    assoc_key(KC_KP_1, GLFW_KEY_KP_1),                      \
    assoc_key(KC_KP_2, GLFW_KEY_KP_2),                      \
    assoc_key(KC_KP_3, GLFW_KEY_KP_3),                      \
    assoc_key(KC_KP_4, GLFW_KEY_KP_4),                      \
    assoc_key(KC_KP_5, GLFW_KEY_KP_5),                      \
    assoc_key(KC_KP_6, GLFW_KEY_KP_6),                      \
    assoc_key(KC_KP_7, GLFW_KEY_KP_7),                      \
    assoc_key(KC_KP_8, GLFW_KEY_KP_8),                      \
    assoc_key(KC_KP_9, GLFW_KEY_KP_9),                      \
    assoc_key(KC_KP_DECIMAL, GLFW_KEY_KP_DECIMAL),          \
    assoc_key(KC_KP_DIVIDE, GLFW_KEY_KP_DIVIDE),            \
    assoc_key(KC_KP_MULTIPLY, GLFW_KEY_KP_MULTIPLY),        \
    assoc_key(KC_KP_SUBTRACT, GLFW_KEY_KP_SUBTRACT),        \
    assoc_key(KC_KP_ADD, GLFW_KEY_KP_ADD),                  \
    assoc_key(KC_KP_ENTER, GLFW_KEY_KP_ENTER),              \
    assoc_key(KC_KP_EQUAL, GLFW_KEY_KP_EQUAL),              \
    assoc_key(KC_LEFT_SHIFT, GLFW_KEY_LEFT_SHIFT),          \
    assoc_key(KC_LEFT_CONTROL, GLFW_KEY_LEFT_CONTROL),      \
    assoc_key(KC_LEFT_ALT, GLFW_KEY_LEFT_ALT),              \
    assoc_key(KC_LEFT_SUPER, GLFW_KEY_LEFT_SUPER),          \
    assoc_key(KC_RIGHT_SHIFT, GLFW_KEY_RIGHT_SHIFT),        \
    assoc_key(KC_RIGHT_CONTROL, GLFW_KEY_RIGHT_CONTROL),    \
    assoc_key(KC_RIGHT_ALT, GLFW_KEY_RIGHT_ALT),            \
    assoc_key(KC_RIGHT_SUPER, GLFW_KEY_RIGHT_SUPER),        \
    assoc_key(KC_MENU, GLFW_KEY_MENU),                      \

static int _keycode_to_glfw(KeyCode key)
{
    static const int arr[KC_NUM_KEYCODES] =
        {
#define assoc_key(keycode, glfwkey) [keycode] = glfwkey
            assoc_key_all()
#undef assoc_key
        };

    return arr[key];
}

static KeyCode _glfw_to_keycode(int key)
{
    /* + 1 so we have non-negative array indices */
    static const KeyCode arr[GLFW_KEY_LAST + 2] =
        {
#define assoc_key(keycode, glfwkey) [glfwkey + 1] = keycode
            assoc_key_all()
#undef assoc_key
        };

    return arr[key + 1];
}

/* GLFW-MouseCode associations */

#define assoc_mouse_all()                       \
    assoc_mouse(MC_1, GLFW_MOUSE_BUTTON_1),     \
    assoc_mouse(MC_2, GLFW_MOUSE_BUTTON_2),     \
    assoc_mouse(MC_3, GLFW_MOUSE_BUTTON_3),     \
    assoc_mouse(MC_4, GLFW_MOUSE_BUTTON_4),     \
    assoc_mouse(MC_5, GLFW_MOUSE_BUTTON_5),     \
    assoc_mouse(MC_6, GLFW_MOUSE_BUTTON_6),     \
    assoc_mouse(MC_7, GLFW_MOUSE_BUTTON_7),     \
    assoc_mouse(MC_8, GLFW_MOUSE_BUTTON_8),     \

static int _mousecode_to_glfw(MouseCode mouse)
{
    static const int arr[MC_NUM_MOUSECODES] =
        {
#define assoc_mouse(mousecode, glfwmouse) [mousecode] = glfwmouse
            assoc_mouse_all()
#undef assoc_mouse
        };

    return arr[mouse];
}

static MouseCode _glfw_to_mousecode(int mouse)
{
    /* + 1 so we have non-negative array indices */
    static const MouseCode arr[GLFW_MOUSE_BUTTON_LAST + 1] =
        {
#define assoc_mouse(mousecode, glfwmouse) [glfwmouse] = mousecode
            assoc_mouse_all()
#undef assoc_key
        };

    return arr[mouse];
}

