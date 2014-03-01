#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>

#include "script_export.h"
#include "vec2.h"

SCRIPT(input,

       typedef enum KeyCode KeyCode;
       enum KeyCode
       {
           KC_UNKNOWN,

           KC_SPACE,
           KC_0,
           KC_1,
           KC_2,
           KC_3,
           KC_4,
           KC_5,
           KC_6,
           KC_7,
           KC_8,
           KC_9,
           KC_A,
           KC_B,
           KC_C,
           KC_D,
           KC_E,
           KC_F,
           KC_G,
           KC_H,
           KC_I,
           KC_J,
           KC_K,
           KC_L,
           KC_M,
           KC_N,
           KC_O,
           KC_P,
           KC_Q,
           KC_R,
           KC_S,
           KC_T,
           KC_U,
           KC_V,
           KC_W,
           KC_X,
           KC_Y,
           KC_Z,

           KC_ESCAPE,
           KC_ENTER,
           KC_TAB,
           KC_BACKSPACE,
           KC_INSERT,
           KC_DELETE,
           KC_RIGHT,
           KC_LEFT,
           KC_DOWN,
           KC_UP,
           KC_PAGE_UP,
           KC_PAGE_DOWN,
           KC_HOME,
           KC_END,
           KC_CAPS_LOCK,
           KC_SCROLL_LOCK,
           KC_NUM_LOCK,
           KC_PRINT_SCREEN,
           KC_PAUSE,
           KC_F1,
           KC_F2,
           KC_F3,
           KC_F4,
           KC_F5,
           KC_F6,
           KC_F7,
           KC_F8,
           KC_F9,
           KC_F10,
           KC_F11,
           KC_F12,
           KC_F13,
           KC_F14,
           KC_F15,
           KC_F16,
           KC_F17,
           KC_F18,
           KC_F19,
           KC_F20,
           KC_F21,
           KC_F22,
           KC_F23,
           KC_F24,
           KC_F25,
           KC_KP_0,
           KC_KP_1,
           KC_KP_2,
           KC_KP_3,
           KC_KP_4,
           KC_KP_5,
           KC_KP_6,
           KC_KP_7,
           KC_KP_8,
           KC_KP_9,
           KC_KP_DECIMAL,
           KC_KP_DIVIDE,
           KC_KP_MULTIPLY,
           KC_KP_SUBTRACT,
           KC_KP_ADD,
           KC_KP_ENTER,
           KC_KP_EQUAL,
           KC_LEFT_SHIFT,
           KC_LEFT_CONTROL,
           KC_LEFT_ALT,
           KC_LEFT_SUPER,
           KC_RIGHT_SHIFT,
           KC_RIGHT_CONTROL,
           KC_RIGHT_ALT,
           KC_RIGHT_SUPER,
           KC_MENU,

           KC_NUM_KEYCODES
       };

       EXPORT bool input_key_down(KeyCode key);

       typedef enum MouseCode MouseCode;
       enum MouseCode
       {
           MC_1,
           MC_2,
           MC_3,
           MC_4,
           MC_5,
           MC_6,
           MC_7,
           MC_8,

           MC_NUM_MOUSECODES,

           /* aliases for common buttons */
           MC_LEFT = MC_1,
           MC_RIGHT = MC_2,
           MC_MIDDLE = MC_3,
       };

       EXPORT Vec2 input_get_mouse_pos_pixels(); /* pixels from top left */
       EXPORT Vec2 input_get_mouse_pos_unit();   /* center is (0, 0), left edge
                                                    has x = -1 */
       EXPORT bool input_mouse_down(MouseCode mouse);

    )

typedef void (*KeyCallback) (KeyCode key);
void input_add_key_down_callback(KeyCallback f);
void input_add_key_up_callback(KeyCallback f);

typedef void (*MouseCallback) (MouseCode mouse);
void input_add_mouse_down_callback(MouseCallback f);
void input_add_mouse_up_callback(MouseCallback f);


void input_init();
void input_deinit();

#endif

