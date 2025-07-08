# Summary: input.h

## Table of Contents

- [Input System Header - Keyboard and Mouse Input Handling](#input-system-header---keyboard-and-mouse-input-handling)

This header file defines a comprehensive input system for handling keyboard and mouse input, with both script-exportable functions and internal callback mechanisms.

## Key Enumerations

• **KeyCode enum**: Defines keyboard key codes with specific numeric values
  - `KC_UNKNOWN` (-1) and `KC_NONE` (0) for undefined/no key states
  - ASCII characters (32-162): space, punctuation, digits 0-9, letters A-Z, brackets, backslash, grave accent, and world keys
  - Function keys (256-348): escape, enter, tab, backspace, navigation keys, page up/down, home, end, lock keys, print screen, pause
  - Function keys F1-F25 (290-314)
  - Keypad keys KP_0 through KP_9 and keypad operators (320-336)
  - Modifier keys: left/right shift, control, alt, super (340-347)
  - Menu key (348)

• **MouseCode enum**: Defines mouse button codes
  - Primary buttons MC_1 through MC_8 (0-7)
  - `MC_NONE` (32) for no button
  - Convenient aliases: `MC_LEFT` (MC_1), `MC_RIGHT` (MC_2), `MC_MIDDLE` (MC_3)

## Script-Exported Functions

Functions marked with `EXPORT` and wrapped in `SCRIPT()` macro for external script access:

• **Key conversion utilities**:
  - `input_keycode_is_char()`: checks if KeyCode represents a character
  - `input_keycode_to_char()`: converts KeyCode to character
  - `input_char_to_keycode()`: converts character to KeyCode

• **Input state queries**:
  - `input_key_down()`: checks if specified key is currently pressed
  - `input_mouse_down()`: checks if specified mouse button is pressed

• **Mouse position functions**:
  - `input_get_mouse_pos_pixels()`: returns mouse position in pixel coordinates (Vec2)
  - `input_get_mouse_pos_unit()`: returns mouse position in unit coordinates (Vec2)

## Callback System

Internal callback mechanism for event-driven input handling:

• **Callback type definitions**:
  - `KeyCallback`: function pointer for key events, takes KeyCode parameter
  - `CharCallback`: function pointer for character input, takes unsigned int parameter
  - `MouseCallback`: function pointer for mouse button events, takes MouseCode parameter
  - `MouseMoveCallback`: function pointer for mouse movement, takes Vec2 position
  - `ScrollCallback`: function pointer for scroll events, takes Vec2 scroll data

• **Callback registration functions**:
  - `input_add_key_down_callback()` / `input_add_key_up_callback()`: register key press/release handlers
  - `input_add_char_down_callback()`: register character input handler
  - `input_add_mouse_down_callback()` / `input_add_mouse_up_callback()`: register mouse button handlers
  - `input_add_mouse_move_callback()`: register mouse movement handler
  - `input_add_scroll_callback()`: register scroll wheel handler

## System Management

• **Lifecycle functions**:
  - `input_init()`: initialize the input system
  - `input_deinit()`: cleanup and shutdown the input system

## Dependencies

• **External dependencies**:
  - `stdbool.h`: for boolean type support
  - `script_export.h`: provides `SCRIPT()` macro and `EXPORT` functionality
  - `vec2.h`: provides `Vec2` type for 2D coordinate handling

This input system provides a dual-interface approach: script-accessible functions for polling input state and a callback system for event-driven input handling within the C codebase.

## Sections

### Input System Header - Keyboard and Mouse Input Handling
Source: [input.h:1-189](../input.h#L1-L189)

This header file defines a comprehensive input system for handling keyboard and mouse input with scripting support. The file establishes key code mappings, mouse button definitions, and callback mechanisms for input events.

## Core Enumerations

### KeyCode Enumeration
- **KC_UNKNOWN** (-1) and **KC_NONE** (0): Special state values
- **ASCII Characters** (32-96, 161-162): Direct ASCII mappings including:
  - Basic punctuation: KC_SPACE, KC_APOSTROPHE, KC_COMMA, KC_MINUS, KC_PERIOD, KC_SLASH
  - Digits: KC_0 through KC_9 (48-57)
  - Letters: KC_A through KC_Z (65-90)
  - Symbols: KC_SEMICOLON, KC_EQUAL, KC_LEFT_BRACKET, KC_BACKSLASH, KC_RIGHT_BRACKET, KC_GRAVE_ACCENT
  - International: KC_WORLD_1, KC_WORLD_2
- **Function Keys** (256-348): Non-ASCII keys including:
  - Navigation: KC_ESCAPE, KC_ENTER, KC_TAB, KC_BACKSPACE, KC_INSERT, KC_DELETE
  - Arrow keys: KC_RIGHT, KC_LEFT, KC_DOWN, KC_UP
  - Page navigation: KC_PAGE_UP, KC_PAGE_DOWN, KC_HOME, KC_END
  - Lock keys: KC_CAPS_LOCK, KC_SCROLL_LOCK, KC_NUM_LOCK
  - System keys: KC_PRINT_SCREEN, KC_PAUSE
  - Function keys: KC_F1 through KC_F25 (290-314)
  - Keypad: KC_KP_0 through KC_KP_9, KC_KP_DECIMAL, KC_KP_DIVIDE, KC_KP_MULTIPLY, KC_KP_SUBTRACT, KC_KP_ADD, KC_KP_ENTER, KC_KP_EQUAL
  - Modifier keys: KC_LEFT_SHIFT, KC_LEFT_CONTROL, KC_LEFT_ALT, KC_LEFT_SUPER, KC_RIGHT_SHIFT, KC_RIGHT_CONTROL, KC_RIGHT_ALT, KC_RIGHT_SUPER
  - Menu key: KC_MENU

### MouseCode Enumeration
- **Numbered buttons**: MC_1 through MC_8 (0-7)
- **Special values**: MC_NONE (32)
- **Aliases**: MC_LEFT (MC_1), MC_RIGHT (MC_2), MC_MIDDLE (MC_3)

## Script-Exported Functions
The following functions are available to scripting systems via the SCRIPT macro:

### Keyboard Functions
- **input_keycode_is_char(KeyCode key)**: Returns bool indicating if key represents a character
- **input_keycode_to_char(KeyCode key)**: Converts KeyCode to char representation
- **input_char_to_keycode(char c)**: Converts char to corresponding KeyCode
- **input_key_down(KeyCode key)**: Returns bool indicating if key is currently pressed

### Mouse Functions
- **input_get_mouse_pos_pixels()**: Returns Vec2 with mouse position in pixel coordinates
- **input_get_mouse_pos_unit()**: Returns Vec2 with mouse position in unit coordinates
- **input_mouse_down(MouseCode mouse)**: Returns bool indicating if mouse button is pressed

## Callback System
The input system supports event-driven programming through callback registration:

### Callback Type Definitions
- **KeyCallback**: Function pointer for keyboard events `void (*KeyCallback)(KeyCode key)`
- **CharCallback**: Function pointer for character input `void (*CharCallback)(unsigned int c)`
- **MouseCallback**: Function pointer for mouse button events `void (*MouseCallback)(MouseCode mouse)`
- **MouseMoveCallback**: Function pointer for mouse movement `void (*MouseMoveCallback)(Vec2 pos)`
- **ScrollCallback**: Function pointer for scroll events `void (*ScrollCallback)(Vec2 scroll)`

### Callback Registration Functions
- **input_add_key_down_callback(KeyCallback f)**: Register key press callback
- **input_add_key_up_callback(KeyCallback f)**: Register key release callback
- **input_add_char_down_callback(CharCallback f)**: Register character input callback
- **input_add_mouse_down_callback(MouseCallback f)**: Register mouse press callback
- **input_add_mouse_up_callback(MouseCallback f)**: Register mouse release callback
- **input_add_mouse_move_callback(MouseMoveCallback f)**: Register mouse movement callback
- **input_add_scroll_callback(ScrollCallback f)**: Register scroll event callback

## System Management
- **input_init()**: Initialize the input system
- **input_deinit()**: Clean up and deinitialize the input system

## Dependencies
- **stdbool.h**: For boolean type support
- **script_export.h**: For SCRIPT and EXPORT macros enabling scripting integration
- **vec2.h**: For Vec2 type used in mouse position and scroll operations

The input system provides both immediate input polling and event-driven callback mechanisms, making it suitable for different programming paradigms and use cases.


### Related
- [Parent](../)

---
Hash: 95485e7c1f784c10d67f2297116b514a1f2ba58d475bdacd7f4e2f8df8825ded
Generated: 2025-07-07T20:21:19-04:00
