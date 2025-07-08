# Summary: system.h

## Table of Contents

- [System Module Header - Core System Management Interface](#system-module-header---core-system-management-interface)

This header file defines the main system management interface for what appears to be a game or application engine. It provides both script-exportable and internal C functions for system lifecycle management.

## Header Guard and Dependencies
- Uses standard header guard pattern with `SYSTEM_H`
- Depends on two other modules:
  - `saveload.h` - for save/load functionality
  - `script_export.h` - for script binding macros

## Script-Exported Functions
The file uses a `SCRIPT(system, ...)` macro to export functions to a scripting system:
- `system_load_all(Store *f)` - loads all system data from a Store object
- `system_save_all(Store *f)` - saves all system data to a Store object
- Both functions are marked with `EXPORT` and take a `Store *f` parameter

## Internal C Functions
Four core system management functions are declared for internal use:
- `system_init()` - initializes the system
- `system_deinit()` - cleans up/deinitializes the system  
- `system_update_all()` - updates all system components (likely called per frame)
- `system_draw_all()` - renders all system components (likely called per frame)

## Key References
- `Store` type - used for save/load operations (defined elsewhere)
- `SCRIPT` macro - enables script binding (defined in script_export.h)
- `EXPORT` macro - marks functions for export to scripting (defined in script_export.h)

This appears to be a central system manager that coordinates initialization, updating, rendering, and persistence of various game/application subsystems.

## Sections

### System Module Header - Core System Management Interface
Source: [system.h:1-19](../system.h#L1-L19)

This header file defines the main interface for the system module, which appears to handle core system lifecycle management and provides save/load functionality.

## Header Guard and Dependencies
• Uses standard header guard pattern with `SYSTEM_H` macro
• Includes two external dependencies:
  - `saveload.h` - likely contains `Store` type definition for persistence operations
  - `script_export.h` - provides `SCRIPT` macro and `EXPORT` functionality for scripting integration

## Script-Exported Functions
• **SCRIPT macro block** defines two functions exported to scripting system:
  - `system_load_all(Store *f)` - loads all system data from a Store object
  - `system_save_all(Store *f)` - saves all system data to a Store object
• Both functions use `EXPORT` modifier indicating they're accessible from external scripts

## Core System Management Functions
• **system_init()** - initializes the system (no parameters, void return)
• **system_deinit()** - deinitializes/cleans up the system (no parameters, void return)
• **system_update_all()** - updates all system components (no parameters, void return)
• **system_draw_all()** - handles rendering/drawing of all system components (no parameters, void return)

## Key Architectural Patterns
• **Lifecycle Management**: Clear init/deinit pattern for resource management
• **Game Loop Integration**: Separate update and draw phases typical of game engines
• **Persistence Layer**: Integration with save/load system through Store abstraction
• **Script Integration**: Selective exposure of save/load functionality to scripting layer

This appears to be a central system manager that coordinates initialization, updates, rendering, and persistence across the application, with particular emphasis on game-like update/draw cycles and scriptable save/load operations.


### Related
- [Parent](../)

---
Hash: 2cfa81212a9103eba31693ab93bb86f8fb2d1ef9070b864d28b74f9e8d7f413f
Generated: 2025-07-07T20:21:19-04:00
