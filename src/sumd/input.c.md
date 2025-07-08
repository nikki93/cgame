# Summary: input.c

## Table of Contents

- [Input System Implementation with GLFW Integration](#input-system-implementation-with-glfw-integration)

This file implements a comprehensive input handling system that acts as a wrapper around GLFW, providing callback-based event handling for keyboard, mouse, and scroll inputs.

## Core Architecture

The system uses **callback lists** to manage input event handlers:
- `key_down_cbs`, `key_up_cbs` - Arrays storing keyboard event callbacks
- `char_down_cbs` - Array for character input callbacks  
- `mouse_down_cbs`, `mouse_up_cbs` - Arrays for mouse button callbacks
- `mouse_move_cbs` - Array for mouse movement callbacks
- `scroll_cbs` - Array for scroll event callbacks

## Key Conversion Functions

**GLFW Integration Functions:**
- `_keycode_to_glfw()` - Converts KeyCode to GLFW key codes (direct passthrough)
- `_glfw_to_keycode()` - Converts GLFW key codes to KeyCode (direct passthrough)  
- `_mousecode_to_glfw()` - Converts MouseCode to GLFW mouse codes (direct passthrough)
- `_glfw_to_mousecode()` - Converts GLFW mouse codes to MouseCode (direct passthrough)

**Character Handling Functions:**
- `input_keycode_is_char()` - Determines if a KeyCode represents a printable character
  - Returns true for: space, punctuation, digits 0-9, letters A-Z, brackets, special characters
- `input_keycode_to_char()` - Converts KeyCode to lowercase character using `tolower()`
- `input_char_to_keycode()` - Converts character to KeyCode using `toupper()`

## Input State Query Functions

**Current State Polling:**
- `input_key_down()` - Checks if a key is currently pressed using `glfwGetKey()`
- `input_mouse_down()` - Checks if a mouse button is currently pressed using `glfwGetMouseButton()`
- `input_get_mouse_pos_pixels()` - Returns mouse position in pixels as Vec2 (note: y-coordinate is negated)
- `input_get_mouse_pos_unit()` - Returns mouse position in game units by calling `game_pixels_to_unit()`

## Callback Registration Functions

**Event Handler Registration:**
- `input_add_key_down_callback()` - Registers KeyCallback for key press events
- `input_add_key_up_callback()` - Registers KeyCallback for key release events
- `input_add_char_down_callback()` - Registers CharCallback for character input events
- `input_add_mouse_down_callback()` - Registers MouseCallback for mouse press events
- `input_add_mouse_up_callback()` - Registers MouseCallback for mouse release events
- `input_add_mouse_move_callback()` - Registers MouseMoveCallback for mouse movement
- `input_add_scroll_callback()` - Registers ScrollCallback for scroll events

## GLFW Callback Handlers

**Internal Event Processors:**
- `_key_callback()` - Handles GLFW key events, dispatches to registered KeyCallback functions based on GLFW_PRESS/GLFW_RELEASE
- `_char_callback()` - Handles GLFW character events, dispatches to all registered CharCallback functions
- `_mouse_callback()` - Handles GLFW mouse button events, dispatches based on GLFW_PRESS/GLFW_RELEASE
- `_cursor_pos_callback()` - Handles GLFW cursor position events, negates y-coordinate and dispatches to MouseMoveCallback functions
- `_scroll_callback()` - Handles GLFW scroll events, dispatches to ScrollCallback functions

## Lifecycle Management

**Initialization (`input_init()`):**
- Creates callback arrays using `array_new()` for each event type
- Registers GLFW callbacks using `glfwSetKeyCallback()`, `glfwSetCharCallback()`, etc.
- Links internal handlers to GLFW event system

**Cleanup (`input_deinit()`):**
- Frees all callback arrays using `array_free()` in reverse order of initialization

## Key Dependencies

- **External modules:** `game` (for `game_window` and `game_pixels_to_unit()`), `glew_glfw.h`, `array.h`, `error.h`
- **Standard libraries:** `ctype.h` for `tolower()` and `toupper()`
- **Data types:** KeyCode, MouseCode, Vec2, various callback function pointer types

The system provides a clean abstraction over GLFW's input handling while maintaining direct access to current input states and flexible event-driven input processing.

## Sections

### Input System Implementation with GLFW Integration
Source: [input.c:1-207](../input.c#L1-L207)

This section implements a comprehensive input handling system that provides a callback-based interface for keyboard, mouse, and scroll events using GLFW as the underlying windowing library.

## Core Data Structures

The system maintains seven static callback arrays to store registered event handlers:
- `key_down_cbs` - Array of `KeyCallback` functions for key press events
- `key_up_cbs` - Array of `KeyCallback` functions for key release events  
- `char_down_cbs` - Array of `CharCallback` functions for character input events
- `mouse_down_cbs` - Array of `MouseCallback` functions for mouse button press events
- `mouse_up_cbs` - Array of `MouseCallback` functions for mouse button release events
- `mouse_move_cbs` - Array of `MouseMoveCallback` functions for mouse movement events
- `scroll_cbs` - Array of `ScrollCallback` functions for scroll wheel events

## Key Code Conversion Functions

The system provides bidirectional conversion between internal key codes and GLFW key codes:
- `_keycode_to_glfw()` - Converts internal `KeyCode` to GLFW key code (currently a direct mapping)
- `_glfw_to_keycode()` - Converts GLFW key code to internal `KeyCode` (currently a direct mapping)
- `_mousecode_to_glfw()` - Converts internal `MouseCode` to GLFW mouse code (currently a direct mapping)
- `_glfw_to_mousecode()` - Converts GLFW mouse code to internal `MouseCode` (currently a direct mapping)

## Character and Key Code Utilities

- `input_keycode_is_char()` - Determines if a `KeyCode` represents a printable character
  - Returns `true` for alphanumeric keys, punctuation, and special characters
  - Includes keys: space, apostrophe, comma, minus, period, slash, digits 0-9, semicolon, equal, letters A-Z, brackets, backslash, grave accent, and world keys 1-2
- `input_keycode_to_char()` - Converts a `KeyCode` to its lowercase character representation using `tolower()`
- `input_char_to_keycode()` - Converts a character to its corresponding `KeyCode` using `toupper()`

## Input State Query Functions

- `input_key_down()` - Checks if a key is currently pressed using `glfwGetKey()` with the global `game_window`
- `input_get_mouse_pos_pixels()` - Returns mouse position in pixels as `Vec2`, with Y-coordinate inverted
- `input_get_mouse_pos_unit()` - Returns mouse position in game units using `game_pixels_to_unit()` conversion
- `input_mouse_down()` - Checks if a mouse button is currently pressed using `glfwGetMouseButton()`

## Callback Registration Functions

Seven functions allow registering callbacks for different input events:
- `input_add_key_down_callback()` - Registers `KeyCallback` for key press events
- `input_add_key_up_callback()` - Registers `KeyCallback` for key release events
- `input_add_char_down_callback()` - Registers `CharCallback` for character input events
- `input_add_mouse_down_callback()` - Registers `MouseCallback` for mouse button press events
- `input_add_mouse_up_callback()` - Registers `MouseCallback` for mouse button release events
- `input_add_mouse_move_callback()` - Registers `MouseMoveCallback` for mouse movement events
- `input_add_scroll_callback()` - Registers `ScrollCallback` for scroll wheel events

## GLFW Event Handlers

Five static callback functions handle GLFW events and dispatch them to registered callbacks:

- `_key_callback()` - Handles keyboard events
  - Dispatches `GLFW_PRESS` events to all registered key down callbacks
  - Dispatches `GLFW_RELEASE` events to all registered key up callbacks
- `_char_callback()` - Handles character input events and dispatches to all registered character callbacks
- `_mouse_callback()` - Handles mouse button events
  - Dispatches `GLFW_PRESS` events to mouse down callbacks
  - Dispatches `GLFW_RELEASE` events to mouse up callbacks
- `_cursor_pos_callback()` - Handles mouse movement events, inverts Y-coordinate, and dispatches as `Vec2`
- `_scroll_callback()` - Handles scroll wheel events and dispatches as `Vec2`

## System Lifecycle Functions

- `input_init()` - Initializes the input system
  - Creates all seven callback arrays using `array_new()`
  - Registers all GLFW callbacks using `glfwSet*Callback()` functions with the global `game_window`
- `input_deinit()` - Cleans up the input system by freeing all callback arrays using `array_free()`

## Key Dependencies

- `game_window` - Global GLFW window handle used for input queries and callback registration
- `game_pixels_to_unit()` - Function for converting pixel coordinates to game units
- `vec2()` - Function for creating 2D vector structures
- GLFW library functions for input handling and callback registration
- Custom array implementation for managing callback lists


### Related
- [Parent](../)

---
Hash: f642ea9b6d5a5ae18c31ce558d5e2270126f817c6849967cf4129421797711f7
Generated: 2025-07-07T20:21:19-04:00
