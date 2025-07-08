# Summary: camera.h

## Table of Contents

- [Camera System Header - Entity-based Camera Management](#camera-system-header---entity-based-camera-management)

This header file defines the camera system interface for a 2D game engine, providing camera entity management and coordinate transformations between world space and screen space.

## Key Dependencies
- **saveload.h** - For persistence functionality
- **entity.h** - For Entity type system
- **vec2.h** - For Vec2 2D vector operations
- **mat3.h** - For Mat3 3x3 matrix operations
- **script_export.h** - For SCRIPT macro and EXPORT functionality

## Camera Entity Management Functions
- **camera_add(Entity ent)** - Adds camera component to an entity
- **camera_remove(Entity ent)** - Removes camera component from an entity
- **camera_has(Entity ent)** - Checks if entity has camera component

## Camera Selection and Control
- **camera_set_edit_camera(Entity ent)** - Sets camera for edit mode (not persisted)
- **camera_set_current(Entity ent, bool current)** - Sets/unsets entity as current camera
- **camera_get_current(Entity ent)** - Checks if entity is the current camera
- **camera_set_current_camera(Entity ent)** - Sets the active camera entity
- **camera_get_current_camera()** - Returns current camera entity (entity_nil if none)

## Camera Properties
- **camera_set_viewport_height(Entity ent, Scalar height)** - Sets vertical world units visible on screen
- **camera_get_viewport_height(Entity ent)** - Gets vertical world units visible on screen

## View Matrix and Coordinate Transformations
- **camera_get_inverse_view_matrix()** - Returns Mat3 inverse view matrix
- **camera_get_inverse_view_matrix_ptr()** - Returns pointer to inverse view matrix for GLSL binding
- **Coordinate conversion functions:**
  - **camera_world_to_pixels(Vec2 p)** - Converts world coordinates to screen pixels
  - **camera_world_to_unit(Vec2 p)** - Converts world coordinates to unit coordinates
  - **camera_pixels_to_world(Vec2 p)** - Converts screen pixels to world coordinates
  - **camera_unit_to_world(Vec2 p)** - Converts unit coordinates to world coordinates

## System Management Functions
- **camera_init()** - Initializes camera system
- **camera_deinit()** - Cleans up camera system
- **camera_update_all()** - Updates all camera entities
- **camera_save_all(Store *s)** - Saves all camera data to store
- **camera_load_all(Store *s)** - Loads all camera data from store

## Default Behavior
The system provides a fallback behavior where if no current camera exists, the inverse view matrix defaults to identity, resulting in a 2x2 unit box view centered at the world origin.

## Script Export Structure
Most functions are wrapped in a SCRIPT macro with EXPORT declarations, indicating they are available to the scripting system for runtime camera control.

## Sections

### Camera System Header - Entity-based Camera Management
Source: [camera.h:1-51](../camera.h#L1-L51)

This header file defines a comprehensive camera system for a 2D game engine that manages multiple camera entities with coordinate transformations and viewport controls.

## Core System Overview
- **Default behavior**: When no camera is active, the inverse view matrix is identity, creating a 2x2 unit box view centered at world origin
- **Entity-based**: Cameras are implemented as entities in the entity-component system
- **Script integration**: Most functions are exported to scripting via `SCRIPT(camera, ...)` macro

## Key Dependencies
- `entity.h` - Entity system for camera management
- `vec2.h` - 2D vector operations for coordinates
- `mat3.h` - 3x3 matrix operations for transformations
- `saveload.h` - Serialization support
- `script_export.h` - Script binding macros

## Camera Entity Management Functions
- `camera_add(Entity ent)` - Adds camera component to entity
- `camera_remove(Entity ent)` - Removes camera component from entity  
- `camera_has(Entity ent)` - Checks if entity has camera component

## Camera State Management
- **Active camera control**:
  - `camera_set_current(Entity ent, bool current)` - Sets entity as current camera
  - `camera_get_current(Entity ent)` - Checks if entity is current camera
  - `camera_set_current_camera(Entity ent)` - Direct current camera setter
  - `camera_get_current_camera()` - Returns current camera entity (entity_nil if none)
- **Edit mode camera**:
  - `camera_set_edit_camera(Entity ent)` - Sets edit-only camera (not persisted)

## Viewport Configuration
- `camera_set_viewport_height(Entity ent, Scalar height)` - Sets world units to fit vertically on screen
- `camera_get_viewport_height(Entity ent)` - Gets current viewport height

## Coordinate Transformation System
- **Matrix access**:
  - `camera_get_inverse_view_matrix()` - Returns transformation matrix
  - `camera_get_inverse_view_matrix_ptr()` - Returns pointer for GLSL shader binding
- **World ↔ Screen transformations**:
  - `camera_world_to_pixels(Vec2 p)` - World coordinates to screen pixels
  - `camera_pixels_to_world(Vec2 p)` - Screen pixels to world coordinates
  - `camera_world_to_unit(Vec2 p)` - World coordinates to normalized units
  - `camera_unit_to_world(Vec2 p)` - Normalized units to world coordinates

## System Lifecycle Functions
- `camera_init()` - Initialize camera system
- `camera_deinit()` - Clean up camera system
- `camera_update_all()` - Update all camera entities per frame
- `camera_save_all(Store *s)` - Serialize all camera data
- `camera_load_all(Store *s)` - Deserialize camera data

## Notable Design Features
- Supports multiple camera entities with one active at a time
- Separate edit-mode camera that doesn't persist
- Comprehensive coordinate system conversion utilities
- Graphics shader integration via matrix pointer access
- Full save/load support for camera configurations


### Related
- [Parent](../)

---
Hash: eed76f8120c6882cb848b3012405847812c8e97da503918d0bb8d1478e42aff0
Generated: 2025-07-07T20:21:19-04:00
