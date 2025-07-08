# Summary: test/keyboard_controlled.h

## Table of Contents

- [Keyboard Controlled Component System Header](#keyboard-controlled-component-system-header)

This header file defines a component system for managing keyboard-controlled entities in what appears to be an entity-component-system (ECS) architecture.

## Key Components and Structure

### Header Guards and Dependencies
- Uses standard header guard pattern with `KEYBOARD_CONTROLLED_H`
- Dependencies on three other modules:
  - `saveload.h` - for persistence functionality
  - `entity.h` - for the Entity type definition
  - `script_export.h` - for the SCRIPT macro and EXPORT functionality

### Script-Exported Interface
The file uses a `SCRIPT` macro to define script-exportable functions for the `keyboard_controlled` component:
- **`keyboard_controlled_add(Entity ent)`** - Adds keyboard control capability to an entity
- **`keyboard_controlled_remove(Entity ent)`** - Removes keyboard control from an entity  
- **`keyboard_controlled_has(Entity ent)`** - Checks if an entity has keyboard control (returns bool)

### Core System Functions
Three main system-level functions for managing all keyboard-controlled entities:
- **`keyboard_controlled_update_all()`** - Updates all entities with keyboard control (likely processes input)
- **`keyboard_controlled_save_all(Store *s)`** - Persists all keyboard-controlled entity data to a Store
- **`keyboard_controlled_load_all(Store *s)`** - Restores keyboard-controlled entity data from a Store

### Architecture Notes
- Follows typical ECS component pattern with add/remove/has operations per entity
- Integrates with a scripting system (functions marked with EXPORT can be called from scripts)
- Supports save/load functionality using a `Store` type for game state persistence
- Uses `Entity` type suggesting this is part of a larger entity management system

This appears to be a clean, minimal interface for managing which entities respond to keyboard input in a game or interactive application.

## Sections

### Keyboard Controlled Component System Header
Source: [keyboard_controlled.h:1-20](../test/keyboard_controlled.h#L1-L20)

This header file defines the interface for a keyboard-controlled component system in what appears to be an entity-component-system (ECS) architecture.

## Key Code Entities Defined

**Header Guard:**
- `KEYBOARD_CONTROLLED_H` - Standard include guard macro

**Script-Exported Functions:**
- `keyboard_controlled_add(Entity ent)` - Adds keyboard control component to an entity
- `keyboard_controlled_remove(Entity ent)` - Removes keyboard control component from an entity  
- `keyboard_controlled_has(Entity ent)` - Checks if an entity has keyboard control component

**System Functions:**
- `keyboard_controlled_update_all()` - Updates all entities with keyboard control components
- `keyboard_controlled_save_all(Store *s)` - Saves all keyboard control component data to storage
- `keyboard_controlled_load_all(Store *s)` - Loads all keyboard control component data from storage

## Dependencies and References

**External Dependencies:**
- `saveload.h` - Provides the `Store` type used for persistence operations
- `entity.h` - Provides the `Entity` type used throughout the component system
- `script_export.h` - Provides the `SCRIPT` macro and `EXPORT` functionality for exposing functions to scripting

## Component System Architecture

**Script Integration:**
- Uses `SCRIPT(keyboard_controlled, ...)` macro to expose three core component functions to a scripting system
- All script-exported functions use `EXPORT` modifier for external visibility

**Component Management Pattern:**
- Follows standard ECS component pattern with add/remove/has operations
- Provides bulk update functionality for all entities with this component
- Includes persistence support through save/load operations

**System Responsibilities:**
- Entity-component association management
- Batch processing of keyboard-controlled entities
- Data persistence for component state

This appears to be part of a game engine or interactive application where entities can be marked as keyboard-controllable, with the system handling input processing and state management for all such entities.


### Related
- [Parent](../)

---
Hash: f2e5981e93aceb79832bc41a68dfc9b76440b9f4dd6224cd05dc62b1abd82fce
Generated: 2025-07-07T20:21:19-04:00
