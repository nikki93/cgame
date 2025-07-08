# Summary: game.h

## Table of Contents

- [Game Module Header - Core Game Engine Interface](#game-module-header---core-game-engine-interface)

This header file defines the main interface for a game engine's core functionality, focusing on window management, coordinate system conversions, and basic game lifecycle control.

## Key Dependencies
- **glew_glfw.h**: OpenGL extension wrangling and GLFW windowing library
- **script_export.h**: Scripting system integration (provides `SCRIPT` and `EXPORT` macros)
- **vec2.h**: 2D vector mathematics
- **color.h**: Color representation

## Global Variables
- **game_window**: `GLFWwindow*` - Global reference to the main game window

## Core Functions

### Game Lifecycle
- **game_run()**: `void game_run(int argc, char **argv)` - Main entry point for the game engine
- **game_get_argc()**: `int` - Retrieves command line argument count passed to game_run
- **game_get_argv()**: `char**` - Retrieves command line arguments passed to game_run
- **game_quit()**: `void` - Terminates the game (script-exported)

### Window Management
- **game_set_window_size()**: `void game_set_window_size(Vec2 s)` - Sets window dimensions in pixels (script-exported)
- **game_get_window_size()**: `Vec2` - Returns current window dimensions in pixels (script-exported)
- **game_set_bg_color()**: `void game_set_bg_color(Color c)` - Sets background color (script-exported)

### Coordinate System Conversion
The engine supports two coordinate systems:
- **Unit coordinates**: Origin (0,0) at screen center, (1,1) at top-right corner
- **Pixel coordinates**: Origin (0,0) at top-left corner, bottom-right at window size

Conversion functions (both script-exported):
- **game_unit_to_pixels()**: `Vec2 game_unit_to_pixels(Vec2 p)` - Converts unit coordinates to pixel coordinates
- **game_pixels_to_unit()**: `Vec2 game_pixels_to_unit(Vec2 p)` - Converts pixel coordinates to unit coordinates

## Scripting Integration
Most functions are wrapped in a `SCRIPT(game, ...)` block with `EXPORT` macros, indicating they are exposed to a scripting system for runtime access. This suggests the engine supports both C/C++ native development and script-based game logic.

## Sections

### Game Module Header - Core Game Engine Interface
Source: [game.h:1-28](../game.h#L1-L28)

This header file defines the main interface for a game engine module with both C and scripting language bindings. It establishes the core game loop, window management, and coordinate system utilities.

## Core Dependencies
- **glew_glfw.h**: OpenGL/GLFW graphics system integration
- **script_export.h**: Scripting system export macros
- **vec2.h**: 2D vector mathematics
- **color.h**: Color representation system

## Global Variables
- **game_window**: External GLFWwindow pointer for the main game window

## Main Entry Point
- **game_run(int argc, char **argv)**: Top-level entry point that initializes and runs the game
- **game_get_argc()**: Returns the argument count passed to game_run
- **game_get_argv()**: Returns the argument vector passed to game_run

## Exported Script Functions
The module exports several functions to scripting languages via the SCRIPT macro:

### Visual Configuration
- **game_set_bg_color(Color c)**: Sets the background color of the game window

### Window Management
- **game_set_window_size(Vec2 s)**: Sets window dimensions in pixels (width, height)
- **game_get_window_size()**: Returns current window dimensions as Vec2

### Coordinate System Conversion
The module defines two coordinate systems:
- **Unit coordinates**: (0,0) at screen center, (1,1) at top-right corner
- **Pixel coordinates**: (0,0) at top-left, game_get_window_size() at bottom-right

Conversion functions:
- **game_unit_to_pixels(Vec2 p)**: Converts from unit coordinates to pixel coordinates
- **game_pixels_to_unit(Vec2 p)**: Converts from pixel coordinates to unit coordinates

### Application Control
- **game_quit()**: Terminates the game application

## Architecture Notes
- Uses conditional compilation guards (`#ifndef GAME_H`)
- Employs a scripting export system that makes C functions available to scripting languages
- Provides dual coordinate system support for different use cases (normalized vs. pixel-based positioning)


### Related
- [Parent](../)

---
Hash: 3bd5b6908399f15aedc3e54396e0501c5e7a4ed5480b800e8a3b2f564a37ffd5
Generated: 2025-07-07T20:21:19-04:00
