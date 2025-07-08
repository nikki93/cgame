# Summary: console.c

## Table of Contents

- [Console System Implementation - Text Display and Output Management](#console-system-implementation---text-display-and-output-management)

This section implements a console system that manages text output using a circular buffer and displays it through a GUI text entity. The console supports both standard output and in-game display with text wrapping capabilities.

## Core Data Structures

• **Circular Buffer Storage**:
  - `lines[NUM_LINES][LINE_LEN]` - Static 2D array storing console lines (20 lines × 128 chars each)
  - `top` - Integer tracking the current top line in the circular buffer
  - `text` - Entity reference for the GUI text component that displays console contents

## Key Functions Defined

• **Entity Management**:
  - `console_set_entity(Entity ent)` - Associates a GUI text entity with the console
  - `console_get_entity()` - Returns the current console text entity
  - `console_set_visible(bool visible)` - Controls console visibility via parent transform
  - `console_get_visible()` - Returns current visibility state

• **Text Output Functions**:
  - `console_puts(const char *s)` - Prints string with newline to both stdout and console
  - `console_printf(const char *fmt, ...)` - Formatted printing using variadic arguments

• **Lifecycle Functions**:
  - `console_init()` - Initializes console by setting text entity to nil
  - `console_deinit()` - Cleanup function (currently empty)

## Internal Helper Functions

• **`_update_text()`** - Updates the GUI display:
  - Allocates temporary buffer for all console lines
  - Iterates through circular buffer starting from current top
  - Concatenates all non-empty lines into single string
  - Calls `gui_text_set_str()` to update display
  - Frees temporary buffer

• **`_write(const char *s)`** - Core text processing with wrapping:
  - Calculates display width based on window size (capped at LINE_LEN-2)
  - Handles tab expansion to 4-space boundaries
  - Implements line wrapping when text exceeds width
  - Advances circular buffer when newlines encountered
  - Adds wrap prefix (character 26 + space) to continuation lines
  - Null-terminates lines and updates display

• **`_print(const char *s)`** - Dual output function:
  - Prints to stdout with flushing
  - Calls `_write()` for console buffer storage

## Key Dependencies

• **External Headers**: `stdarg.h`, `stdbool.h`, `string.h`, `stdio.h`, `stdlib.h`
• **Internal Modules**: `vec2.h`, `game.h`, `input.h`, `gui.h`, `transform.h`
• **Entity System**: Uses `Entity` type, `entity_nil`, `entity_eq()`
• **GUI System**: Calls `gui_text_add()`, `gui_text_set_str()`, `gui_set_visible()`, `gui_get_visible()`
• **Transform System**: Uses `transform_get_parent()` for visibility control
• **Game System**: Calls `game_get_window_size()` for dynamic width calculation

## Algorithm Details

• **Circular Buffer Management**:
  - Buffer wraps using modulo arithmetic: `(top + i) % NUM_LINES`
  - New lines advance top pointer and reset cursor position
  - Maintains fixed memory footprint regardless of output volume

• **Text Wrapping Logic**:
  - Dynamic width calculation based on window size divided by 10
  - Tab stops align to 4-character boundaries
  - Wrapped lines marked with special prefix character (ASCII 26)
  - Forced newlines when content exceeds calculated width

## Sections

### Console System Implementation - Text Display and Output Management
Source: [console.c:1-176](../console.c#L1-L176)

This section implements a console system that manages text output display within a game engine, providing both visual console functionality and stdout output.

## Core Data Structures

- **`lines[NUM_LINES][LINE_LEN]`**: Static circular buffer storing console text lines
  - `NUM_LINES` = 20 lines maximum
  - `LINE_LEN` = 128 characters per line (including newline and null terminator)
- **`top`**: Static integer tracking the current top line in the circular buffer
- **`text`**: Static Entity representing the GUI text component that displays console contents

## Key Functions Defined

### Entity Management
- **`console_set_entity(Entity ent)`**: Associates a GUI text entity with the console
  - Calls `gui_text_add()` to register the entity
  - Updates display via `_update_text()`
- **`console_get_entity()`**: Returns the current console text entity

### Visibility Control
- **`console_set_visible(bool visible)`**: Shows/hides the console
  - Uses `gui_set_visible()` on the entity's parent transform
- **`console_get_visible()`**: Returns current visibility state

### Text Output Functions
- **`console_puts(const char *s)`**: Prints string with automatic newline
- **`console_printf(const char *fmt, ...)`**: Formatted output using variadic arguments
  - Uses `vsnprintf()` to calculate required buffer size
  - Dynamically allocates memory for formatted string

### Internal Helper Functions
- **`_update_text()`**: Updates the GUI display
  - Concatenates all non-empty lines from circular buffer
  - Calls `gui_text_set_str()` to update display
- **`_write(const char *s)`**: Core text writing with line wrapping
  - Maintains static cursor position
  - Calculates wrap width based on window size (`game_get_window_size().x / 10`)
  - Handles tab characters with 4-space tabstops
  - Automatically wraps lines at width boundary
  - Adds wrap prefix (character 26 + space) to continuation lines
- **`_print(const char *s)`**: Dual output function
  - Sends text to both stdout (via `printf()`) and console display
  - Calls `fflush(stdout)` to ensure immediate output

### Initialization
- **`console_init()`**: Initializes console system (sets `text` to `entity_nil`)
- **`console_deinit()`**: Cleanup function (currently empty)

## Key Dependencies

The console system integrates with several other engine components:
- **GUI system**: `gui_text_add()`, `gui_text_set_str()`, `gui_set_visible()`, `gui_get_visible()`
- **Transform system**: `transform_get_parent()`
- **Game system**: `game_get_window_size()`
- **Entity system**: Uses `Entity` type and `entity_nil` constant

## Text Processing Logic

The `_write()` function implements sophisticated text processing:
1. **Width calculation**: Uses window width divided by 10, capped at `LINE_LEN - 2`
2. **Tab handling**: Expands tabs to 4-space boundaries, wrapping if necessary
3. **Line wrapping**: Automatically wraps at width boundary, adding continuation prefix
4. **Circular buffer management**: Advances `top` pointer when lines are completed
5. **Null termination**: Ensures all lines are properly null-terminated

The system maintains both a visual console display and mirrors output to stdout, making it suitable for both in-game debugging and external logging.


### Related
- [Parent](../)

---
Hash: d0f8e6680844b41ccd8d4e5083323e2e3c891a3fcbae1c6563c8443c1e0a4e70
Generated: 2025-07-07T20:21:19-04:00
