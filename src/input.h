#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>

#include "glew_glfw.h"
#include "script_export.h"
#include "vec2.h"

SCRIPT(input,

       typedef enum KeyCode KeyCode;
       enum KeyCode
       {
           KC_UNKNOWN =             -1,
           KC_NONE =                0,

           /* ascii */
           KC_SPACE =               32,
           KC_APOSTROPHE =          39,
           KC_COMMA =               44,
           KC_MINUS =               45,
           KC_PERIOD =              46,
           KC_SLASH =               47,
           KC_0 =                   48,
           KC_1 =                   49,
           KC_2 =                   50,
           KC_3 =                   51,
           KC_4 =                   52,
           KC_5 =                   53,
           KC_6 =                   54,
           KC_7 =                   55,
           KC_8 =                   56,
           KC_9 =                   57,
           KC_SEMICOLON =           59,
           KC_EQUAL =               61,
           KC_A =                   65,
           KC_B =                   66,
           KC_C =                   67,
           KC_D =                   68,
           KC_E =                   69,
           KC_F =                   70,
           KC_G =                   71,
           KC_H =                   72,
           KC_I =                   73,
           KC_J =                   74,
           KC_K =                   75,
           KC_L =                   76,
           KC_M =                   77,
           KC_N =                   78,
           KC_O =                   79,
           KC_P =                   80,
           KC_Q =                   81,
           KC_R =                   82,
           KC_S =                   83,
           KC_T =                   84,
           KC_U =                   85,
           KC_V =                   86,
           KC_W =                   87,
           KC_X =                   88,
           KC_Y =                   89,
           KC_Z =                   90,
           KC_LEFT_BRACKET =        91,
           KC_BACKSLASH =           92,
           KC_RIGHT_BRACKET =       93,
           KC_GRAVE_ACCENT =        96,
           KC_WORLD_1 =             161,
           KC_WORLD_2 =             162,

           /* function keys */
           KC_ESCAPE =              256,
           KC_ENTER =               257,
           KC_TAB =                 258,
           KC_BACKSPACE =           259,
           KC_INSERT =              260,
           KC_DELETE =              261,
           KC_RIGHT =               262,
           KC_LEFT =                263,
           KC_DOWN =                264,
           KC_UP =                  265,
           KC_PAGE_UP =             266,
           KC_PAGE_DOWN =           267,
           KC_HOME =                268,
           KC_END =                 269,
           KC_CAPS_LOCK =           280,
           KC_SCROLL_LOCK =         281,
           KC_NUM_LOCK =            282,
           KC_PRINT_SCREEN =        283,
           KC_PAUSE =               284,
           KC_F1 =                  290,
           KC_F2 =                  291,
           KC_F3 =                  292,
           KC_F4 =                  293,
           KC_F5 =                  294,
           KC_F6 =                  295,
           KC_F7 =                  296,
           KC_F8 =                  297,
           KC_F9 =                  298,
           KC_F10 =                 299,
           KC_F11 =                 300,
           KC_F12 =                 301,
           KC_F13 =                 302,
           KC_F14 =                 303,
           KC_F15 =                 304,
           KC_F16 =                 305,
           KC_F17 =                 306,
           KC_F18 =                 307,
           KC_F19 =                 308,
           KC_F20 =                 309,
           KC_F21 =                 310,
           KC_F22 =                 311,
           KC_F23 =                 312,
           KC_F24 =                 313,
           KC_F25 =                 314,
           KC_KP_0 =                320,
           KC_KP_1 =                321,
           KC_KP_2 =                322,
           KC_KP_3 =                323,
           KC_KP_4 =                324,
           KC_KP_5 =                325,
           KC_KP_6 =                326,
           KC_KP_7 =                327,
           KC_KP_8 =                328,
           KC_KP_9 =                329,
           KC_KP_DECIMAL =          330,
           KC_KP_DIVIDE =           331,
           KC_KP_MULTIPLY =         332,
           KC_KP_SUBTRACT =         333,
           KC_KP_ADD =              334,
           KC_KP_ENTER =            335,
           KC_KP_EQUAL =            336,
           KC_LEFT_SHIFT =          340,
           KC_LEFT_CONTROL =        341,
           KC_LEFT_ALT =            342,
           KC_LEFT_SUPER =          343,
           KC_RIGHT_SHIFT =         344,
           KC_RIGHT_CONTROL =       345,
           KC_RIGHT_ALT =           346,
           KC_RIGHT_SUPER =         347,
           KC_MENU =                348,
       };

       EXPORT bool input_keycode_is_char(KeyCode key);
       EXPORT char input_keycode_to_char(KeyCode key);
       EXPORT KeyCode input_char_to_keycode(char c);

       EXPORT bool input_key_down(KeyCode key);

       typedef enum MouseCode MouseCode;
       enum MouseCode
       {
           MC_1 =          0,
           MC_2 =          1,
           MC_3 =          2,
           MC_4 =          3,
           MC_5 =          4,
           MC_6 =          5,
           MC_7 =          6,
           MC_8 =          7,

           MC_NONE =       32,

           MC_LEFT =       MC_1,
           MC_RIGHT =      MC_2,
           MC_MIDDLE =     MC_3,
       };

       EXPORT Vec2 input_get_mouse_pos_pixels();
       EXPORT Vec2 input_get_mouse_pos_unit();

       EXPORT bool input_mouse_down(MouseCode mouse);

    )

typedef void (*KeyCallback) (KeyCode key);
void input_add_key_down_callback(KeyCallback f);
void input_add_key_up_callback(KeyCallback f);

typedef void (*CharCallback) (unsigned int c);
void input_add_char_down_callback(CharCallback f);

typedef void (*MouseCallback) (MouseCode mouse);
void input_add_mouse_down_callback(MouseCallback f);
void input_add_mouse_up_callback(MouseCallback f);

typedef void (*MouseMoveCallback) (Vec2 pos);
void input_add_mouse_move_callback(MouseMoveCallback f);

void input_init();
void input_deinit();

#endif

