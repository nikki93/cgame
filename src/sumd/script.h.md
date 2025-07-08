# Summary: script.h

## Table of Contents

- [Script System Header - Core Scripting Interface](#script-system-header---core-scripting-interface)

This header file defines the public interface for a script system that handles script execution, lifecycle management, and event handling integration.

## Header Guard and Dependencies
- Uses standard header guard pattern with `SCRIPT_H`
- Dependencies on other system components:
  - `scalar.h` - for mathematical types like `Vec2`
  - `script_export.h` - for script export functionality
  - `saveload.h` - for persistence via `Store` type
  - `input.h` - for input handling types (`KeyCode`, `MouseCode`)

## Script Execution Functions
- `script_run_string(const char *s)` - executes script code from a string
- `script_run_file(const char *filename)` - executes script code from a file
- `script_error(const char *s)` - handles script errors with non-returning error jump

## System Lifecycle Management
- `script_init()` - initializes the script system
- `script_deinit()` - cleans up and shuts down the script system

## Script Entity Management
- `script_update_all()` - updates all active script entities
- `script_post_update_all()` - performs post-update processing for all script entities
- `script_draw_all()` - handles rendering for all script entities

## Input Event Integration
The script system provides handlers for various input events:
- **Keyboard Events:**
  - `script_key_down(KeyCode key)` - handles key press events
  - `script_key_up(KeyCode key)` - handles key release events
- **Mouse Events:**
  - `script_mouse_down(MouseCode mouse)` - handles mouse button press
  - `script_mouse_up(MouseCode mouse)` - handles mouse button release
  - `script_mouse_move(Vec2 pos)` - handles mouse movement with position
  - `script_scroll(Vec2 scroll)` - handles scroll wheel input

## Persistence Support
- `script_save_all(Store *s)` - saves all script entity states to a store
- `script_load_all(Store *s)` - loads script entity states from a store

This interface suggests a comprehensive script system that can execute code, manage scripted entities through their lifecycle, handle input events, and provide save/load functionality for persistence.

## Sections

### Script System Header - Core Scripting Interface
Source: [script.h:1-23](../script.h#L1-L23)

This header file defines the public interface for a scripting system that handles script execution, game loop integration, input handling, and save/load functionality.

## Header Guard and Dependencies
- Uses standard header guard pattern with `SCRIPT_H` macro
- Dependencies on other modules:
  - `scalar.h` - provides mathematical types like `Vec2`
  - `script_export.h` - likely contains script export functionality
  - `saveload.h` - provides `Store` type for save/load operations
  - `input.h` - provides `KeyCode` and `MouseCode` types

## Core Script Execution Functions
- `script_run_string(const char *s)` - executes script code from a string
- `script_run_file(const char *filename)` - executes script code from a file
- `script_error(const char *s)` - handles script errors with error jump (non-returning function)

## System Lifecycle Management
- `script_init()` - initializes the scripting system
- `script_deinit()` - cleans up and shuts down the scripting system

## Game Loop Integration
- `script_update_all()` - updates all script entities during main game loop
- `script_post_update_all()` - performs post-update operations on all script entities
- `script_draw_all()` - handles rendering for all script entities

## Input Event Handling
The system provides comprehensive input event forwarding to scripts:
- **Keyboard Events:**
  - `script_key_down(KeyCode key)` - handles key press events
  - `script_key_up(KeyCode key)` - handles key release events
- **Mouse Events:**
  - `script_mouse_down(MouseCode mouse)` - handles mouse button press events
  - `script_mouse_up(MouseCode mouse)` - handles mouse button release events
  - `script_mouse_move(Vec2 pos)` - handles mouse movement with 2D position
  - `script_scroll(Vec2 scroll)` - handles scroll wheel events with 2D scroll data

## Save/Load System Integration
- `script_save_all(Store *s)` - serializes all script data to a store
- `script_load_all(Store *s)` - deserializes all script data from a store

This interface suggests a comprehensive scripting system that integrates deeply with the game engine, handling everything from basic script execution to input events and persistent data management.


### Related
- [Parent](../)

---
Hash: e3aacd4cbfb631b411cd90c32c789f3c1e8ee675ba1c914092ac4c045cd10012
Generated: 2025-07-07T20:21:19-04:00
