# Summary: console.h

## Table of Contents

- [Console System Header Definition](#console-system-header-definition)

This header file defines a console system that provides debug output functionality through an entity-based architecture. The console can be attached to game entities and supports both visibility control and formatted text output.

## Key Features and Components

### Header Guard and Dependencies
- Uses standard header guard pattern with `CONSOLE_H`
- Depends on:
  - `entity.h` - for Entity type definitions
  - `script_export.h` - for SCRIPT macro and EXPORT functionality

### Exported Script Interface
The console system exposes a scripting interface through the `SCRIPT(console, ...)` macro containing:

**Entity Management:**
- `console_set_entity(Entity ent)` - Associates console output with a specific entity
  - Automatically adds entity to gui_text system if not already present
  - Pass `entity_nil` to disable console output
- `console_get_entity()` - Returns currently associated entity or `entity_nil` if none set

**Visibility Control:**
- `console_set_visible(bool visible)` - Controls console visibility state
- `console_get_visible()` - Returns current visibility state

**Text Output Functions:**
- `console_puts(const char *s)` - Outputs a string to the console
- `console_printf(const char *fmt, ...)` - Formatted output with variable arguments (printf-style)

### System Management Functions
**Lifecycle Management:**
- `console_init()` - Initializes the console system
- `console_deinit()` - Cleans up console system resources

## Architecture Notes
- The console system integrates with a GUI text rendering system
- Uses entity-based architecture where console output is tied to specific game entities
- Provides both C-style and script-accessible interfaces
- All script-exported functions use the `EXPORT` macro for external visibility

This system appears designed for debugging and development purposes, allowing developers to display console output within the game world through the entity system rather than traditional separate console windows.

## Sections

### Console System Header Definition
Source: [console.h:1-27](../console.h#L1-L27)

This header file defines the interface for a console system that provides text output functionality with GUI integration and scripting support.

## Header Structure
- **Include guard**: `CONSOLE_H` prevents multiple inclusions
- **Dependencies**: 
  - `entity.h` - provides Entity type for GUI integration
  - `script_export.h` - provides SCRIPT macro and EXPORT functionality

## Exported Script Interface
The file defines a script-exportable interface using the `SCRIPT(console, ...)` macro with the following functions:

### Entity Management
- **`console_set_entity(Entity ent)`**: Associates console output with a specific entity
  - Automatically adds entity to gui_text system if not already present
  - Pass `entity_nil` to disable entity-based output
- **`console_get_entity()`**: Returns the currently associated entity
  - Returns `entity_nil` if no entity is set

### Visibility Control
- **`console_set_visible(bool visible)`**: Controls console visibility state
- **`console_get_visible()`**: Returns current visibility state

### Text Output Functions
- **`console_puts(const char *s)`**: Outputs a string to the console
- **`console_printf(const char *fmt, ...)`**: Formatted output using printf-style formatting

## Internal Management Functions
These functions are not exported to scripts:
- **`console_init()`**: Initializes the console system
- **`console_deinit()`**: Cleans up console system resources

## Key Design Features
- **GUI Integration**: Console output is tied to the entity/GUI system through entity association
- **Script Accessibility**: All primary functions are exported for use in scripting environments
- **Flexible Output**: Supports both simple string output and formatted printf-style output
- **Optional Display**: Entity can be disabled by setting to `entity_nil`


### Related
- [Parent](../)

---
Hash: 6ee71cd5b9551ef0fe17f39ba20715069cd24c5796b34dc720089d672a951c7d
Generated: 2025-07-07T20:21:19-04:00
