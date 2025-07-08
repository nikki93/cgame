# Summary: camera.c

## Table of Contents

- [Camera System Implementation](#camera-system-implementation)

This file implements a camera system for a game engine that manages viewport rendering and coordinate transformations.

## Core Data Structure
- **Camera struct**: Contains an `EntityPoolElem pool_elem` and `Scalar viewport_height` (default 1.0)

## Global State Variables
- **curr_camera**: The currently active camera entity
- **edit_camera**: Camera used specifically for edit mode
- **inverse_view_matrix**: Cached inverse view matrix (Mat3) for performance optimization
- **pool**: EntityPool pointer for managing camera entities

## Entity Management Functions
- **camera_add(Entity ent)**: Adds camera component to entity, ensures transform component exists, sets as current camera if none exists
- **camera_remove(Entity ent)**: Removes camera from pool, clears current camera if it was the removed one
- **camera_has(Entity ent)**: Checks if entity has camera component

## Camera Control Functions
- **camera_set_edit_camera(Entity ent)**: Sets the edit mode camera
- **camera_set_current(Entity ent, bool current)**: Sets or unsets entity as current camera
- **camera_get_current(Entity ent)**: Checks if entity is the current camera
- **camera_set_current_camera(Entity ent)**: Direct setter for current camera
- **camera_get_current_camera()**: Returns edit camera if edit mode is enabled, otherwise returns current camera

## Viewport Configuration
- **camera_set_viewport_height(Entity ent, Scalar height)**: Sets viewport height for specified camera
- **camera_get_viewport_height(Entity ent)**: Gets viewport height for specified camera

## Matrix Access Functions
- **camera_get_inverse_view_matrix()**: Returns cached inverse view matrix by value
- **camera_get_inverse_view_matrix_ptr()**: Returns pointer to cached inverse view matrix

## Coordinate Transformation Functions
- **camera_world_to_pixels(Vec2 p)**: Converts world coordinates to pixel coordinates
  - Uses cached inverse view matrix, then converts to pixels via game system
- **camera_world_to_unit(Vec2 p)**: Converts world coordinates to unit coordinates using cached inverse view matrix
- **camera_pixels_to_world(Vec2 p)**: Converts pixel coordinates to world coordinates
  - First converts pixels to unit coordinates, then to world coordinates
- **camera_unit_to_world(Vec2 p)**: Converts unit coordinates to world coordinates using current camera's transform

## System Lifecycle Functions
- **camera_init()**: Initializes camera system
  - Creates entity pool
  - Sets cameras to entity_nil
  - Initializes inverse view matrix to identity
- **camera_deinit()**: Cleans up by freeing entity pool

## Update System
- **camera_update_all()**: Main update function that:
  - Removes destroyed entities from pool
  - Calculates window aspect ratio from game window size
  - Updates all camera scales based on viewport height and aspect ratio
  - Updates bounding boxes for edit mode
  - Recalculates inverse view matrix from current camera's world transform matrix

## Save/Load System
- **camera_save_all(Store *s)**: Saves camera system state including current camera, inverse view matrix, and all camera viewport heights
- **camera_load_all(Store *s)**: Loads camera system state with appropriate defaults

## Key Dependencies
- **entitypool.h**: For entity pool management
- **transform.h**: For transform components and world matrix operations
- **game.h**: For window size and pixel/unit conversions
- **edit.h**: For edit mode detection and bounding box updates
- **saveload.h**: For serialization functionality
- **error.h**: For assertions

The system uses an entity-component architecture where cameras are components that can be attached to entities, with the transform system handling positioning and the camera system handling viewport and coordinate transformations.

## Sections

### Camera System Implementation
Source: [camera.c:1-199](../camera.c#L1-L199)

This section implements a complete camera system for a game engine, providing viewport management, coordinate transformations, and entity-based camera control.

## Core Data Structures

**Camera struct:**
- Contains `EntityPoolElem pool_elem` for entity pool management
- Contains `Scalar viewport_height` to define the camera's vertical viewing area

**Static global variables:**
- `curr_camera`: The currently active camera entity
- `edit_camera`: Special camera used during edit mode
- `inverse_view_matrix`: Cached 3x3 inverse view matrix for performance
- `pool`: EntityPool pointer for managing all camera entities

## Camera Entity Management

**Entity lifecycle functions:**
- `camera_add(Entity ent)`: Adds camera component to entity
  - Ensures transform component is added
  - Sets default viewport_height to 1.0
  - Automatically sets as current camera if none exists
- `camera_remove(Entity ent)`: Removes camera from entity pool
  - Clears curr_camera if removing the active camera
- `camera_has(Entity ent)`: Checks if entity has camera component

## Camera Control and Selection

**Current camera management:**
- `camera_set_current(Entity ent, bool current)`: Sets/unsets entity as current camera
- `camera_get_current(Entity ent)`: Checks if entity is the current camera
- `camera_set_current_camera(Entity ent)`: Direct setter for current camera
- `camera_get_current_camera()`: Returns active camera entity
  - Returns `edit_camera` if edit mode is enabled
  - Otherwise returns `curr_camera`

**Edit mode support:**
- `camera_set_edit_camera(Entity ent)`: Sets the camera used during editing

## Viewport Configuration

**Viewport height management:**
- `camera_set_viewport_height(Entity ent, Scalar height)`: Sets camera's vertical viewing area
- `camera_get_viewport_height(Entity ent)`: Gets camera's viewport height

## Coordinate System Transformations

**Matrix access:**
- `camera_get_inverse_view_matrix()`: Returns cached inverse view matrix
- `camera_get_inverse_view_matrix_ptr()`: Returns pointer to cached matrix

**World-to-screen coordinate conversion:**
- `camera_world_to_pixels(Vec2 p)`: World coordinates → pixel coordinates
  - Calls `camera_world_to_unit()` then `game_unit_to_pixels()`
- `camera_world_to_unit(Vec2 p)`: World coordinates → unit coordinates
  - Uses cached inverse view matrix transformation

**Screen-to-world coordinate conversion:**
- `camera_pixels_to_world(Vec2 p)`: Pixel coordinates → world coordinates
  - Calls `game_pixels_to_unit()` then `camera_unit_to_world()`
- `camera_unit_to_world(Vec2 p)`: Unit coordinates → world coordinates
  - Uses current camera's transform matrix via `transform_local_to_world()`

## System Management

**Initialization and cleanup:**
- `camera_init()`: Initializes camera system
  - Creates entity pool
  - Sets cameras to entity_nil
  - Initializes inverse view matrix to identity
- `camera_deinit()`: Cleans up entity pool

**Per-frame updates:**
- `camera_update_all()`: Updates all cameras each frame
  - Removes destroyed entities from pool
  - Calculates aspect ratio from window size
  - Updates each camera's scale based on viewport_height and aspect ratio
  - Updates edit bounding boxes
  - Recalculates inverse view matrix for current camera

## Save/Load System

**Persistence functions:**
- `camera_save_all(Store *s)`: Saves all camera state
  - Saves current camera entity (if marked for saving)
  - Saves cached inverse view matrix
  - Saves viewport_height for each camera in pool
- `camera_load_all(Store *s)`: Loads camera state
  - Loads current camera entity
  - Loads inverse view matrix (defaults to identity)
  - Loads viewport_height for each camera (defaults to 1)

## Key Dependencies

**External modules referenced:**
- `transform.h`: For entity transformation matrices
- `game.h`: For window size and pixel/unit conversions
- `entitypool.h`: For entity component management
- `edit.h`: For edit mode detection and bounding box updates
- `saveload.h`: For persistence operations
- `error.h`: For assertions


### Related
- [Parent](../)

---
Hash: d403eae2b958fe80be9c36716ecf52bbccd359e87f9b9fb2ab73f99e6a4981dd
Generated: 2025-07-07T20:21:19-04:00
