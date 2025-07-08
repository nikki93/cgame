# Summary: game.c

## Table of Contents

- [Core Game Engine Implementation with OpenGL/GLFW Setup](#core-game-engine-implementation-with-openglglfw-setup)

This file implements the core game engine functionality, providing window management, OpenGL initialization, and the main game loop. Here's a detailed breakdown:

## Global Variables and State
- **game_window**: Global `GLFWwindow*` pointer for the main game window
- **quit**: Static boolean flag to control main loop termination
- **sargc, sargv**: Static storage for command line arguments

## Core Initialization (`_game_init`)
- **GLFW Setup**:
  - Sets error callback to `_glfw_error_callback`
  - Initializes GLFW library
  - Configures OpenGL context (version 3.2, core profile, forward compatibility)
  - Creates 800x600 window titled "cgame"
  - Optionally initializes debug window if `CGAME_DEBUG_WINDOW` is defined

- **OpenGL Setup**:
  - Activates OpenGL context with `glfwMakeContextCurrent`
  - Initializes GLEW with experimental features enabled
  - Clears initial OpenGL error (known GLEW issue)
  - Configures OpenGL state:
    - Enables program point size and blending
    - Sets blend function for alpha transparency
    - Disables depth testing
    - Sets clear color to light gray (0.95, 0.95, 0.95, 1.0)

- **System Initialization**:
  - Seeds random number generator with current time
  - Initializes console and prints welcome message
  - Calls `system_init()` and `test_init()`

## Main Game Loop (`game_run`)
The main entry point that:
1. Stores command line arguments in static variables
2. Calls `_game_init()`
3. Runs main loop while `quit` is false:
   - **Events**: `_game_events()` - polls GLFW events and checks for window close
   - **Update**: `_game_update()` - calls `system_update_all()`
   - **Draw**: `_game_draw()` - handles rendering with special first-frame skip logic
4. Calls `_game_deinit()` for cleanup

## Rendering Logic (`_game_draw`)
- Uses static `first` flag to skip rendering the first frame
- Clears color buffer
- Calls `system_draw_all()` for actual rendering
- Swaps front/back buffers with `glfwSwapBuffers`

## Utility Functions
- **Window Management**:
  - `game_set_window_size(Vec2 s)`: Sets window dimensions
  - `game_get_window_size()`: Returns current window size as `Vec2`
  - `game_set_bg_color(Color c)`: Updates OpenGL clear color

- **Coordinate System Conversion**:
  - `game_unit_to_pixels(Vec2 p)`: Converts normalized coordinates to pixel coordinates
    - Scales by half-window size
    - Translates origin to center with Y-axis flip
  - `game_pixels_to_unit(Vec2 p)`: Converts pixel coordinates to normalized coordinates
    - Inverse of unit_to_pixels transformation

- **Application Control**:
  - `game_quit()`: Sets quit flag to terminate main loop
  - `game_get_argc()`, `game_get_argv()`: Access stored command line arguments

## Dependencies
- **External Libraries**: GLFW, GLEW, standard C libraries
- **Internal Modules**: `system`, `console`, `test`, optional `debugwin`
- **Data Types**: `Vec2`, `Color` (defined elsewhere)
- **Functions**: `system_init/deinit/update_all/draw_all`, `test_init`, `console_puts`

## Error Handling
- GLFW errors are logged to stderr via `_glfw_error_callback`
- OpenGL initialization error is cleared after GLEW setup (known issue workaround)

## Sections

### Core Game Engine Implementation with OpenGL/GLFW Setup
Source: [game.c:1-174](../game.c#L1-L174)

This section implements the main game engine core with OpenGL rendering and GLFW window management. It provides the primary game loop and essential utility functions.

## Key Dependencies
- **External libraries**: GLFW for windowing, GLEW for OpenGL extension loading
- **Internal modules**: `system.h`, `console.h`, `test/test.h`
- **Optional**: `debugwin.h` (conditionally compiled with `CGAME_DEBUG_WINDOW`)

## Global Variables and State
- `GLFWwindow *game_window` - Main game window handle
- `static bool quit` - Controls main game loop exit condition
- `static int sargc, char **sargv` - Stored command line arguments

## Core Functions Defined

### Private Initialization Functions
- **`_glfw_error_callback(int error, const char *desc)`**
  - GLFW error handler that prints errors to stderr

- **`_game_init()`**
  - Sets up GLFW with OpenGL 3.2 core profile
  - Creates 800x600 window titled "cgame"
  - Initializes debug window if `CGAME_DEBUG_WINDOW` is defined
  - Configures OpenGL context and GLEW
  - Sets OpenGL state:
    - Enables program point size, blending (alpha transparency)
    - Disables depth testing
    - Sets light gray background color (0.95, 0.95, 0.95)
  - Seeds random number generator with current time
  - Initializes console, system, and test modules

- **`_game_deinit()`**
  - Cleanup function that deinitializes systems and terminates GLFW

### Game Loop Components
- **`_game_events()`**
  - Polls GLFW events
  - Calls `game_quit()` if window close requested

- **`_game_update()`**
  - Updates all systems via `system_update_all()`

- **`_game_draw()`**
  - Skips first frame to allow full update cycle
  - Clears color buffer
  - Draws all systems via `system_draw_all()`
  - Swaps front/back buffers

### Public API Functions
- **`game_run(int argc, char **argv)`**
  - Main entry point that stores command line arguments
  - Executes complete game lifecycle: init → loop → deinit
  - Game loop continues until `quit` flag is set

- **`game_set_bg_color(Color c)`**
  - Sets OpenGL clear color from Color struct

- **`game_set_window_size(Vec2 s)` / `game_get_window_size()`**
  - Window size management using Vec2 coordinates

- **Coordinate System Conversion**
  - `game_unit_to_pixels(Vec2 p)` - Converts normalized coordinates to pixel coordinates
  - `game_pixels_to_unit(Vec2 p)` - Converts pixel coordinates to normalized coordinates
  - Both functions use window center as origin and scale by half-window dimensions

- **Utility Functions**
  - `game_quit()` - Sets quit flag to exit main loop
  - `game_get_argc()` / `game_get_argv()` - Access stored command line arguments

## Key Implementation Details
- Uses OpenGL 3.2 core profile with forward compatibility
- Implements alpha blending for transparency support
- Coordinate system treats center of window as origin with normalized units
- Skips rendering first frame to ensure proper initialization
- Integrates with modular system architecture via `system_*` functions


### Related
- [Parent](../)

---
Hash: 424680069ce6141c7eb2d69696aa89ef30aad263cd846580b4b1e7e16fe1c32f
Generated: 2025-07-07T20:21:19-04:00
