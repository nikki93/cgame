# Summary: cgame_ffi.h

## Table of Contents

- [Lua FFI Binding Configuration for cgame Modules](#lua-ffi-binding-configuration-for-cgame-modules)

This header file serves as the central configuration for exposing C game engine functionality to Lua scripts through a Foreign Function Interface (FFI).

## Purpose and Structure

• **Primary Function**: Defines the C interface that will be accessible from Lua under the `cgame.` namespace
• **Mechanism**: Uses `SCRIPT(...)` blocks from various modules to generate Lua bindings
• **Guard Macro**: Defines `__CGAME_FFI__` to prevent multiple inclusions

## Module Dependencies

The file includes headers for all game engine modules that expose Lua functionality:

• **Core Math/Geometry**: 
  - `scalar.h` - scalar mathematics
  - `vec2.h` - 2D vector operations
  - `mat3.h` - 3x3 matrix operations  
  - `bbox.h` - bounding box functionality
  - `color.h` - color management

• **Engine Systems**:
  - `game.h` - core game loop and state
  - `saveload.h` - save/load functionality
  - `system.h` - system management
  - `timing.h` - timing and frame rate control

• **Input/Output**:
  - `fs.h` - file system operations
  - `input.h` - input handling
  - `console.h` - debug console
  - `sound.h` - audio system

• **Game Objects**:
  - `entity.h` - entity system
  - `prefab.h` - prefab system
  - `transform.h` - transformation components
  - `camera.h` - camera system
  - `sprite.h` - sprite rendering
  - `physics.h` - physics simulation

• **Tools**:
  - `gui.h` - graphical user interface
  - `edit.h` - editing tools

• **Test Components**:
  - `test/keyboard_controlled.h` - keyboard control testing

## FFI Registration Array

**Key Data Structure**: `cgame_ffi[]` - static array of string pointers

• **Contents**: Each entry points to a `cgame_ffi_NAME` string variable where `NAME` corresponds to the module's `SCRIPT(NAME, ...)` declaration
• **Ordering Constraint**: Dependencies must be respected - modules that reference types from other modules must come after their dependencies (e.g., `transform` uses `Mat3`, so `mat3` must precede `transform`)

**Registered Modules** (in dependency order):
1. `cgame_ffi_scalar`
2. `cgame_ffi_saveload`
3. `cgame_ffi_vec2`
4. `cgame_ffi_mat3`
5. `cgame_ffi_bbox`
6. `cgame_ffi_color`
7. `cgame_ffi_fs`
8. `cgame_ffi_game`
9. `cgame_ffi_system`
10. `cgame_ffi_input`
11. `cgame_ffi_entity`
12. `cgame_ffi_prefab`
13. `cgame_ffi_timing`
14. `cgame_ffi_transform`
15. `cgame_ffi_camera`
16. `cgame_ffi_sprite`
17. `cgame_ffi_gui`
18. `cgame_ffi_console`
19. `cgame_ffi_sound`
20. `cgame_ffi_physics`
21. `cgame_ffi_edit`
22. `cgame_ffi_keyboard_controlled`

## Configuration Constants

• **`n_cgame_ffi`**: Static constant containing the count of registered FFI modules, calculated using `sizeof` array division

## Usage Instructions

The comments provide clear guidance for adding new modules:
• Include the relevant header file
• Add `&cgame_ffi_NAME` to the array where `NAME` matches the `SCRIPT(NAME, ...)` declaration
• Maintain proper dependency ordering in the array

## Sections

### Lua FFI Binding Configuration for cgame Modules
Source: [cgame_ffi.h:1-70](../cgame_ffi.h#L1-L70)

This section defines the C interface configuration that exposes cgame functionality to Lua through FFI (Foreign Function Interface) bindings.

## Key Components

### Header Guard and Includes
- **`__CGAME_FFI__`**: Preprocessor definition serving as a header guard
- **Module includes**: 22 header files are included, covering core game engine functionality:
  - Math utilities: `scalar.h`, `vec2.h`, `mat3.h`, `bbox.h`
  - Core systems: `game.h`, `system.h`, `entity.h`, `timing.h`
  - Graphics: `sprite.h`, `camera.h`, `gui.h`, `color.h`
  - I/O and utilities: `input.h`, `fs.h`, `console.h`, `sound.h`
  - Game features: `physics.h`, `transform.h`, `prefab.h`, `edit.h`
  - Save system: `saveload.h`
  - Testing: `test/keyboard_controlled.h`

### FFI Binding Array
- **`cgame_ffi`**: Static array of string pointers that references FFI binding definitions
  - Contains 22 entries corresponding to each included module
  - Each entry follows the naming pattern `&cgame_ffi_NAME` where NAME matches the module
  - **Dependency ordering**: Array order respects module dependencies (e.g., `mat3` comes before `transform` since transform uses Mat3)

### Configuration Metadata
- **`n_cgame_ffi`**: Static constant that calculates the number of FFI bindings using `sizeof` arithmetic

## Module Binding References
The array contains these specific FFI binding references:
- `cgame_ffi_scalar`, `cgame_ffi_saveload`, `cgame_ffi_vec2`, `cgame_ffi_mat3`
- `cgame_ffi_bbox`, `cgame_ffi_color`, `cgame_ffi_fs`, `cgame_ffi_game`
- `cgame_ffi_system`, `cgame_ffi_input`, `cgame_ffi_entity`, `cgame_ffi_prefab`
- `cgame_ffi_timing`, `cgame_ffi_transform`, `cgame_ffi_camera`, `cgame_ffi_sprite`
- `cgame_ffi_gui`, `cgame_ffi_console`, `cgame_ffi_sound`, `cgame_ffi_physics`
- `cgame_ffi_edit`, `cgame_ffi_keyboard_controlled`

## Usage Context
- All declarations in `SCRIPT(...)` blocks from the included modules become available under the `cgame.` namespace in Lua
- This configuration serves as the bridge between C game engine functionality and Lua scripting capabilities
- The dependency ordering ensures proper symbol resolution during FFI binding generation


### Related
- [Parent](../)

---
Hash: 8aaa1682c113872e71fd30935bd8d93f203c2cbca4cabe6c49eaa7a0f105ee53
Generated: 2025-07-07T20:21:19-04:00
