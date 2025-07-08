# Summary: .

## Table of Contents

- [Dynamic Array Implementation with Generic Type Support (array.c)](#dynamic-array-implementation-with-generic-type-support-arrayc)
- [Dynamic Array Header - Generic Resizable Array Implementation (array.h)](#dynamic-array-header---generic-resizable-array-implementation-arrayh)
- [Bounding Box Operations Implementation (bbox.c)](#bounding-box-operations-implementation-bboxc)
- [Bounding Box Data Structure and Operations Header (bbox.h)](#bounding-box-data-structure-and-operations-header-bboxh)
- [Camera System Implementation (camera.c)](#camera-system-implementation-camerac)
- [Camera System Interface and Management (camera.h)](#camera-system-interface-and-management-camerah)
- [Lua-C FFI Interface Header and Module Registration (cgame_ffi.h)](#lua-c-ffi-interface-header-and-module-registration-cgame_ffih)
- [Color Data Structure and Utility Functions (color.c)](#color-data-structure-and-utility-functions-colorc)
- [Color Structure Definition and API (color.h)](#color-structure-definition-and-api-colorh)
- [Console System Implementation with Circular Buffer and Text Display (console.c)](#console-system-implementation-with-circular-buffer-and-text-display-consolec)
- [Console System Header - Entity-Based Debug Output Interface (console.h)](#console-system-header---entity-based-debug-output-interface-consoleh)
- [Debug Window Header Declaration (debugwin.h)](#debug-window-header-declaration-debugwinh)
- [Debug Window Initialization for macOS (debugwin.m)](#debug-window-initialization-for-macos-debugwinm)
- [Directory Path Configuration Header (dirs.h)](#directory-path-configuration-header-dirsh)
- [Edit System for Game Entity Editing and Visualization (edit.c)](#edit-system-for-game-entity-editing-and-visualization-editc)
- [Edit System Header - Entity Editor Interface (edit.h)](#edit-system-header---entity-editor-interface-edith)
- [Entity Management System - Core Entity Lifecycle and ID Management (entity.c)](#entity-management-system---core-entity-lifecycle-and-id-management-entityc)
- [Entity System Core Header - ID Management and Lifecycle (entity.h)](#entity-system-core-header---id-management-and-lifecycle-entityh)
- [EntityMap Dynamic Array Implementation (entitymap.c)](#entitymap-dynamic-array-implementation-entitymapc)
- [EntityMap Header - Hash Map for Entity to Integer Mapping (entitymap.h)](#entitymap-header---hash-map-for-entity-to-integer-mapping-entitymaph)
- [EntityPool Implementation - Entity-Indexed Object Pool (entitypool.c)](#entitypool-implementation---entity-indexed-object-pool-entitypoolc)
- [EntityPool - Memory Pool Management for Entity Components (entitypool.h)](#entitypool---memory-pool-management-for-entity-components-entitypoolh)
- [Error Handling Module with Formatted Error Reporting (error.c)](#error-handling-module-with-formatted-error-reporting-errorc)
- [Error Handling Header with Macro-Based Formatting (error.h)](#error-handling-header-with-macro-based-formatting-errorh)
- [Directory Operations Implementation (fs.c)](#directory-operations-implementation-fsc)
- [File System Directory Operations Header (fs.h)](#file-system-directory-operations-header-fsh)
- [Game Engine Main Loop and Window Management (game.c)](#game-engine-main-loop-and-window-management-gamec)
- [Game Engine Main Header - Core Window Management and Coordinate Systems (game.h)](#game-engine-main-header---core-window-management-and-coordinate-systems-gameh)
- [OpenGL Shader Compilation and Program Creation (gfx.c)](#opengl-shader-compilation-and-program-creation-gfxc)
- [OpenGL Graphics Utility Header (gfx.h)](#opengl-graphics-utility-header-gfxh)
- [OpenGL Library Header Integration (glew_glfw.h)](#opengl-library-header-integration-glew_glfwh)
- [GUI System Implementation - Core Components and UI Elements (gui.c)](#gui-system-implementation---core-components-and-ui-elements-guic)
- [GUI System Header - Entity-based User Interface Components (gui.h)](#gui-system-header---entity-based-user-interface-components-guih)
- [Input System Implementation with GLFW Integration (input.c)](#input-system-implementation-with-glfw-integration-inputc)
- [Input System Header - Keyboard and Mouse Input Handling (input.h)](#input-system-header---keyboard-and-mouse-input-handling-inputh)
- [Main Program Entry Point (main.c)](#main-program-entry-point-mainc)
- [3x3 Matrix Operations Implementation (mat3.c)](#3x3-matrix-operations-implementation-mat3c)
- [3x3 Matrix Data Structure and Operations Header (mat3.h)](#3x3-matrix-data-structure-and-operations-header-mat3h)
- [Physics System Implementation Using Chipmunk Physics Engine (physics.c)](#physics-system-implementation-using-chipmunk-physics-engine-physicsc)
- [Physics Engine Header - Core Physics System Interface (physics.h)](#physics-engine-header---core-physics-system-interface-physicsh)
- [Prefab System for Entity Serialization (prefab.c)](#prefab-system-for-entity-serialization-prefabc)
- [Prefab System Header - Entity Serialization and Deserialization (prefab.h)](#prefab-system-header---entity-serialization-and-deserialization-prefabh)
- [Save/Load System Implementation with Hierarchical Data Storage (saveload.c)](#saveload-system-implementation-with-hierarchical-data-storage-saveloadc)
- [Save/Load System Header - Data Persistence Interface (saveload.h)](#saveload-system-header---data-persistence-interface-saveloadh)
- [Scalar Type Definition and Mathematical Function Mappings (scalar.h)](#scalar-type-definition-and-mathematical-function-mappings-scalarh)
- [Scratch File Monitor and Executor (scratch.c)](#scratch-file-monitor-and-executor-scratchc)
- [Scratch Module Header Declaration (scratch.h)](#scratch-module-header-declaration-scratchh)
- [Lua Script Engine Integration and Event System (script.c)](#lua-script-engine-integration-and-event-system-scriptc)
- [Script System Interface Header (script.h)](#script-system-interface-header-scripth)
- [Script Export Header - Lua Integration and Export Macros (script_export.h)](#script-export-header---lua-integration-and-export-macros-script_exporth)
- [Sound System Implementation (sound.c)](#sound-system-implementation-soundc)
- [Sound System Entity Component Header (sound.h)](#sound-system-entity-component-header-soundh)
- [Sprite Component System Implementation (sprite.c)](#sprite-component-system-implementation-spritec)
- [Sprite System Header - 2D Rendering Component Interface (sprite.h)](#sprite-system-header---2d-rendering-component-interface-spriteh)
- [System Manager - Core Game Engine Initialization and Main Loop (system.c)](#system-manager---core-game-engine-initialization-and-main-loop-systemc)
- [System Module Header - Core System Management Interface (system.h)](#system-module-header---core-system-management-interface-systemh)
- [Test Directory - Keyboard Control System and Scene Setup (test)](#test-directory---keyboard-control-system-and-scene-setup-test)
- [OpenGL Texture Loading and Management System (texture.c)](#opengl-texture-loading-and-management-system-texturec)
- [Texture Management System Header (texture.h)](#texture-management-system-header-textureh)
- [Timing System Implementation (timing.c)](#timing-system-implementation-timingc)
- [Timing System Header - Game Time Management Interface (timing.h)](#timing-system-header---game-time-management-interface-timingh)
- [Transform Component System Implementation (transform.c)](#transform-component-system-implementation-transformc)
- [Transform Component System Header (transform.h)](#transform-component-system-header-transformh)
- [2D Vector Operations Implementation (vec2.c)](#2d-vector-operations-implementation-vec2c)
- [Vec2 Header - 2D Vector Structure and Operations (vec2.h)](#vec2-header---2d-vector-structure-and-operations-vec2h)

This directory contains the complete source code for a 2D game engine built with C and OpenGL, organized into modular components with Lua scripting integration. The engine follows an entity-component-system (ECS) architecture and provides comprehensive functionality for game development.

## Core Architecture Components

### Entity System (`entity.h`, `entity.c`)
- **Entity**: Core entity type with unique ID-based identification
- **entity_nil**: Special constant representing no entity
- **Key functions**: `entity_create()`, `entity_destroy()`, `entity_eq()`, `entity_destroyed()`
- **Save/load system**: Complete persistence with ID mapping and filtering capabilities
- **Two-pass destruction**: Entities remain in destroyed state for 2 update cycles before cleanup
- **ID reuse optimization**: Destroyed entity IDs are recycled to prevent unbounded growth

### Component Management Infrastructure
- **EntityPool** (`entitypool.h`, `entitypool.c`): Generic component storage with Entity-indexed access
- **EntityMap** (`entitymap.h`, `entitymap.c`): Hash map for Entity-to-integer mappings with dynamic resizing
- **Array** (`array.h`, `array.c`): Generic dynamic array with automatic memory management

### Transform System (`transform.h`, `transform.c`)
- **Transform**: Hierarchical 2D transformation component with position, rotation, scale
- **Parent-child relationships**: Complete hierarchy management with `transform_set_parent()`
- **Matrix caching**: Efficient world matrix computation with dirty tracking
- **Coordinate space conversion**: `transform_local_to_world()`, `transform_world_to_local()`
- **Recursive operations**: `transform_destroy_rec()`, `transform_set_save_filter_rec()`

## Rendering System

### Graphics Pipeline
- **Sprite System** (`sprite.h`, `sprite.c`): 2D sprite rendering with texture atlas support
  - **Sprite**: Component with size, texture coordinates, and depth sorting
  - **Atlas management**: `sprite_set_atlas()`, `sprite_get_atlas()`
  - **Depth sorting**: Integer-based layering system
- **Texture System** (`texture.h`, `texture.c`): OpenGL texture loading and management
  - **Hot-reloading**: Automatic texture updates when files change
  - **Caching**: Prevents duplicate texture loading
  - **Key functions**: `texture_load()`, `texture_bind()`, `texture_get_size()`
- **Graphics Utilities** (`gfx.h`, `gfx.c`): OpenGL shader compilation and vertex attribute binding
  - **gfx_create_program()**: Shader program creation from vertex/geometry/fragment shaders
  - **gfx_bind_vertex_attrib()**: Macro for binding vertex attributes to struct fields

### Camera System (`camera.h`, `camera.c`)
- **Camera**: Entity component for viewport management and coordinate transformations
- **Coordinate conversion**: Between world space, unit coordinates, and pixel coordinates
- **View matrix management**: Cached inverse view matrix for efficient rendering
- **Edit mode support**: Separate camera for editing interface

## User Interface System

### GUI Framework (`gui.h`, `gui.c`)
- **Hierarchical layout**: Parent-child relationships with alignment and padding
- **GuiAlign**: Alignment system (GA_MIN, GA_MID, GA_MAX, GA_TABLE, GA_NONE)
- **Event system**: Focus management, mouse/keyboard event handling
- **Specialized components**:
  - **GUI Rectangle**: Drawable rectangles with fit/fill layout behavior
  - **GUI Text**: Text rendering with cursor support and multi-line capabilities
  - **GUI TextEdit**: Text input fields with numerical input validation

### Console System (`console.h`, `console.c`)
- **Circular buffer**: Fixed-size line storage with automatic wrapping
- **Text wrapping**: Dynamic width calculation and continuation line marking
- **Dual output**: Both stdout and in-game display via GUI text entity
- **Key functions**: `console_puts()`, `console_printf()`

## Physics Integration

### Physics System (`physics.h`, `physics.c`)
- **Chipmunk Physics**: Complete wrapper around Chipmunk 2D physics engine
- **PhysicsBody**: Three body types (PB_STATIC, PB_DYNAMIC, PB_KINEMATIC)
- **Shape system**: Circle and polygon collision shapes with sensor support
- **Dynamics**: Force/impulse application, velocity limits, mass management
- **Collision detection**: Query system and spatial queries
- **Debug visualization**: OpenGL wireframe rendering of physics shapes

## Audio System

### Sound Management (`sound.h`, `sound.c`)
- **Sound**: Entity component for audio playback using Gorilla Audio library
- **File format support**: OGG (streaming) and WAV (direct) formats
- **Playback control**: Play/pause, seeking, looping, volume control
- **Auto-destruction**: Optional entity cleanup when audio finishes
- **Key functions**: `sound_set_path()`, `sound_set_playing()`, `sound_set_loop()`

## Input and Interaction

### Input System (`input.h`, `input.c`)
- **GLFW integration**: Wrapper around GLFW input handling
- **KeyCode/MouseCode**: Comprehensive enums for keyboard and mouse input
- **Callback system**: Event-driven input handling with callback registration
- **State queries**: `input_key_down()`, `input_mouse_down()`, `input_get_mouse_pos_pixels()`
- **Character handling**: Conversion between KeyCode and printable characters

### Edit System (`edit.h`, `edit.c`)
- **Entity editing**: Visual editing interface with bounding box visualization
- **Grid system**: Configurable grid rendering for alignment assistance
- **Line drawing**: Debug line rendering with `edit_line_add()`
- **Selection system**: Entity selection state management
- **Editability control**: Per-entity editability flags

## Scripting Integration

### Lua Integration (`script.h`, `script.c`)
- **FFI bindings**: Complete C-to-Lua integration using LuaJIT FFI
- **Event system**: Bidirectional event handling between C and Lua
- **Data exchange**: `_push_cdata()` for C-to-Lua data transfer
- **Script execution**: `script_run_string()`, `script_run_file()`
- **Error handling**: Enhanced error reporting with stack traces

### Script Export System (`script_export.h`, `cgame_ffi.h`)
- **SCRIPT macro**: Generates both C declarations and Lua FFI bindings
- **EXPORT macro**: Platform-specific function export handling
- **FFI registration**: Centralized registration of all exported functions

## Core Systems and Utilities

### Mathematical Foundation
- **Scalar** (`scalar.h`): Abstracted floating-point type with mathematical constants
- **Vec2** (`vec2.h`, `vec2.c`): 2D vector operations (arithmetic, geometric, rotation)
- **Mat3** (`mat3.h`, `mat3.c`): 3x3 matrix operations for 2D transformations
- **BBox** (`bbox.h`, `bbox.c`): Bounding box operations and transformations
- **Color** (`color.h`, `color.c`): RGBA color representation with predefined constants

### System Management
- **System** (`system.h`, `system.c`): Central system coordinator managing all subsystems
- **Game** (`game.h`, `game.c`): Main game loop, OpenGL initialization, and window management
- **Timing** (`timing.h`, `timing.c`): Delta time calculation with pause and scaling support
- **File System** (`fs.h`, `fs.c`): Cross-platform directory traversal operations

### Persistence Framework
- **Save/Load System** (`saveload.h`, `saveload.c`): Hierarchical data serialization
  - **Store**: Tree-based data structure for nested save data
  - **Type-specific serialization**: Functions for all primitive types
  - **Compression support**: Compact and human-readable formats
- **Prefab System** (`prefab.h`, `prefab.c`): Entity hierarchy serialization

### Development Tools
- **Error Handling** (`error.h`, `error.c`): Formatted error reporting with file/line information
- **Debug Window** (`debugwin.h`, `debugwin.c`): macOS-specific debug window positioning
- **Scratch System** (`scratch.h`, `scratch.c`): Hot-reloading Lua scratch file monitor
- **Console Integration**: Debug output system with both C and Lua interfaces

## Test and Example Code

### Test Framework (`test/`)
- **Keyboard Control** (`keyboard_controlled.h`, `keyboard_controlled.c`): Example movement system
- **Test Scenes** (`test.h`, `test.c`): Scene setup with camera, sprites, and entities

## Key Dependencies and Integration Points

### External Libraries
- **OpenGL/GLEW**: Graphics rendering and extension loading
- **GLFW**: Window management and input handling
- **Chipmunk Physics**: 2D physics simulation
- **Gorilla Audio**: Audio playback and mixing
- **LuaJIT**: Scripting engine integration
- **stb_image**: Image loading for textures

### Internal Architecture Patterns
- **Entity-Component-System**: Consistent ECS pattern across all game object systems
- **EntityPool pattern**: Standardized component storage and management
- **Save/load consistency**: Unified persistence across all systems
- **Script export pattern**: Consistent C-to-Lua binding generation
- **Hierarchical systems**: Parent-child relationships in transforms and GUI
- **Event-driven architecture**: Callback-based input and GUI event handling

The engine provides a complete foundation for 2D game development with modern architecture patterns, comprehensive tooling, and extensive scripting support while maintaining clear separation between C engine code and Lua game logic.

## Sections

### Dynamic Array Implementation with Generic Type Support (array.c)
This file implements a generic dynamic array data structure in C with automatic memory management and various utility functions.

## Core Data Structure

**`struct Array`** - The main array structure containing:
- `char *buf` - Buffer for storing elements (char* enables pointer arithmetic)
- `unsigned int capacity` - Allocated size of the buffer
- `unsigned int length` - Current number of elements
- `size_t object_size` - Size of each individual element

**`MIN_CAPACITY`** - Constant defining minimum array capacity (value: 2)

## Primary Functions

### Memory Management
- **`array_new_(size_t object_size)`** - Constructor that:
  - Allocates memory for Array struct
  - Sets initial capacity to MIN_CAPACITY
  - Allocates buffer with `object_size * capacity` bytes
  - Initializes length to 0
- **`array_free(Array *arr)`** - Destructor that frees both buffer and struct memory

### Element Access
- **`array_get(Array *arr, unsigned int i)`** - Returns pointer to element at index i
- **`array_top(Array *arr)`** - Returns pointer to last element
- **`array_length(Array *arr)`** - Returns current number of elements

### Iterator Support
- **`array_begin(Array *arr)`** - Returns pointer to first element
- **`array_end(Array *arr)`** - Returns pointer past the last element

### Dynamic Operations
- **`array_add(Array *arr)`** - Adds new element with automatic resizing:
  - Increments length
  - Doubles capacity if needed using `realloc`
  - Returns pointer to newly added element location

- **`array_reset(Array *arr, unsigned int num)`** - Resets array to specific size:
  - Frees existing buffer
  - Sets new length and capacity (minimum MIN_CAPACITY)
  - Allocates new buffer

- **`array_pop(Array *arr)`** - Removes last element with automatic downsizing:
  - Decrements length
  - Halves capacity if usage drops below 1/4 and capacity > MIN_CAPACITY

### Advanced Operations
- **`array_quick_remove(Array *arr, unsigned int i)`** - Efficient removal:
  - Copies last element to position i (if not removing last element)
  - Calls `array_pop()` to handle length/capacity
  - Returns boolean indicating if element was swapped

- **`array_sort(Array *arr, int (*compar)(const void *, const void *))`** - Sorts array using `qsort`

## Test Code (when ARRAY_TEST is defined)

### Test Data Structure
**`IntPair`** - Test struct with two integers (a, b)

### Test Functions
- **`dump(Array *arr)`** - Prints array state and contents
- **`int_compare(const void *a, const void *b)`** - Comparison function for integer sorting
- **`test_sort()`** - Demonstrates sorting functionality with integer array
- **`main()`** - Main test driver demonstrating:
  - Array creation and population
  - Element removal with `array_quick_remove`
  - Memory management verification
  - Sorting functionality

## Key Dependencies
- References **`array.h`** header file (likely contains macros like `array_new`, `array_add_val`, `array_foreach`)
- Uses standard library functions: `malloc`, `free`, `realloc`, `memcpy`, `qsort`
- Test code uses `printf` for output

## Memory Management Strategy
The implementation uses a doubling/halving strategy for dynamic resizing:
- **Growth**: Doubles capacity when length exceeds capacity
- **Shrinkage**: Halves capacity when usage drops below 25% of capacity (with minimum threshold)

### Dynamic Array Header - Generic Resizable Array Implementation (array.h)
This header file defines a generic dynamic array data structure that stores objects contiguously in memory with automatic resizing capabilities.

## Core Data Structure
- **`Array`**: Opaque struct type representing the dynamic array container
- **Important constraint**: Objects may move in memory during operations, so pointers to array elements should not be retained

## Memory Management Functions
- **`array_new_(size_t object_size)`**: Constructor function that takes element size in bytes
- **`array_new(type)`**: Macro wrapper that automatically calculates `sizeof(type)` for convenience
- **`array_free(Array *arr)`**: Destructor to deallocate array memory

## Element Access Functions
- **`array_get(Array *arr, unsigned int i)`**: Returns pointer to element at index `i`
- **`array_get_val(type, arr, i)`**: Macro that dereferences the pointer to return the actual value
- **`array_top(Array *arr)`**: Returns pointer to the last element
- **`array_top_val(type, arr)`**: Macro that dereferences to return the last element's value
- **`array_length(Array *arr)`**: Returns the number of elements currently stored

## Iterator Support
- **`array_begin(Array *arr)`**: Returns pointer to first element
- **`array_end(Array *arr)`**: Returns pointer to one-past-last element (standard iterator pattern)
- **Warning**: These pointers may become invalid after add/remove operations

## Modification Functions
- **`array_add(Array *arr)`**: Adds new uninitialized element, returns pointer to it (index becomes `length - 1`)
- **`array_add_val(type, arr)`**: Macro that returns reference to newly added element for direct assignment
- **`array_reset(Array *arr, unsigned int num)`**: Resizes array to exactly `num` elements with undefined data
- **`array_clear(arr)`**: Macro that resets array to zero elements
- **`array_pop(Array *arr)`**: Removes the last element
- **`array_quick_remove(Array *arr, unsigned int i)`**: Efficient removal that may swap another element into position `i`
  - Returns `true` if a swap occurred, `false` otherwise

## Utility Functions
- **`array_sort(Array *arr, int (*compar)(const void *, const void *))`**: Sorts array using provided comparator function compatible with `qsort(3)`

## Iteration Macro
- **`array_foreach(var, arr)`**: Provides clean iteration syntax
  - `var` must be a pre-declared pointer variable of appropriate element type
  - Visits elements in index order
  - **Warning**: Do not add/remove elements during iteration

## Dependencies
- **`<stddef.h>`**: For `size_t` type
- **`<stdbool.h>`**: For `bool` type and `true`/`false` values

## Usage Pattern
The design follows a generic container pattern where:
1. Arrays store elements of uniform size specified at creation
2. Automatic memory management handles resizing
3. Type-safe macros provide convenient access while maintaining generic implementation
4. Iterator support enables standard C-style iteration patterns

### Bounding Box Operations Implementation (bbox.c)
This section implements core bounding box operations for 2D geometric calculations. A bounding box (BBox) represents a rectangular region defined by minimum and maximum corner points.

## Functions Defined

• **`bbox_merge(BBox a, BBox b)`**
  - Merges two bounding boxes into a single bounding box that encompasses both
  - Returns a new BBox with:
    - min corner: component-wise minimum of both input min corners
    - max corner: component-wise maximum of both input max corners
  - Uses `scalar_min()` and `scalar_max()` utility functions

• **`bbox_bound(Vec2 a, Vec2 b)`**
  - Creates a bounding box from two arbitrary points
  - Returns a new BBox with:
    - min corner: component-wise minimum of the two input points
    - max corner: component-wise maximum of the two input points
  - Ensures proper min/max ordering regardless of input point order

• **`bbox_contains(BBox b, Vec2 p)`**
  - Tests if a point lies within a bounding box (inclusive bounds)
  - Returns `true` if point p is within bounds: `min.x ≤ p.x ≤ max.x` AND `min.y ≤ p.y ≤ max.y`
  - Uses inclusive comparison on all boundaries

• **`bbox(Vec2 min, Vec2 max)`**
  - Constructor function for creating a BBox from min/max corners
  - Directly assigns the provided min and max vectors to the BBox structure
  - No validation of min/max ordering

• **`bbox_transform(Mat3 m, BBox b)`**
  - Applies a 3x3 transformation matrix to a bounding box
  - Algorithm steps:
    1. Transform all four corners of the original bounding box using `mat3_transform()`
    2. Create two intermediate bounding boxes from the transformed corners
    3. Merge the intermediate boxes to get the final transformed bounding box
  - Handles arbitrary transformations (rotation, scaling, translation, etc.)

## Key Dependencies

• **Data Types**: `BBox`, `Vec2`, `Mat3`
• **Vector Operations**: `vec2()` constructor
• **Scalar Operations**: `scalar_min()`, `scalar_max()`
• **Matrix Operations**: `mat3_transform()`
• **Header**: `bbox.h` (included)

The implementation provides a complete set of fundamental bounding box operations commonly needed in 2D graphics, collision detection, and spatial algorithms.

### Bounding Box Data Structure and Operations Header (bbox.h)
This header file defines a 2D bounding box data structure and associated operations for spatial calculations and transformations.

## Core Data Structure
- **BBox**: A structure representing a 2D axis-aligned bounding box
  - Contains two `Vec2` fields: `min` and `max`
  - Represents the minimum and maximum coordinates of the bounding box

## Dependencies
- **vec2.h**: Provides the `Vec2` type used for the min/max coordinates
- **mat3.h**: Provides the `Mat3` type used for transformations
- **script_export.h**: Provides the `SCRIPT` and `EXPORT` macros for code generation/binding

## Exported Functions
- **bbox(Vec2 min, Vec2 max)**: Constructor function that creates a BBox from minimum and maximum coordinate vectors
- **bbox_bound(Vec2 a, Vec2 b)**: Creates a bounding box that encompasses two points
- **bbox_merge(BBox a, BBox b)**: Combines two bounding boxes into a single bounding box that contains both
- **bbox_contains(BBox b, Vec2 p)**: Returns a boolean indicating whether a point is contained within the bounding box
- **bbox_transform(Mat3 m, BBox b)**: Applies a 3x3 transformation matrix to a bounding box and returns the resulting bounding box that encompasses the transformed box

## Code Organization Features
- Uses header guards (`#ifndef BBOX_H`, `#define BBOX_H`) to prevent multiple inclusions
- All definitions are wrapped in a `SCRIPT` macro block, suggesting this code is designed to be exported to a scripting system
- All function declarations use the `EXPORT` macro, indicating they are part of a public API

This header provides a complete interface for 2D bounding box operations commonly needed in graphics, collision detection, and spatial partitioning systems.

### Camera System Implementation (camera.c)
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

### Camera System Interface and Management (camera.h)
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

### Lua-C FFI Interface Header and Module Registration (cgame_ffi.h)
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

### Color Data Structure and Utility Functions (color.c)
This section implements a color system with predefined color constants, serialization functions, and color creation utilities.

## Predefined Color Constants
The file defines seven global `Color` constants with RGBA values:
- `color_black`: { 0.0, 0.0, 0.0, 1.0 }
- `color_white`: { 1.0, 1.0, 1.0, 1.0 }
- `color_gray`: { 0.5, 0.5, 0.5, 1.0 }
- `color_red`: { 1.0, 0.0, 0.0, 1.0 }
- `color_green`: { 0.0, 1.0, 0.0, 1.0 }
- `color_blue`: { 0.0, 0.0, 1.0, 1.0 }
- `color_clear`: { 0.0, 0.0, 0.0, 0.0 } (transparent black)

## Serialization Functions
Two functions handle saving and loading colors to/from a storage system:

### `color_save` Function
- **Purpose**: Saves a color to storage
- **Parameters**: 
  - `Color *c`: pointer to color to save
  - `const char *n`: name/key for storage
  - `Store *s`: storage object
- **Logic**:
  - Creates a child store using `store_child_save`
  - If successful, saves each RGBA component separately using `scalar_save`
  - Components saved as "r", "g", "b", "a"

### `color_load` Function
- **Purpose**: Loads a color from storage with fallback default
- **Parameters**:
  - `Color *c`: pointer to color to load into
  - `const char *n`: name/key for storage lookup
  - `Color d`: default color if load fails
  - `Store *s`: storage object
- **Logic**:
  - Attempts to load child store using `store_child_load`
  - If successful, loads each RGBA component using `scalar_load` with 0 as default
  - If unsuccessful, assigns the provided default color `d`
  - Returns boolean indicating success/failure

## Color Creation Functions
Two utility functions create `Color` instances:

### `color_opaque` Function
- **Purpose**: Creates an opaque color (alpha = 1.0)
- **Parameters**: `Scalar r`, `Scalar g`, `Scalar b`
- **Implementation**: Calls `color(r, g, b, 1)` internally
- **Note**: Uses `#undef color_opaque`, suggesting it may be defined as a macro elsewhere

### `color` Function
- **Purpose**: Creates a color with specified RGBA values
- **Parameters**: `Scalar r`, `Scalar g`, `Scalar b`, `Scalar a`
- **Implementation**: Returns a `Color` struct initialized with the four values
- **Note**: Uses `#undef color`, suggesting it may be defined as a macro elsewhere

## Key Dependencies
- `color.h`: Header file defining the `Color` structure and related types
- `Store`: Storage system for serialization (defined elsewhere)
- `Scalar`: Numeric type for color components (likely float or double)
- `scalar_save`/`scalar_load`: Functions for serializing scalar values
- `store_child_save`/`store_child_load`: Functions for hierarchical storage operations

### Color Structure Definition and API (color.h)
This header file defines a color system with RGBA components and provides a comprehensive API for color manipulation and persistence.

## Core Data Structure
- **Color**: A struct containing four `Scalar` components:
  - `r` (red)
  - `g` (green) 
  - `b` (blue)
  - `a` (alpha/transparency)

## Dependencies
- `scalar.h`: Provides the `Scalar` type used for color components
- `script_export.h`: Enables the `SCRIPT` and `EXPORT` macros for scripting integration
- `saveload.h`: Provides the `Store` type and persistence functionality

## Constructor Functions
- **color()**: Creates a color with specified RGBA values
- **color_opaque()**: Creates a color with specified RGB values and full opacity (alpha = 1)

## Predefined Color Constants
The file exports several standard color constants:
- `color_black`
- `color_white` 
- `color_gray`
- `color_red`
- `color_green`
- `color_blue`
- `color_clear` (transparent color with zero alpha)

## Persistence API
- **color_save()**: Saves a color to a store with a given name
  - Parameters: `Color *c`, `const char *n`, `Store *s`
- **color_load()**: Loads a color from a store with a default fallback
  - Parameters: `Color *c`, `const char *n`, `Color d`, `Store *s`
  - Returns: `bool` indicating success/failure

## Macro Definitions
- **color()**: Macro for convenient color creation using struct literal syntax
- **color_opaque()**: Macro that creates an opaque color by calling the main color macro with alpha = 1

## Integration Features
- The entire API is wrapped in a `SCRIPT()` macro, indicating this module is designed to be accessible from a scripting system
- All public functions and constants are marked with `EXPORT` for external visibility
- Standard header guard pattern (`#ifndef COLOR_H`) prevents multiple inclusion

### Console System Implementation with Circular Buffer and Text Display (console.c)
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

### Console System Header - Entity-Based Debug Output Interface (console.h)
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

### Debug Window Header Declaration (debugwin.h)
This is a header file that declares a utility function for window management during debugging operations.

## Header Guard
- Uses standard include guard pattern with `DEBUGWIN_H` macro to prevent multiple inclusions

## Function Declaration
- **`debugwin_init()`**: A void function that configures window positioning and properties
  - Sets window to stay on top of other windows
  - Moves window to upper-left position
  - Designed specifically for debugging workflows and scratch buffer editing convenience
  - Takes no parameters and returns no value

## Purpose and Context
- This appears to be a debugging utility focused on window management
- The function is intended to make debugging more convenient by:
  - Ensuring the debug window remains visible above other applications
  - Positioning it in a consistent, accessible location (upper-left corner)
  - Facilitating scratch buffer editing workflows during debugging sessions

## Dependencies
- No external dependencies or references to other code entities are visible in this header
- The actual implementation would likely depend on platform-specific windowing system APIs

This header provides a simple interface for developers who need consistent window positioning during debugging tasks, particularly when working with temporary buffers or debugging output.

### Debug Window Initialization for macOS (debugwin.m)
This section implements platform-specific window configuration for a debug window on macOS using GLFW and Cocoa APIs.

## Function Definitions

**`debugwin_init()`** - Configures a debug window with specific display properties on macOS

## Dependencies and Includes

• **`glew_glfw.h`** - OpenGL Extension Wrangler and GLFW integration
• **`<GLFW/glfw3native.h>`** - GLFW native platform access functions
  - Enabled with `GLFW_EXPOSE_NATIVE_COCOA` and `GLFW_EXPOSE_NATIVE_NSGL` macros
• **`game.h`** - References `game_window` variable

## Implementation Details

The `debugwin_init()` function performs two main window configuration tasks:

• **Always-on-top behavior:**
  - Retrieves the native Cocoa window handle using `glfwGetCocoaWindow(game_window)`
  - Sets the window level to `NSFloatingWindowLevel` to keep it above other windows
  - Uses Objective-C syntax to call `setLevel:` method on the window object

• **Window positioning:**
  - Sets the window position to coordinates (9, 53) using `glfwSetWindowPos()`
  - This places the window in the top-left area of the screen

## Key References

• **`game_window`** - External window handle variable defined in `game.h`
• **`glfwGetCocoaWindow()`** - GLFW function to get native Cocoa window handle
• **`glfwSetWindowPos()`** - GLFW function for window positioning
• **`NSFloatingWindowLevel`** - macOS Cocoa constant for floating window level

This code is specifically designed for macOS development, using Cocoa's native window management features to create a debug window that stays visible above other application windows.

### Directory Path Configuration Header (dirs.h)
This header file defines default directory paths and utility macros for constructing file paths within the application.

**Defined Entities:**

• **DATA_DIR** - Preprocessor macro defining the default data directory path
  - Set to `"./data/"` if not already defined elsewhere
  - Uses conditional compilation (`#ifndef`) to allow override at compile time

• **USR_DIR** - Preprocessor macro defining the default user directory path
  - Set to `"./usr/"` if not already defined elsewhere
  - Uses conditional compilation (`#ifndef`) to allow override at compile time

• **data_path(path)** - Function-like macro for constructing data file paths
  - Takes a `path` parameter and concatenates it with `DATA_DIR`
  - Example usage: `data_path("config.txt")` would expand to `"./data/config.txt"`

• **usr_path(path)** - Function-like macro for constructing user file paths
  - Takes a `path` parameter and concatenates it with `USR_DIR`
  - Example usage: `usr_path("settings.ini")` would expand to `"./usr/settings.ini"`

**Key Features:**

• **Include Guard** - Uses `#ifndef DIRS_H` / `#define DIRS_H` / `#endif` pattern to prevent multiple inclusions

• **Configurable Defaults** - Both directory macros can be overridden at compile time by defining them before including this header

• **Path Construction Utilities** - Provides convenient macros to build full file paths by combining base directories with relative paths

This header serves as a centralized location for managing application directory structure and provides a consistent interface for file path construction throughout the codebase.

### Edit System for Game Entity Editing and Visualization (edit.c)
This file implements a comprehensive editing system for a game engine that provides entity editability management, bounding box visualization, grid rendering, and line drawing capabilities. The system is designed to support in-game editing workflows with visual feedback.

## Core Edit System Management

**Key Functions:**
- `edit_set_enabled(bool e)` / `edit_get_enabled()` - Controls whether the edit system is active
- `edit_set_editable(Entity ent, bool editable)` / `edit_get_editable(Entity ent)` - Manages per-entity editability
- `edit_clear()` - Clears all editing data
- `edit_init()` / `edit_deinit()` - System initialization and cleanup
- `edit_update_all()` / `edit_draw_all()` - Main update and rendering loops
- `edit_save_all(Store *s)` / `edit_load_all(Store *s)` - Persistence functionality

**Data Structures:**
- `uneditable_pool` - EntityPool tracking non-editable entities (inverse storage for efficiency)

## Bounding Box Visualization System

**Core Structure:**
- `BBoxPoolElem` - Contains world matrix (`Mat3 wmat`), bounding box (`BBox bbox`), and selection state (`Scalar selected`)
- `bbox_pool` - EntityPool managing all entity bounding boxes

**Key Functions:**
- `edit_bboxes_update(Entity ent, BBox bbox)` - Updates/merges bounding boxes for editable entities
- `edit_bboxes_has(Entity ent)` / `edit_bboxes_get(Entity ent)` - Bounding box queries
- `edit_bboxes_get_num()` / `edit_bboxes_get_nth(unsigned int n)` - Collection access returning `EntityBBoxPair`
- `edit_bboxes_set_selected(Entity ent, bool selected)` - Selection state management

**Rendering Pipeline:**
- `_bboxes_init()` - Creates OpenGL resources:
  - Shader program from "bbox.vert", "bbox.geom", "bbox.frag"
  - VAO (`bboxes_vao`) and VBO (`bboxes_vbo`)
  - Vertex attributes: world matrix (3x3), bbox min/max, selection state
- `_bboxes_update_all()` - Updates world matrices and applies default bbox (0.5x0.5) for invalid boxes
- `_bboxes_draw_all()` - Renders using point primitives with geometry shader expansion

## Grid System

**Configuration:**
- `grid_size` - Vec2 controlling grid cell dimensions (0 = auto-size)
- `edit_set_grid_size(Vec2 size)` / `edit_get_grid_size()` - Grid size management

**Rendering Process:**
- `_grid_create_cells()` - Dynamic grid generation algorithm:
  1. Calculates camera bounds in world space
  2. Determines cell size (auto-sizing if grid_size is 0)
  3. Prevents excessive subdivision (max 70 cells per dimension)
  4. Snaps to grid boundaries
  5. Generates cell bounding boxes with transform matrices
- `_grid_draw()` - Renders grid using same shader as bounding boxes with `is_grid` uniform set to 1

## Line Drawing System

**Structure:**
- `LinePoint` - Contains position (`Vec2`), point size (`Scalar`), and color (`Color`)
- `line_points` - Array storing line endpoints (consecutive pairs form lines)

**Key Functions:**
- `edit_line_add(Vec2 a, Vec2 b, Scalar point_size, Color color)` - Adds a line segment

**Rendering:**
- Uses separate shader program from "edit_line.vert" and "edit_line.frag"
- Draws both GL_LINES and GL_POINTS for line segments with endpoints
- `_line_draw_all()` - Renders all accumulated line data

## Dependencies and Integration

**External Dependencies:**
- Graphics system (`gfx.h`) - Shader management and vertex attribute binding
- Entity system (`entitymap.h`, `entitypool.h`) - Entity management
- Transform system (`transform.h`) - World matrix calculations
- Camera system (`camera.h`) - View transformations
- Math utilities (`mat3.h`, `Vec2`, `BBox`) - Mathematical operations
- Game framework (`game.h`) - Window size queries
- Data management (`Store`) - Serialization system

**Shader Integration:**
- Passes `inverse_view_matrix` and `aspect` ratio uniforms to all shaders
- Uses `is_grid` uniform to distinguish between bbox and grid rendering modes
- Supports instanced rendering through vertex attributes

The system provides a complete editing interface with visual feedback, supporting both immediate-mode drawing (lines) and persistent entity visualization (bounding boxes) with grid alignment assistance.

### Edit System Header - Entity Editor Interface (edit.h)
This header file defines the interface for an entity editing system that provides interactive editing capabilities for game entities. The system includes both script-exported functions and internal C functions.

## Script-Exported Functions (SCRIPT macro)

The following functions are exported to scripting languages:

### Core Edit System Control
- `edit_set_enabled(bool e)` - Enables/disables the edit system
- `edit_get_enabled()` - Returns current enabled state

### Entity Editability Management
- `edit_set_editable(Entity ent, bool editable)` - Sets whether an entity can be edited (non-editable entities cannot be selected)
- `edit_get_editable(Entity ent)` - Returns editability status of an entity

### Grid System
- `edit_set_grid_size(Vec2 size)` - Sets grid size for snapping (non-negative values, zero means no grid)
- `edit_get_grid_size()` - Returns current grid size

### Bounding Box Management
Used for click selection and entity interaction:
- `edit_bboxes_update(Entity ent, BBox bbox)` - Merges a bounding box with an entity's existing bbox
- `edit_bboxes_has(Entity ent)` - Checks if entity has a bounding box
- `edit_bboxes_get(Entity ent)` - Retrieves entity's bounding box
- `edit_bboxes_get_num()` - Returns total number of bounding boxes
- `edit_bboxes_get_nth(unsigned int n)` - Gets nth entity-bbox pair
- `edit_bboxes_set_selected(Entity ent, bool selected)` - Sets selection state of an entity

### Visual Debug Tools
- `edit_line_add(Vec2 a, Vec2 b, Scalar point_size, Color color)` - Draws a line between two world-space coordinates

## Data Structures

- `EntityBBoxPair` struct - Contains an Entity and its associated BBox for pairing entity references with their bounding boxes

## Internal C Functions

System lifecycle and management functions (not exported to scripts):
- `edit_clear()` - Clears edit system state
- `edit_init()` - Initializes the edit system
- `edit_deinit()` - Cleans up the edit system
- `edit_update_all()` - Updates all edit system components
- `edit_draw_all()` - Renders all edit system visuals
- `edit_save_all(Store *s)` - Saves edit system state to storage
- `edit_load_all(Store *s)` - Loads edit system state from storage

## Dependencies

The system depends on several external modules:
- `entity.h` - For Entity type definitions
- `bbox.h` - For BBox (bounding box) functionality
- `color.h` - For Color type used in visual elements
- `script_export.h` - For the SCRIPT macro that exports functions to scripting languages

This edit system appears to be designed for interactive level editing or entity manipulation within a game engine, providing both programmatic and visual tools for entity management.

### Entity Management System - Core Entity Lifecycle and ID Management (entity.c)
This section implements a comprehensive entity management system that handles the complete lifecycle of entities from creation through destruction, including save/load functionality and ID reuse optimization.

## Core Data Structures

- **Entity**: Base entity type with an `id` field
- **DestroyEntry**: Structure containing an `Entity ent` and `unsigned int pass` for tracking destruction phases
- **ExistsPoolElem**: Wrapper structure containing `EntityPoolElem pool_elem` for the entity pool
- **SaveFilter**: Enum with values `SF_SAVE`, `SF_NO_SAVE`, `SF_UNSET` for controlling save behavior

## Global State Management

- **entity_nil**: Global null entity constant with ID 0
- **counter**: Static counter starting at 1 for generating new entity IDs
- **exists_pool**: EntityPool tracking all currently existing entities
- **destroyed_map**: EntityMap tracking which entities are marked for destruction
- **destroyed**: Array of DestroyEntry objects for entities in destruction process
- **unused_map**: EntityMap tracking entities available for ID reuse
- **unused**: Array of Entity IDs that can be reused after removal
- **load_map**: EntityMap for mapping saved IDs to real IDs during loading
- **save_filter_map**: EntityMap controlling which entities should be saved
- **save_filter_default**: Default save filter behavior (initially SF_SAVE)

## Entity Lifecycle Management

The system implements a three-state lifecycle with delayed destruction:

1. **Doesn't exist/removed state**:
   - `destroyed_map[ent] = false`
   - Entity not in destroyed array
   - Transitions to "exists" on `entity_create()`

2. **Exists state**:
   - `destroyed_map[ent] = false` 
   - Entity not in destroyed array
   - Transitions to "destroyed" on `entity_destroy()`

3. **Destroyed state**:
   - `destroyed_map[ent] = true`
   - Entity entry `{ent, pass}` added to destroyed array
   - After 2 update passes, `_remove()` called to return to state 1

## Key Functions

### Entity Creation and Destruction
- **entity_create()**: Creates new entity by calling `_generate_id()`
- **_generate_id()**: 
  - Reuses unused IDs from unused array if available
  - Otherwise increments counter for new ID
  - Adds entity to exists_pool
- **entity_destroy()**: Marks entity for destruction with 2-pass delay
- **entity_destroy_all()**: Destroys all entities in exists_pool
- **_remove()**: Actually removes entity and moves ID to unused list for reuse

### Save/Load System
- **entity_set_save_filter()**: Controls whether specific entities are saved
- **entity_get_save_filter()**: Checks if entity should be saved
- **entity_clear_save_filters()**: Resets all save filters to default
- **entity_save()**: Serializes entity to Store, validates save filter
- **entity_load()**: Deserializes entity from Store
- **_entity_resolve_saved_id()**: Maps saved IDs to real IDs during loading
- **entity_load_all_begin()**: Initializes load_map for loading session
- **entity_load_all_end()**: Cleans up load_map and save filters

### System Management
- **entity_init()**: Initializes all global data structures
- **entity_deinit()**: Frees all allocated memory
- **entity_update_all()**: Processes destroyed entities, implementing 2-pass removal
- **entity_eq()**: Compares two entities for equality by ID
- **entity_destroyed()**: Checks if entity is marked for destruction

### Bulk Save/Load Operations
- **entity_save_all()**: Saves complete entity system state including exists_pool and destroyed array
- **entity_load_all()**: Loads complete entity system state from saved data

## Dependencies
The system depends on several external modules:
- **saveload.h**: For Store-based serialization
- **entitymap.h**: For EntityMap data structure
- **array.h**: For dynamic Array implementation  
- **entitypool.h**: For EntityPool management
- **error.h**: For error handling and assertions

## Key Algorithms

**Two-Pass Destruction**: Entities marked for destruction remain in the destroyed array for 2 update cycles before actual removal, allowing other systems to clean up references.

**ID Reuse Optimization**: Destroyed entity IDs are recycled through the unused array to prevent unbounded ID growth.

**Save/Load ID Mapping**: During loading, a temporary mapping translates saved IDs to current runtime IDs, allowing saved entity references to be properly restored.

### Entity System Core Header - ID Management and Lifecycle (entity.h)
This header file defines the core entity system for managing unique entity identifiers and their lifecycle. It provides a component-based architecture foundation where entities are lightweight ID containers.

## Core Data Structures

- **`Entity`**: A struct containing a single `unsigned int id` field representing a unique entity identifier
- **`entity_nil`**: A special Entity value that represents no valid entity (exported constant)

## Entity Lifecycle Management

- **`entity_create()`**: Allocates and returns a new unused Entity ID
- **`entity_destroy(Entity ent)`**: Releases a specific Entity ID for reuse
- **`entity_destroy_all()`**: Destroys all existing entities
- **`entity_destroyed(Entity ent)`**: Checks if an entity has been destroyed

## Entity Operations

- **`entity_eq(Entity e, Entity f)`**: Compares two entities for equality
  - Implemented as a C macro: `((e).id == (f).id)`

## Save/Load System Integration

The entity system integrates with a save/load system through multiple mechanisms:

### Save Filtering
- **`entity_set_save_filter(Entity ent, bool filter)`**: Marks entities for selective saving
- **`entity_get_save_filter(Entity ent)`**: Retrieves save filter status
- **`entity_clear_save_filters()`**: Removes all save filters
- **Save filtering logic**: When any entity has save filter set to true, only entities with true filters are saved; entities with false filters are excluded

### Serialization Functions
- **`entity_save(Entity *ent, const char *n, Store *s)`**: Saves entity ID data
- **`entity_load(Entity *ent, const char *n, Entity d, Store *s)`**: Loads entity ID data with default fallback
- **`_entity_resolve_saved_id(unsigned int id)`**: Internal function for resolving saved IDs during merge operations

### Bulk Operations
- **`entity_save_all(Store *s)`**: Saves all entities to storage
- **`entity_load_all(Store *s)`**: Loads all entities from storage
- **`entity_load_all_begin()`**: Initiates bulk loading process
- **`entity_load_all_end()`**: Finalizes bulk loading process

## System Management

- **`entity_init()`**: Initializes the entity system
- **`entity_deinit()`**: Cleans up the entity system
- **`entity_update_all()`**: Updates all entities (likely calls update logic for entity components)

## Dependencies

- **`saveload.h`**: Provides `Store` type for persistence operations
- **`script_export.h`**: Provides `SCRIPT()` and `EXPORT` macros for scripting language integration
- **`<stdbool.h>`**: Standard boolean type support

## Script Integration

The entire public API is wrapped in a `SCRIPT(entity, ...)` macro, indicating this system is exposed to a scripting language with all exported functions available for script access.

### EntityMap Dynamic Array Implementation (entitymap.c)
This section implements a dynamic array data structure called `EntityMap` that maps entity IDs to integer values with automatic resizing capabilities.

## Data Structure Definition

• **EntityMap struct** contains:
  - `int *arr`: Dynamic array storing the actual values
  - `unsigned int bound`: One plus the maximum key currently in use
  - `unsigned int capacity`: Total allocated space in the array
  - `int def`: Default value returned for unset keys
  - Maintains invariants: `bound <= capacity` and `MIN_CAPACITY <= capacity`

• **MIN_CAPACITY** constant set to 2 (minimum array size)

## Core Functions

• **entitymap_new(int def)**:
  - Allocates new EntityMap with specified default value
  - Calls internal `_init()` to set up initial state
  - Returns pointer to new EntityMap

• **entitymap_clear(EntityMap *emap)**:
  - Frees current array and reinitializes to empty state
  - Preserves the default value

• **entitymap_free(EntityMap *emap)**:
  - Deallocates both the array and the EntityMap struct itself

## Memory Management Functions

• **_init(EntityMap *emap)** (internal):
  - Sets bound to 0, capacity to MIN_CAPACITY
  - Allocates initial array and fills with default values

• **_grow(EntityMap *emap)** (internal):
  - Finds next power of 2 that accommodates the current bound
  - Reallocates array to new capacity
  - Initializes new elements with default value

• **_shrink(EntityMap *emap)** (internal):
  - Shrinks capacity when bound is less than 1/4 of current capacity
  - Halves capacity while maintaining minimum size constraint
  - Only shrinks if capacity exceeds MIN_CAPACITY

## Access Functions

• **entitymap_set(EntityMap *emap, Entity ent, int val)**:
  - If setting to default value (deletion):
    - Updates array at entity ID
    - Potentially shrinks bound by scanning backwards for non-default values
    - Calls `_shrink()` if bound changed
  - If setting to non-default value:
    - Expands bound if necessary
    - Calls `_grow()` if entity ID exceeds current capacity
    - Updates array at entity ID

• **entitymap_get(EntityMap *emap, Entity ent)**:
  - Returns default value if entity ID is beyond current capacity
  - Otherwise returns value from array at entity ID

## Dependencies

• References **Entity** type (defined elsewhere, uses `.id` field)
• Uses standard library functions: `malloc`, `realloc`, `free`
• Includes "entitymap.h" header file

## Key Algorithms

• **Dynamic resizing**: Grows by powers of 2 when needed, shrinks when utilization drops below 25%
• **Bound management**: Tracks the highest used index plus one to optimize memory usage
• **Lazy deletion**: Setting values to default is treated as deletion, triggering potential shrinking

### EntityMap Header - Hash Map for Entity to Integer Mapping (entitymap.h)
This header file defines an interface for a hash map data structure that maps `Entity` objects to integer values.

## Data Structure Definition
- **EntityMap**: A typedef for `struct EntityMap` that represents the main hash map data structure
  - The actual struct definition is not shown (likely in the corresponding .c file)
  - Maps Entity keys to integer values
  - Supports a default value for unset/missing keys

## Dependencies
- **entity.h**: Required include that defines the `Entity` type used as keys in the map

## Function Interface
The header declares the following functions for EntityMap operations:

### Memory Management
- **entitymap_new(int def)**: Constructor function
  - Creates a new EntityMap instance
  - Takes a default integer value (`def`) that will be returned for unset keys
  - Returns a pointer to the new EntityMap
- **entitymap_free(EntityMap *emap)**: Destructor function
  - Frees all memory associated with the EntityMap

### Data Operations
- **entitymap_clear(EntityMap *emap)**: Clears all entries from the map
  - Removes all key-value pairs but keeps the map structure intact
- **entitymap_set(EntityMap *emap, Entity ent, int val)**: Sets a key-value pair
  - Maps the Entity `ent` to the integer value `val`
- **entitymap_get(EntityMap *emap, Entity ent)**: Retrieves a value by key
  - Returns the integer value associated with Entity `ent`
  - Returns the default value if the key is not found

## Design Notes
- The implementation uses opaque data structure pattern (struct definition hidden)
- Supports default values for missing keys, eliminating need for explicit "key not found" error handling
- All functions take EntityMap pointer as first parameter (object-oriented style in C)

### EntityPool Implementation - Entity-Indexed Object Pool (entitypool.c)
This file implements an `EntityPool` data structure that provides efficient storage and retrieval of objects indexed by Entity IDs. The pool combines an EntityMap for fast lookups with an Array for contiguous storage.

## Core Data Structure

• **EntityPool struct** contains:
  - `emap`: EntityMap pointer that maps Entity IDs to array indices (-1 if doesn't exist)
  - `array`: Array pointer for contiguous object storage

## Memory Management Functions

• **entitypool_new_()**: Creates new EntityPool with specified object size
  - Allocates EntityPool struct
  - Initializes EntityMap with -1 as default value
  - Creates new Array with given object size

• **entitypool_free()**: Deallocates EntityPool and its components
  - Frees the internal array
  - Frees the entity map
  - Frees the pool struct itself

## Core Operations

• **entitypool_add()**: Adds object for given Entity
  - First checks if entity already exists (returns existing if found)
  - Adds new element to array and stores Entity ID in the element
  - Updates entity map to point to new array index (length - 1)
  - Returns pointer to the new element

• **entitypool_remove()**: Removes object associated with Entity
  - Looks up array index from entity map
  - Uses `array_quick_remove()` which may swap with last element
  - If swap occurred, updates the swapped element's mapping
  - Sets entity mapping to -1 to mark as removed

• **entitypool_get()**: Retrieves object for given Entity
  - Looks up array index from entity map
  - Returns array element if found, NULL otherwise

## Iteration and Access Functions

• **entitypool_begin()**: Returns pointer to first array element
• **entitypool_end()**: Returns pointer past last array element  
• **entitypool_nth()**: Returns nth element in array
• **entitypool_size()**: Returns number of elements in pool

## Utility Operations

• **entitypool_clear()**: Empties the pool
  - Clears both entity map and array

• **entitypool_sort()**: Sorts pool elements using comparison function
  - Sorts the underlying array
  - Rebuilds all Entity-to-index mappings since array order changed
  - Iterates through sorted array to update EntityMap

## Serialization Support

• **entitypool_elem_save()**: Saves EntityPoolElem to Store
  - Extracts Entity ID from element and saves it with "pool_elem" key

• **entitypool_elem_load()**: Loads EntityPoolElem from Store
  - Loads Entity ID from "pool_elem" key
  - Adds element to pool with that Entity ID
  - Updates the loaded element pointer

## Key Dependencies

• **EntityMap**: For Entity ID to array index mapping
• **Array**: For contiguous object storage
• **Entity**: Entity ID type and operations
• **Store**: For serialization operations
• **EntityPoolElem**: Element type that contains Entity ID field

## Important Implementation Details

• Uses -1 as sentinel value for non-existent mappings
• Remove operation uses quick removal (swap with last) for O(1) performance
• Sort operation requires full remapping since array indices change
• Elements store their Entity ID internally for reverse lookup during sorting

### EntityPool - Memory Pool Management for Entity Components (entitypool.h)
This header file defines a generic memory pool system for managing entity components in what appears to be an entity-component-system (ECS) architecture.

## Core Data Structures

• **EntityPool** - Opaque struct representing a contiguous memory pool for storing entity components
• **EntityPoolElem** - Required base struct that must be embedded at the top of all pooled data structures
  - Contains an `Entity ent` field used as the key for lookups
  - Must be included in user-defined structs like: `struct Data { EntityPoolElem pool_elem; ... }`

## Key Dependencies

• **entity.h** - Provides `Entity` type and related functions like `entity_destroyed()` and `entity_get_save_filter()`
• **saveload.h** - Provides `Store` type for serialization functionality

## Memory Management Functions

• **entitypool_new_()** / **entitypool_new(type)** - Creates new pool with specified element size
• **entitypool_free()** - Destroys the pool
• **entitypool_clear()** - Removes all elements from pool

## Element Operations

• **entitypool_add()** - Adds element for given entity, returns pointer to new element
• **entitypool_remove()** - Removes element associated with entity
• **entitypool_get()** - Retrieves element by entity key (returns NULL if not found)

## Iteration and Access

• **Direct pointer iteration** - Elements are stored contiguously in memory:
  - `entitypool_begin()` - Returns pointer to first element
  - `entitypool_end()` - Returns one-past-end pointer
  - `entitypool_nth()` - Returns pointer to nth element (0-indexed)
• **entitypool_size()** - Returns number of elements in pool
• **entitypool_sort()** - Sorts elements using provided comparator function

## Utility Macros

• **entitypool_foreach(var, pool)** - Macro for iterating over all elements
  - Variable `var` must be pre-declared as pointer to element type
  - Elements visited in order of increasing index

• **entitypool_remove_destroyed(pool, func)** - Macro for cleanup operations
  - Iterates through pool and calls `func` on each destroyed entity
  - Typically used in `*_update_all()` functions

## Serialization Support

• **entitypool_elem_save()** / **entitypool_elem_load()** - Save/load individual elements
• **entitypool_save_foreach()** / **entitypool_load_foreach()** - Macros for bulk serialization
  - Respect entity save filtering
  - Create hierarchical store structure for organized data

## Important Design Considerations

• **Memory layout warning** - Elements are continuous in memory but may be relocated/shuffled
• **Iterator invalidation** - Adding/removing elements during iteration invalidates pointers
• **Required struct embedding** - All pooled data must include `EntityPoolElem` as first member
• **Usage examples** - References transform.c, sprite.c as implementation examples

This system provides efficient memory management for entity components while maintaining fast iteration and lookup capabilities essential for game engine performance.

### Error Handling Module with Formatted Error Reporting (error.c)
This section implements a simple error handling system that provides formatted error message capabilities and delegates actual error processing to a script module.

## Defined Functions

• **`_error(const char *s)`** - Static helper function
  - Takes a single string parameter
  - Calls `script_error(s)` to delegate error handling to the script module
  - Serves as an internal wrapper for the external error handling mechanism

• **`errorf(const char *fmt, ...)`** - Public formatted error function
  - Accepts printf-style format string and variable arguments
  - Implements a two-pass approach for safe formatted string creation:
    1. **Size calculation pass**: Uses `vsnprintf(NULL, 0, fmt, ap2)` to determine required buffer size
    2. **String creation pass**: Allocates exact buffer size, formats string with `vsprintf()`
  - **Memory management**: Dynamically allocates buffer with `malloc(n + 1)` and properly frees it
  - **Variable argument handling**: Uses `va_list`, `va_start`, `va_copy`, and `va_end` for safe argument processing
  - Calls `_error(s)` to process the formatted error message

## Dependencies

• **External headers**: 
  - `stdarg.h` - for variable argument list handling
  - `stdio.h` - for formatted string functions
  - `stdlib.h` - for memory allocation
  - `error.h` - module's own header
  - `script.h` - for script error handling integration

• **External function references**:
  - `script_error()` - external function that handles the actual error processing

## Key Implementation Details

• Uses `va_copy()` to safely duplicate variable argument lists, allowing multiple passes over the same arguments
• Employs the standard technique of calling `vsnprintf()` with NULL buffer to determine required space
• Ensures proper cleanup by freeing allocated memory after error processing
• Separates formatting logic from error handling logic through the `_error()` abstraction layer

### Error Handling Header with Macro-Based Formatting (error.h)
This header file defines a comprehensive error handling system that provides printf-style error formatting with automatic file and line number inclusion. The system is designed to cause exceptions in scripts when errors occur in C code called from scripts, otherwise it aborts execution.

## Core Function
- **`errorf(const char *fmt, ...)`**: Main error function that accepts printf-style format strings and variable arguments for flexible error message formatting

## Macro System for Line Information
The file implements a three-level macro system to automatically include file and line number information:

- **`line_str__(line)`**: Bottom-level macro that concatenates `__FILE__`, a colon, the stringified line number, and a colon-space suffix
  - Uses `#line` to stringify the line number parameter
  - Produces format: `"filename:linenumber: "`

- **`line_str_(line)`**: Middle-level macro that calls `line_str__(line)`
  - Serves as an indirection layer for proper macro expansion

- **`line_str()`**: Top-level macro that calls `line_str_(__LINE__)`
  - Uses `__LINE__` to automatically capture the current line number
  - This is the macro typically used by other macros

## High-Level Error Macros
- **`error(...)`**: Primary error macro that combines automatic file/line information with custom error messages
  - Calls `errorf()` with `line_str()` prepended to the variable arguments
  - Accepts printf-style format strings and arguments via `__VA_ARGS__`

- **`error_assert(cond, ...)`**: Assertion macro that checks a condition and reports errors with context
  - Evaluates the condition `cond`
  - If condition is true, returns 0 (no error)
  - If condition is false, calls `error()` with:
    - Automatic file/line information
    - The literal condition text (via `#cond`)
    - Custom error message arguments
  - Message format: `"assertion 'condition' failed ... [custom message]"`

## Dependencies
- Relies on standard C preprocessor features: `__FILE__`, `__LINE__`, `#` (stringification), and `__VA_ARGS__`
- Uses include guards (`#ifndef ERROR_H`, `#define ERROR_H`, `#endif`) to prevent multiple inclusions

### Directory Operations Implementation (fs.c)
This section implements a cross-platform directory traversal API using a wrapper around the standard POSIX directory functions. The implementation provides a simplified interface for opening, reading, and closing directories.

## Key Components

### Data Structure
- **`struct Dir`**: A wrapper structure containing a single member `DIR *d` that holds the underlying system directory handle

### Function Definitions

- **`fs_dir_open(const char *path)`**: Opens a directory for reading
  - Allocates memory for a `Dir` structure
  - Calls `opendir(path)` to get the system directory handle
  - Returns `NULL` on failure (with proper cleanup), or the `Dir` pointer on success
  - Handles error cases by freeing allocated memory before returning `NULL`

- **`fs_dir_next_file(Dir *dir)`**: Retrieves the next file/directory entry
  - Calls `readdir(dir->d)` to get the next directory entry
  - Returns the filename (`de->d_name`) if an entry exists
  - Returns `NULL` when no more entries are available

- **`fs_dir_close(Dir *dir)`**: Closes the directory and cleans up resources
  - Calls `closedir(dir->d)` to close the system directory handle
  - Frees the allocated `Dir` structure memory

## Platform Compatibility

The code handles cross-platform compilation differences:
- **Windows (MSVC)**: Uses `#include <dirent_win.h>` 
- **Unix/Linux systems**: Uses standard `#include <dirent.h>`

## Dependencies

- **Internal**: `fs.h` header file
- **External**: Standard library functions (`malloc`, `free`) and directory operations (`opendir`, `readdir`, `closedir`)
- **Platform-specific**: `dirent.h` or `dirent_win.h` for directory entry structures and functions

## Usage Pattern

The API follows a typical open-read-close pattern:
1. Open directory with `fs_dir_open()`
2. Iterate through entries with repeated calls to `fs_dir_next_file()`
3. Close and cleanup with `fs_dir_close()`

### File System Directory Operations Header (fs.h)
This header file (`fs.h`) defines a C API for directory traversal operations that are exported to a scripting system. The file establishes a simple interface for opening directories, iterating through their contents, and properly cleaning up resources.

## Key Components

### Header Structure
- Uses standard header guard pattern with `#ifndef FS_H` / `#define FS_H` / `#endif`
- Includes dependency on `"script_export.h"` for scripting integration macros

### Script Export Block
- All API definitions are wrapped in a `SCRIPT(fs, ...)` macro block
- This suggests the filesystem operations are being exposed to a scripting language or runtime system

### Data Types
- **`Dir`**: An opaque struct typedef representing a directory handle
  - Forward declared only (implementation details hidden)
  - Used as a handle for directory operations

### API Functions
The interface provides three core directory operations:

- **`fs_dir_open(const char *path)`**
  - Opens a directory at the specified path
  - Returns a `Dir*` pointer (directory handle)
  - Marked with `EXPORT` for script system visibility

- **`fs_dir_next_file(Dir *dir)`**
  - Iterates through files in an opened directory
  - Returns `const char*` pointing to the next filename
  - Returns `NULL` when no more files remain
  - Requires valid `Dir*` handle from `fs_dir_open`

- **`fs_dir_close(Dir *dir)`**
  - Closes directory handle and frees associated resources
  - Takes the `Dir*` handle to be cleaned up
  - Essential for proper resource management

### Resource Management
- Includes explicit comment reminder: "remember to *_close(...) when done to free resources!"
- Follows typical C pattern of open/iterate/close lifecycle
- Manual resource cleanup required (no automatic cleanup)

### Dependencies
- **`script_export.h`**: Provides `SCRIPT()` and `EXPORT` macros for binding C functions to scripting system
- Standard C string handling (uses `const char*` for paths and filenames)

### Game Engine Main Loop and Window Management (game.c)
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

### Game Engine Main Header - Core Window Management and Coordinate Systems (game.h)
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

### OpenGL Shader Compilation and Program Creation (gfx.c)
This section implements OpenGL shader compilation and program creation functionality for a graphics system.

## Functions Defined

### `_compile_shader(GLuint shader, const char *filename)`
A static helper function that compiles a shader from a source file:
- **File Reading Process:**
  - Opens the specified shader file in binary mode
  - Determines file size using `fseek()` and `ftell()`
  - Allocates memory buffer for file contents
  - Reads entire file into memory and null-terminates it
  - Closes the file and frees the buffer after use
- **Shader Compilation:**
  - Sets shader source code using `glShaderSource()`
  - Compiles the shader with `glCompileShader()`
  - Retrieves compilation status using `glGetShaderiv()` with `GL_COMPILE_STATUS`
  - Logs compilation results and any error messages via `console_printf()`
  - Returns the compilation status as a `GLint`

### `gfx_create_program(const char *vert_path, const char *geom_path, const char *frag_path)`
Creates a complete OpenGL shader program from optional shader files:
- **Shader Compilation Phase:**
  - Uses a macro `compile(shader, type)` to conditionally compile each shader type
  - Creates vertex shader (`GL_VERTEX_SHADER`) if `vert_path` is provided
  - Creates geometry shader (`GL_GEOMETRY_SHADER`) if `geom_path` is provided
  - Creates fragment shader (`GL_FRAGMENT_SHADER`) if `frag_path` is provided
  - Returns 0 if any shader compilation fails
- **Program Linking Phase:**
  - Creates a new OpenGL program object with `glCreateProgram()`
  - Attaches all successfully compiled shaders to the program
  - Links the program using `glLinkProgram()`
  - Cleans up by deleting individual shader objects (OpenGL automatically detaches them)
  - Returns the program handle

## Dependencies
- **Headers:** `gfx.h`, `stdlib.h`, `stdio.h`, `console.h`
- **External Functions:** `console_printf()` for logging output
- **OpenGL Functions:** `glCreateShader()`, `glShaderSource()`, `glCompileShader()`, `glGetShaderiv()`, `glGetShaderInfoLog()`, `glCreateProgram()`, `glAttachShader()`, `glLinkProgram()`, `glDeleteShader()`

## Key Features
- Flexible shader program creation supporting any combination of vertex, geometry, and fragment shaders
- Comprehensive error logging and status reporting
- Automatic memory management and cleanup
- File-based shader source loading

### OpenGL Graphics Utility Header (gfx.h)
This is a header file that provides OpenGL graphics utilities and macros for shader program management and vertex attribute binding.

## Dependencies
- Includes `GL/glew.h` for OpenGL Extension Wrangler Library support

## Function Declarations

### `gfx_create_program`
- **Purpose**: Compiles and links an OpenGL shader program from shader file paths
- **Parameters**:
  - `const char *vert_path`: Path to vertex shader file (can be NULL)
  - `const char *geom_path`: Path to geometry shader file (can be NULL)  
  - `const char *frag_path`: Path to fragment shader file (can be NULL)
- **Returns**: `GLuint` program ID
- **Note**: Does not call `glUseProgram()` after creation

### `gfx_free_program`
- **Purpose**: Frees/deletes an OpenGL shader program
- **Parameters**: `GLuint program` - the program ID to free

## Utility Macros

### `poffsetof`
- **Purpose**: Gets the pointer offset of a field within a struct type
- **Parameters**:
  - `type`: The struct type
  - `field`: The field name within the struct
- **Returns**: `void *` pointer to the field offset
- **Implementation**: Uses pointer arithmetic with a null pointer cast to calculate offset

### `gfx_bind_vertex_attrib`
- **Purpose**: Binds vertex attribute data to a specific field in a struct type for OpenGL rendering
- **Parameters**:
  - `program`: OpenGL program ID
  - `gl_type`: OpenGL data type (e.g., `GL_FLOAT`)
  - `components`: Number of components in the attribute
  - `param_name`: String name of the parameter as it appears in the shader program
  - `type`: The struct type containing the vertex data
  - `field`: The specific field within the struct to bind
- **Implementation Steps**:
  1. Gets the attribute location using `glGetAttribLocation()`
  2. Sets up vertex attribute pointer using `glVertexAttribPointer()` with:
     - Attribute location
     - Number of components
     - Data type
     - Normalization disabled (`GL_FALSE`)
     - Struct size as stride
     - Field offset as pointer offset
  3. Enables the vertex attribute array using `glEnableVertexAttribArray()`
- **Usage Examples**: Referenced in `sprite.c` and `text.c` files

## Header Guard
- Uses standard header guard pattern with `GFX_H` macro to prevent multiple inclusions

### OpenGL Library Header Integration (glew_glfw.h)
This is a header file that provides a standardized way to include OpenGL extension libraries in the correct order.

**File Structure:**
- **Header guard**: `GLEW_GLFW_H` prevents multiple inclusions of this header
- **Include directives**: Ensures proper ordering of OpenGL library headers

**Key Dependencies:**
- `GL/glew.h` - OpenGL Extension Wrangler Library header
- `GLFW/glfw3.h` - OpenGL Framework (GLFW) version 3 header

**Primary Purpose:**
- **Include order enforcement**: The comment explicitly states "make sure to include GLEW headers before GLFW ones"
  - This ordering is critical because GLEW must be initialized before GLFW can properly access OpenGL extensions
  - Incorrect ordering can lead to compilation errors or runtime issues with OpenGL function loading

**Usage Context:**
- This header serves as a convenience wrapper for projects that need both GLEW and GLFW
- Developers can include this single header instead of manually including both libraries in the correct order
- Prevents common integration mistakes when setting up OpenGL applications that require both extension loading (GLEW) and window/context management (GLFW)

### GUI System Implementation - Core Components and UI Elements (gui.c)
This file implements a comprehensive GUI system for a game engine, providing common GUI functionality and specific UI components like rectangles, text, and text editing fields.

## Core GUI System (`gui.c`)

### Main Data Structures and Pools
- **`Gui` struct**: Core GUI component containing visibility, focus, color, bounding box, alignment, and padding properties
- **`gui_pool`**: EntityPool managing all GUI entities
- **Event maps**: Multiple EntityMap instances for tracking GUI events:
  - `focus_enter_map`, `focus_exit_map`: Focus state changes
  - `changed_map`: GUI element changes
  - `mouse_down_map`, `mouse_up_map`: Mouse interactions
  - `key_down_map`, `key_up_map`: Keyboard interactions

### Core GUI Management
- **`gui_root`**: Static root entity that all GUI elements attach to for screen positioning
- **`focused`**: Currently focused entity (or `entity_nil` if none)
- **`captured_event`**: Boolean flag indicating if GUI captured an input event

### Key Functions
- **Entity management**: `gui_add()`, `gui_remove()`, `gui_has()`
- **Property setters/getters**: Functions for color, visibility, focus, alignment, padding
- **Event handling**: Functions for mouse/keyboard events and focus management
- **Update system**: `_common_update_visible()` recursively computes visibility, `_common_update_align()` handles alignment

### Alignment System
- **`GuiAlign` enum**: Supports `GA_NONE`, `GA_MIN`, `GA_MAX`, `GA_MID`, `GA_TABLE`
- **`_common_align()`**: Positions elements based on parent bounding box and alignment settings
- **Table alignment**: Special `GA_TABLE` mode for sequential layout

## Rectangle Component

### Data Structure
- **`Rect` struct**: Contains size, visibility, color, fit/fill options, and depth for rendering order
- **`rect_pool`**: EntityPool for rectangle entities

### Key Features
- **Fitting**: `hfit`/`vfit` automatically size to fit children
- **Filling**: `hfill`/`vfill` expand to fill parent container
- **Depth sorting**: `_rect_depth_compare()` ensures proper rendering order
- **Update pipeline**:
  - `_rect_update_child_first()`: Processes children before parents for fitting
  - `_rect_update_parent_first()`: Processes parents before children for filling
  - `_rect_update_table_align()`: Handles table-based sequential layout

### Rendering
- **OpenGL integration**: Uses shader program with vertex arrays for efficient rendering
- **Shader uniforms**: Passes transformation matrices, size, color, and visibility data
- **Instanced rendering**: Renders all rectangles in a single draw call

## Text Component

### Data Structures
- **`Text` struct**: Contains string data, character array, bounds, and cursor position
- **`TextChar` struct**: Individual character data with position, font cell coordinates, and cursor flag
- **Font system**: Uses 16x16 character grid with 10x12 pixel characters

### Key Features
- **Dynamic text rendering**: `_text_set_str()` converts strings to renderable character arrays
- **Cursor support**: Handles cursor positioning and blinking animation
- **Newline handling**: Supports multi-line text with proper positioning
- **Bounds calculation**: Automatically computes text bounding box for layout

### Rendering Pipeline
- **Font texture**: Uses `font1.png` with grid-based character lookup
- **Shader program**: Specialized text rendering with cursor blinking support
- **Per-entity rendering**: Each text entity rendered separately with individual transforms

## Text Edit Component

### Data Structure
- **`TextEdit` struct**: Extends text component with cursor position and numerical input flag
- **`textedit_pool`**: EntityPool for text editing entities

### Input Handling
- **Keyboard events**: `_textedit_key_event()` handles all keyboard input
- **Character insertion**: Supports printable character input with string manipulation
- **Cursor movement**: Left/right arrow key navigation
- **Text deletion**: Backspace and delete key support
- **Focus management**: Enter/escape keys for focus control

### Key Functions
- **`_textedit_fix_cursor()`**: Ensures cursor stays within valid string bounds
- **`_textedit_set_str()`**: Updates text and fires change events
- **Numerical mode**: `numerical` flag for numeric-only input validation

## System Integration

### Update Pipeline
1. **Visibility computation**: Recursive parent-to-child visibility inheritance
2. **Alignment reset**: Move elements to reference positions
3. **Component updates**: Process text editing, text rendering, rectangle layout
4. **Final alignment**: Apply actual alignment settings
5. **Transform updates**: Update world matrices for rendering

### Event System
- **Mouse handling**: `_common_mouse_event()` performs hit testing and focus management
- **Keyboard routing**: Routes input to focused elements
- **Event clearing**: `_common_event_clear()` resets all event flags each frame

### Save/Load System
- **Serialization**: Each component implements save/load functions for game state persistence
- **Entity reconstruction**: Properly restores GUI hierarchies and properties

This GUI system provides a complete foundation for game UI with hierarchical layout, event handling, and multiple specialized components working together through a unified entity-component architecture.

### GUI System Header - Entity-based User Interface Components (gui.h)
This header file defines a comprehensive GUI system built on an entity-component architecture. The system provides a complete interface for creating and managing GUI elements with layout, styling, input handling, and specialized components.

## Core Dependencies
- `entity.h` - Entity system for GUI component management
- `color.h` - Color representation for GUI styling
- `vec2.h` - 2D vector math for positioning and sizing
- `input.h` - Input handling (keyboard/mouse events)
- `script_export.h` - Script binding system via `SCRIPT` macro

## Main GUI System Functions

### Root Entity Management
- **`gui_get_root()`** - Returns the root entity that serves as parent for all GUI elements
  - Root entity uses screen pixel coordinates
  - Automatically stays within camera view

### Entity Management
- **`gui_add(Entity ent)`** - Adds entity to GUI system
- **`gui_remove(Entity ent)`** - Removes entity from GUI system  
- **`gui_has(Entity ent)`** - Checks if entity is in GUI system

### Visual Properties
- **Color control**: `gui_set_color()` / `gui_get_color()`
- **Visibility control**: `gui_set_visible()` / `gui_get_visible()`

### Layout and Alignment System
- **`GuiAlign` enum** defines alignment options:
  - `GA_MIN` (0) - Horizontal: left, Vertical: bottom
  - `GA_MID` (1) - Horizontal: center, Vertical: center  
  - `GA_MAX` (2) - Horizontal: right, Vertical: top
  - `GA_TABLE` (3) - Horizontal: left-right table, Vertical: top-down table
  - `GA_NONE` (4) - Manual positioning
- **Alignment functions**: `gui_set_halign()`, `gui_get_halign()`, `gui_set_valign()`, `gui_get_valign()`
- **Padding control**: `gui_set_padding()` / `gui_get_padding()` using `Vec2` for horizontal/vertical values

## Input and Event System

### Focus Management
- **`gui_set_focusable(Entity ent, bool focusable)`** - Controls whether entity can receive focus
- **`gui_get_focusable(Entity ent)`** - Checks if entity can be focused
- **`gui_set_focused_entity(Entity ent)`** - Sets which entity has focus (entity_nil for no focus)
- **`gui_get_focused_entity()`** - Returns currently focused entity
- **`gui_set_focus()` / `gui_get_focus()`** - Individual entity focus control
- **`gui_has_focus()`** - Checks if any GUI element has focus

### Event Capture
- **`gui_set_captures_events()` / `gui_get_captures_events()`** - Controls whether entity captures input events
- **`gui_captured_event()`** - Checks if current event was captured by GUI system

### Event Handling Functions
- **Focus events**: `gui_event_focus_enter()`, `gui_event_focus_exit()`
- **Input change events**: `gui_event_changed()`, `gui_fire_event_changed()`
- **Mouse events**: `gui_event_mouse_down()`, `gui_event_mouse_up()` (return `MouseCode`)
- **Keyboard events**: `gui_event_key_down()`, `gui_event_key_up()` (return `KeyCode`)

## Specialized GUI Components

### GUI Rectangle Component (`gui_rect`)
- **Management**: `gui_rect_add()`, `gui_rect_remove()`, `gui_rect_has()`
- **Size control**: `gui_rect_set_size()` / `gui_rect_get_size()` using `Vec2`
- **Layout behavior**:
  - **Fit controls**: `gui_rect_set_hfit()`, `gui_rect_set_vfit()` - Size to content
  - **Fill controls**: `gui_rect_set_hfill()`, `gui_rect_set_vfill()` - Expand to fill space

### GUI Text Component (`gui_text`)
- **Management**: `gui_text_add()`, `gui_text_remove()`, `gui_text_has()`
- **Content**: `gui_text_set_str()` / `gui_text_get_str()` for text content
- **Cursor**: `gui_text_set_cursor()` for text cursor positioning

### GUI Text Edit Component (`gui_textedit`)
- **Management**: `gui_textedit_add()`, `gui_textedit_remove()`, `gui_textedit_has()`
- **Cursor control**: `gui_textedit_set_cursor()` / `gui_textedit_get_cursor()` using `unsigned int`
- **Numerical input**: 
  - `gui_textedit_set_numerical()` / `gui_textedit_get_numerical()` - Enable/disable numerical-only input
  - `gui_textedit_get_num()` - Get numerical value (returns 0 if not numerical, uses `Scalar` type)

## System Management Functions (Non-exported)
- **`gui_event_clear()`** - Clears pending GUI events
- **Lifecycle**: `gui_init()`, `gui_deinit()`
- **Per-frame updates**: `gui_update_all()`, `gui_draw_all()`
- **Input processing**: `gui_key_down()`, `gui_key_up()`, `gui_char_down()`, `gui_mouse_down()`, `gui_mouse_up()`
- **Persistence**: `gui_save_all(Store *s)`, `gui_load_all(Store *s)`

The system uses the `SCRIPT()` macro to export most functions to a scripting system, making the GUI accessible from scripts while keeping core system functions (init, update, draw) as internal C functions.

### Input System Implementation with GLFW Integration (input.c)
This file implements a comprehensive input handling system that acts as a wrapper around GLFW, providing callback-based event handling for keyboard, mouse, and scroll inputs.

## Core Architecture

The system uses **callback lists** to manage input event handlers:
- `key_down_cbs`, `key_up_cbs` - Arrays storing keyboard event callbacks
- `char_down_cbs` - Array for character input callbacks  
- `mouse_down_cbs`, `mouse_up_cbs` - Arrays for mouse button callbacks
- `mouse_move_cbs` - Array for mouse movement callbacks
- `scroll_cbs` - Array for scroll event callbacks

## Key Conversion Functions

**GLFW Integration Functions:**
- `_keycode_to_glfw()` - Converts KeyCode to GLFW key codes (direct passthrough)
- `_glfw_to_keycode()` - Converts GLFW key codes to KeyCode (direct passthrough)  
- `_mousecode_to_glfw()` - Converts MouseCode to GLFW mouse codes (direct passthrough)
- `_glfw_to_mousecode()` - Converts GLFW mouse codes to MouseCode (direct passthrough)

**Character Handling Functions:**
- `input_keycode_is_char()` - Determines if a KeyCode represents a printable character
  - Returns true for: space, punctuation, digits 0-9, letters A-Z, brackets, special characters
- `input_keycode_to_char()` - Converts KeyCode to lowercase character using `tolower()`
- `input_char_to_keycode()` - Converts character to KeyCode using `toupper()`

## Input State Query Functions

**Current State Polling:**
- `input_key_down()` - Checks if a key is currently pressed using `glfwGetKey()`
- `input_mouse_down()` - Checks if a mouse button is currently pressed using `glfwGetMouseButton()`
- `input_get_mouse_pos_pixels()` - Returns mouse position in pixels as Vec2 (note: y-coordinate is negated)
- `input_get_mouse_pos_unit()` - Returns mouse position in game units by calling `game_pixels_to_unit()`

## Callback Registration Functions

**Event Handler Registration:**
- `input_add_key_down_callback()` - Registers KeyCallback for key press events
- `input_add_key_up_callback()` - Registers KeyCallback for key release events
- `input_add_char_down_callback()` - Registers CharCallback for character input events
- `input_add_mouse_down_callback()` - Registers MouseCallback for mouse press events
- `input_add_mouse_up_callback()` - Registers MouseCallback for mouse release events
- `input_add_mouse_move_callback()` - Registers MouseMoveCallback for mouse movement
- `input_add_scroll_callback()` - Registers ScrollCallback for scroll events

## GLFW Callback Handlers

**Internal Event Processors:**
- `_key_callback()` - Handles GLFW key events, dispatches to registered KeyCallback functions based on GLFW_PRESS/GLFW_RELEASE
- `_char_callback()` - Handles GLFW character events, dispatches to all registered CharCallback functions
- `_mouse_callback()` - Handles GLFW mouse button events, dispatches based on GLFW_PRESS/GLFW_RELEASE
- `_cursor_pos_callback()` - Handles GLFW cursor position events, negates y-coordinate and dispatches to MouseMoveCallback functions
- `_scroll_callback()` - Handles GLFW scroll events, dispatches to ScrollCallback functions

## Lifecycle Management

**Initialization (`input_init()`):**
- Creates callback arrays using `array_new()` for each event type
- Registers GLFW callbacks using `glfwSetKeyCallback()`, `glfwSetCharCallback()`, etc.
- Links internal handlers to GLFW event system

**Cleanup (`input_deinit()`):**
- Frees all callback arrays using `array_free()` in reverse order of initialization

## Key Dependencies

- **External modules:** `game` (for `game_window` and `game_pixels_to_unit()`), `glew_glfw.h`, `array.h`, `error.h`
- **Standard libraries:** `ctype.h` for `tolower()` and `toupper()`
- **Data types:** KeyCode, MouseCode, Vec2, various callback function pointer types

The system provides a clean abstraction over GLFW's input handling while maintaining direct access to current input states and flexible event-driven input processing.

### Input System Header - Keyboard and Mouse Input Handling (input.h)
This header file defines a comprehensive input system for handling keyboard and mouse input, with both script-exportable functions and internal callback mechanisms.

## Key Enumerations

• **KeyCode enum**: Defines keyboard key codes with specific numeric values
  - `KC_UNKNOWN` (-1) and `KC_NONE` (0) for undefined/no key states
  - ASCII characters (32-162): space, punctuation, digits 0-9, letters A-Z, brackets, backslash, grave accent, and world keys
  - Function keys (256-348): escape, enter, tab, backspace, navigation keys, page up/down, home, end, lock keys, print screen, pause
  - Function keys F1-F25 (290-314)
  - Keypad keys KP_0 through KP_9 and keypad operators (320-336)
  - Modifier keys: left/right shift, control, alt, super (340-347)
  - Menu key (348)

• **MouseCode enum**: Defines mouse button codes
  - Primary buttons MC_1 through MC_8 (0-7)
  - `MC_NONE` (32) for no button
  - Convenient aliases: `MC_LEFT` (MC_1), `MC_RIGHT` (MC_2), `MC_MIDDLE` (MC_3)

## Script-Exported Functions

Functions marked with `EXPORT` and wrapped in `SCRIPT()` macro for external script access:

• **Key conversion utilities**:
  - `input_keycode_is_char()`: checks if KeyCode represents a character
  - `input_keycode_to_char()`: converts KeyCode to character
  - `input_char_to_keycode()`: converts character to KeyCode

• **Input state queries**:
  - `input_key_down()`: checks if specified key is currently pressed
  - `input_mouse_down()`: checks if specified mouse button is pressed

• **Mouse position functions**:
  - `input_get_mouse_pos_pixels()`: returns mouse position in pixel coordinates (Vec2)
  - `input_get_mouse_pos_unit()`: returns mouse position in unit coordinates (Vec2)

## Callback System

Internal callback mechanism for event-driven input handling:

• **Callback type definitions**:
  - `KeyCallback`: function pointer for key events, takes KeyCode parameter
  - `CharCallback`: function pointer for character input, takes unsigned int parameter
  - `MouseCallback`: function pointer for mouse button events, takes MouseCode parameter
  - `MouseMoveCallback`: function pointer for mouse movement, takes Vec2 position
  - `ScrollCallback`: function pointer for scroll events, takes Vec2 scroll data

• **Callback registration functions**:
  - `input_add_key_down_callback()` / `input_add_key_up_callback()`: register key press/release handlers
  - `input_add_char_down_callback()`: register character input handler
  - `input_add_mouse_down_callback()` / `input_add_mouse_up_callback()`: register mouse button handlers
  - `input_add_mouse_move_callback()`: register mouse movement handler
  - `input_add_scroll_callback()`: register scroll wheel handler

## System Management

• **Lifecycle functions**:
  - `input_init()`: initialize the input system
  - `input_deinit()`: cleanup and shutdown the input system

## Dependencies

• **External dependencies**:
  - `stdbool.h`: for boolean type support
  - `script_export.h`: provides `SCRIPT()` macro and `EXPORT` functionality
  - `vec2.h`: provides `Vec2` type for 2D coordinate handling

This input system provides a dual-interface approach: script-accessible functions for polling input state and a callback system for event-driven input handling within the C codebase.

### Main Program Entry Point (main.c)
This section contains the main entry point for the application, implementing a minimal main function that delegates execution to the game system.

**Function Defined:**
• `main(int argc, char **argv)` - Standard C program entry point that accepts command-line arguments

**Key Dependencies:**
• `#include "game.h"` - Includes the game module header file
• `game_run(argc, argv)` - Calls the main game execution function, passing through all command-line arguments

**Program Flow:**
• The main function performs a simple delegation pattern:
  - Receives command-line arguments (argument count and argument vector)
  - Passes these arguments directly to `game_run()` function
  - Returns 0 to indicate successful program termination

**Architecture Notes:**
• This is a thin wrapper that separates the program entry point from the actual game logic
• All command-line argument processing and game execution is handled by the `game_run()` function defined elsewhere
• The main function assumes `game_run()` will handle all error conditions and program flow

### 3x3 Matrix Operations Implementation (mat3.c)
This section implements a comprehensive 3x3 matrix mathematics library in C, providing essential matrix operations for 2D transformations and linear algebra.

## Core Functions Defined

• **Matrix Construction Functions:**
  - `mat3()` - Creates a Mat3 from 9 scalar values (m00, m01, m02, m10, m11, m12, m20, m21, m22)
  - `mat3_identity()` - Returns a 3x3 identity matrix with 1.0f on diagonal, 0.0f elsewhere

• **Matrix Arithmetic Operations:**
  - `mat3_mul()` - Performs matrix multiplication of two Mat3 matrices using standard row-column dot product algorithm
    - Implements the mathematical operation: result[i][j] = Σ(m[i][k] * n[k][j])
    - Returns new Mat3 containing the product

• **Transformation Matrix Operations:**
  - `mat3_scaling_rotation_translation()` - Creates a 2D transformation matrix from:
    - `Vec2 scale` - scaling factors for x and y axes
    - `Scalar rot` - rotation angle in radians
    - `Vec2 trans` - translation offsets
    - Combines operations in SRT order using trigonometric functions

• **Matrix Decomposition Functions:**
  - `mat3_get_translation()` - Extracts translation vector from matrix elements m[2][0] and m[2][1]
  - `mat3_get_rotation()` - Calculates rotation angle using `scalar_atan2(m[0][1], m[0][0])`
  - `mat3_get_scale()` - Computes scale factors by calculating magnitude of first two column vectors

• **Matrix Inversion:**
  - `mat3_inverse()` - Computes matrix inverse using cofactor method:
    - Calculates 3x3 cofactor matrix
    - Computes determinant using first row expansion
    - Checks for near-zero determinant (threshold: 10e-8)
    - Divides all elements by determinant if invertible
    - Returns cofactor matrix unchanged if not invertible (TODO: handle properly)

• **Vector Transformation:**
  - `mat3_transform()` - Applies matrix transformation to Vec2:
    - Treats Vec2 as homogeneous coordinate (x, y, 1)
    - Returns transformed 2D point: (m[0][0]*x + m[1][0]*y + m[2][0], m[0][1]*x + m[1][1]*y + m[2][1])

• **Serialization Support:**
  - `mat3_save()` - Saves matrix to Store using compressed child storage
  - `mat3_load()` - Loads matrix from Store with default fallback value
  - Both functions iterate through 3x3 matrix elements using nested loops

## Key Dependencies

• **External Types Referenced:**
  - `Mat3` - 3x3 matrix type with `m[3][3]` member array
  - `Vec2` - 2D vector type with `x`, `y` members  
  - `Scalar` - floating-point scalar type
  - `Store` - serialization storage type

• **External Functions Called:**
  - `scalar_cos()`, `scalar_sin()`, `scalar_atan2()`, `scalar_sqrt()` - trigonometric operations
  - `vec2()` - Vec2 constructor
  - `scalar_save()`, `scalar_load()` - scalar serialization
  - `store_child_save_compressed()`, `store_child_load()` - storage operations

## Implementation Notes

• The file uses `#undef` directives for `mat3_identity` and `mat3`, suggesting these may be defined as macros elsewhere
• Matrix inverse function has incomplete error handling for non-invertible matrices
• All matrix operations follow standard mathematical conventions for 3x3 matrices
• The transformation matrix function creates matrices suitable for 2D graphics transformations

### 3x3 Matrix Data Structure and Operations Header (mat3.h)
This header file defines a 3x3 matrix data structure and its associated operations for 2D transformations and linear algebra.

## Core Data Structure
- **Mat3**: A 3x3 matrix structure containing a 2D array `Scalar m[3][3]`
  - Stored in column-major order for efficient graphics operations
  - Matrix element access: `m.m[column][row]`
  - Visual layout shows columns as vertical vectors in the matrix representation

## Dependencies
- **scalar.h**: Provides the `Scalar` type used for matrix elements
- **vec2.h**: Provides `Vec2` type for 2D vector operations
- **script_export.h**: Provides `SCRIPT` and `EXPORT` macros for code generation
- **saveload.h**: Provides `Store` type and `bool` for serialization functions

## Matrix Construction Functions
- **mat3()**: Creates a matrix from 9 scalar values (m00-m22 in row-major parameter order)
- **mat3_identity()**: Returns a 3x3 identity matrix
- **mat3_scaling_rotation_translation()**: Creates a transformation matrix combining scale, rotation, and translation operations in sequence

## Matrix Operations
- **mat3_mul()**: Multiplies two Mat3 matrices (m * n)
- **mat3_inverse()**: Computes the inverse of a Mat3 matrix
- **mat3_transform()**: Transforms a Vec2 vector by a Mat3 matrix

## Transformation Extraction
- **mat3_get_translation()**: Extracts translation component as Vec2
- **mat3_get_rotation()**: Extracts rotation component as Scalar
- **mat3_get_scale()**: Extracts scale component as Vec2

## Serialization Functions
- **mat3_save()**: Saves a Mat3 to a Store with a given name
- **mat3_load()**: Loads a Mat3 from a Store with a given name and default value

## C Inline Implementations
- **mat3 macro**: Inline constructor macro that creates Mat3 literals with proper column-major ordering
- **mat3_identity macro**: Inline macro that creates identity matrix with 1.0f on diagonal, 0.0f elsewhere

The header uses a SCRIPT wrapper macro system, likely for automatic binding generation to scripting languages, with all functions marked as EXPORT for external visibility.

### Physics System Implementation Using Chipmunk Physics Engine (physics.c)
This file implements a comprehensive physics system that wraps the Chipmunk physics engine for use in an entity-component-system (ECS) architecture. It provides 2D rigid body physics simulation with collision detection, shape management, and integration with a transform system.

## Core Data Structures

• **PhysicsInfo**: Per-entity physics information containing:
  - `EntityPoolElem pool_elem`: Entity pool management
  - `PhysicsBody type`: Body type (static, dynamic, kinematic)
  - `Scalar mass`: Mass stored separately for type conversions
  - `cpVect last_pos`, `cpFloat last_ang`: Previous position/angle for kinematic bodies
  - `unsigned int last_dirty_count`: Transform synchronization tracking
  - `cpBody *body`: Chipmunk physics body
  - `Array *shapes`: Collection of attached collision shapes
  - `Array *collisions`: Current frame collision information

• **ShapeInfo**: Per-shape information containing:
  - `PhysicsShape type`: Shape type (circle, polygon)
  - `cpShape *shape`: Chipmunk shape object

## Global State Management

• **Static Variables**:
  - `cpSpace *space`: Main Chipmunk physics simulation space
  - `Scalar period`: Fixed timestep for physics simulation (default 1/60 second)
  - `EntityPool *pool`: Entity pool for managing PhysicsInfo instances
  - `EntityMap *debug_draw_map`: Tracks entities marked for debug visualization

## Physics Body Management

• **Body Creation and Removal**:
  - `physics_add()`: Creates physics component for entity, initializes Chipmunk body
  - `physics_remove()`: Removes all physics objects and cleans up memory
  - `physics_has()`: Checks if entity has physics component

• **Body Type Management**:
  - `physics_set_type()`, `physics_get_type()`: Manage body types (PB_STATIC, PB_DYNAMIC, PB_KINEMATIC)
  - `_set_type()`: Internal function handling Chipmunk body type conversions
  - Special handling for kinematic bodies to track velocity computation

## Shape Management System

• **Shape Creation Functions**:
  - `physics_shape_add_circle()`: Creates circular collision shape
  - `physics_shape_add_box()`: Creates rectangular collision shape
  - `physics_shape_add_poly()`: Creates convex polygon shape with automatic convex hull computation
  - `_shape_add()`: Internal function for common shape initialization

• **Shape Properties**:
  - `physics_shape_set_surface_velocity()`, `physics_shape_get_surface_velocity()`: Surface velocity for conveyor belt effects
  - `physics_shape_set_sensor()`, `physics_shape_get_sensor()`: Sensor shapes (detect collisions without physical response)
  - `physics_shape_remove()`: Removes individual shapes and recalculates moment of inertia

• **Shape Utilities**:
  - `physics_convex_hull()`: Computes convex hull of vertex array
  - `_recalculate_moment()`: Recalculates moment of inertia based on all shapes

## Dynamics and Forces

• **Mass and Inertia**:
  - `physics_set_mass()`, `physics_get_mass()`: Mass management
  - `physics_set_freeze_rotation()`, `physics_get_freeze_rotation()`: Rotation locking

• **Velocity and Angular Velocity**:
  - `physics_set_velocity()`, `physics_get_velocity()`: Linear velocity
  - `physics_set_angular_velocity()`, `physics_get_angular_velocity()`: Angular velocity
  - Velocity limit functions for both linear and angular motion

• **Force Application**:
  - `physics_apply_force()`, `physics_apply_force_at()`: Apply forces
  - `physics_apply_impulse()`, `physics_apply_impulse_at()`: Apply impulses
  - `physics_reset_forces()`: Clear accumulated forces

## Collision Detection

• **Collision Queries**:
  - `physics_get_num_collisions()`, `physics_get_collisions()`: Access collision information
  - `_add_collision()`: Callback for gathering collisions from Chipmunk
  - `_update_collisions()`: Lazy collision list building

• **Spatial Queries**:
  - `physics_nearest()`: Find nearest shape to a point within maximum distance

## Simulation Update Loop

• **Main Update Process** (`physics_update_all()`):
  1. Remove destroyed entities from physics system
  2. Clear debug draw map
  3. Update kinematic bodies from transform system
  4. Step physics simulation with fixed timestep
  5. Synchronize transforms with physics bodies (bidirectional)
  6. Update dirty count tracking

• **Kinematic Body Updates** (`_update_kinematics()`):
  - Computes velocities based on position changes
  - Updates Chipmunk body positions from transform system
  - Reindexes spatial hash for collision detection

• **Fixed Timestep Simulation** (`_step()`):
  - Accumulates frame time and steps simulation at fixed intervals
  - Ensures consistent physics behavior regardless of framerate

## Debug Visualization

• **Debug Drawing System**:
  - OpenGL-based wireframe rendering of physics shapes
  - `physics_draw_all()`: Renders all shapes marked for debug display
  - `_circle_draw()`, `_polygon_draw()`: Shape-specific rendering functions
  - Integration with camera system for proper viewport transformation

## Serialization System

• **Save/Load Infrastructure**:
  - `physics_save_all()`, `physics_load_all()`: Complete physics state persistence
  - `_body_save()`, `_body_load()`: Chipmunk body properties
  - `_shapes_save()`, `_shapes_load()`: Shape collection with type-specific data
  - Automatic restoration of entity references and physics object relationships

## Key Dependencies

• **External Systems**:
  - Chipmunk Physics Engine (`chipmunk.h`)
  - Transform system for position/rotation synchronization
  - Entity pool management system
  - Graphics system for debug visualization
  - Timing system for fixed timestep simulation
  - Store system for serialization

• **Utility Functions**:
  - `cpv_of_vec2()`, `vec2_of_cpv()`: Vector format conversions
  - `_remove_body()`, `_remove_shape()`: Safe Chipmunk object cleanup

This physics system provides a complete 2D rigid body physics solution with proper integration into the game engine's ECS architecture, including transform synchronization, collision detection, debug visualization, and full serialization support.

### Physics Engine Header - Core Physics System Interface (physics.h)
This header file defines the complete interface for a 2D physics engine system, providing comprehensive physics simulation capabilities for game entities.

## Dependencies
- References external headers: `saveload.h`, `script_export.h`, `scalar.h`, `entity.h`, `vec2.h`, `bbox.h`
- Uses `stdbool.h` for boolean types
- All functions are wrapped in `SCRIPT(physics, ...)` macro for script export

## Global Physics Configuration
- **physics_set_gravity(Vec2 g)** / **physics_get_gravity()** - Controls world gravity vector
- **physics_set_simulation_frequency(Scalar freq)** / **physics_get_simulation_frequency()** - Controls physics update rate

## Physics Body Management
- **PhysicsBody enum** defines three body types:
  - `PB_STATIC` (0) - Immovable objects like walls
  - `PB_KINEMATIC` (1) - Moving objects not subject to dynamics (e.g., moving platforms)
  - `PB_DYNAMIC` (2) - Full physics simulation objects (e.g., bowling pins)
- **physics_add(Entity ent)** - Adds entity to physics system (defaults to PB_DYNAMIC)
- **physics_remove(Entity ent)** / **physics_has(Entity ent)** - Entity management
- **physics_set_type(Entity ent, PhysicsBody type)** / **physics_get_type(Entity ent)** - Body type control
- **physics_debug_draw(Entity ent)** - Debug visualization for one frame

## Shape System
- **PhysicsShape enum** defines shape types:
  - `PS_CIRCLE` (0) - Circular collision shapes
  - `PS_POLYGON` (1) - Polygonal collision shapes
- Shape creation functions (all return shape index):
  - **physics_shape_add_circle(Entity ent, Scalar r, Vec2 offset)** - Adds circle with radius and offset
  - **physics_shape_add_box(Entity ent, BBox b, Scalar r)** - Adds box with rounding radius
  - **physics_shape_add_poly(Entity ent, unsigned int nverts, const Vec2 *verts, Scalar r)** - Adds custom polygon
- Shape management:
  - **physics_get_num_shapes(Entity ent)** / **physics_shape_get_type(Entity ent, unsigned int i)** - Shape queries
  - **physics_shape_remove(Entity ent, unsigned int i)** - Shape removal
  - **physics_poly_get_num_verts(Entity ent, unsigned int i)** - Polygon vertex count (-1 if not polygon)
- **physics_convex_hull(unsigned int nverts, Vec2 *verts)** - In-place convex hull computation
- Shape properties:
  - **physics_shape_set_sensor(Entity ent, unsigned int i, bool sensor)** / **physics_shape_get_sensor(Entity ent, unsigned int i)** - Sensor flag (collision detection without physical response)
  - **physics_shape_set_surface_velocity(Entity ent, unsigned int i, Vec2 v)** / **physics_shape_get_surface_velocity(Entity ent, unsigned int i)** - Surface velocity for conveyor belt effects

## Dynamics System
- **Mass**: physics_set_mass() / physics_get_mass()
- **Rotation constraints**: physics_set_freeze_rotation() / physics_get_freeze_rotation()
- **Linear motion**:
  - Velocity: physics_set_velocity() / physics_get_velocity()
  - Force: physics_set_force() / physics_get_force()
  - Velocity limits: physics_set_velocity_limit() / physics_get_velocity_limit()
- **Angular motion**:
  - Angular velocity: physics_set_angular_velocity() / physics_get_angular_velocity()
  - Torque: physics_set_torque() / physics_get_torque()
  - Angular velocity limits: physics_set_angular_velocity_limit() / physics_get_angular_velocity_limit()
- **Force application**:
  - **physics_reset_forces(Entity ent)** - Clears all forces
  - **physics_apply_force(Entity ent, Vec2 force)** - Applies force at center of mass
  - **physics_apply_force_at(Entity ent, Vec2 force, Vec2 at)** - Applies force at specific point
  - **physics_apply_impulse(Entity ent, Vec2 impulse)** - Applies impulse at center of mass
  - **physics_apply_impulse_at(Entity ent, Vec2 impulse, Vec2 at)** - Applies impulse at specific point

## Collision Detection
- **Collision struct** contains:
  - `Entity a, b` - The two colliding entities
- **physics_get_num_collisions(Entity ent)** - Returns collision count for entity
- **physics_get_collisions(Entity ent)** - Returns array of Collision structs

## Spatial Queries
- **NearestResult struct** contains:
  - `Entity ent` - Closest entity (entity_nil if none in range)
  - `Vec2 p` - Closest point on shape surface
  - `Scalar d` - Distance to point (negative if inside shape)
  - `Vec2 g` - Gradient of distance function
- **physics_nearest(Vec2 point, Scalar max_dist)** - Finds nearest entity within max distance

## System Management Functions
- **physics_init()** / **physics_deinit()** - System initialization/cleanup
- **physics_update_all()** / **physics_post_update_all()** - Simulation step functions
- **physics_draw_all()** - Debug rendering for all physics objects
- **physics_save_all(Store *s)** / **physics_load_all(Store *s)** - Serialization support

### Prefab System for Entity Serialization (prefab.c)
This section implements a prefab system that allows saving and loading entity hierarchies to/from files. The system provides functionality to serialize entity data and restore it later.

## Key Components

### Static Variables
- `saved_root`: A static Entity variable that temporarily stores the root entity during save/load operations

### Public Functions

- **`prefab_save(const char *filename, Entity root)`**
  - Saves an entity hierarchy to a file
  - Steps:
    1. Sets `saved_root` to the provided root entity
    2. Opens a new store using `store_open()`
    3. Calls `system_save_all()` to serialize all system data
    4. Writes the store data to the specified filename using `store_write_file()`
    5. Closes the store
    6. Resets `saved_root` to `entity_nil`

- **`prefab_load(const char *filename)`**
  - Loads an entity hierarchy from a file
  - Steps:
    1. Opens the specified file as a store using `store_open_file()`
    2. Calls `system_load_all()` to deserialize all system data
    3. Closes the store
    4. Retrieves the root entity from `saved_root`
    5. Resets `saved_root` to `entity_nil`
    6. Returns the loaded root entity

- **`prefab_save_all(Store *s)`**
  - Serializes prefab-specific data to a store
  - Creates a child store section named "prefab"
  - Saves the `saved_root` entity with the key "saved_root"

- **`prefab_load_all(Store *s)`**
  - Deserializes prefab-specific data from a store
  - Loads from a child store section named "prefab"
  - Restores the `saved_root` entity using the key "saved_root"

## Dependencies

### External Headers
- `prefab.h`: Header file for this module
- `system.h`: Provides system-wide save/load functionality

### External Functions Referenced
- `store_open()`, `store_open_file()`, `store_write_file()`, `store_close()`: Store management functions
- `store_child_save()`, `store_child_load()`: Hierarchical store operations
- `system_save_all()`, `system_load_all()`: System-wide serialization functions
- `entity_save()`, `entity_load()`: Entity serialization functions

### External Constants
- `entity_nil`: Represents a null/empty entity value

The prefab system acts as a high-level interface for entity serialization, coordinating between the store system and the entity system to provide complete save/load functionality for entity hierarchies.

### Prefab System Header - Entity Serialization and Deserialization (prefab.h)
This header file defines the interface for a prefab system that handles saving and loading collections of entities to/from files. The prefab system allows for serializing entity hierarchies with a designated root entity.

## Key Components Defined:

• **Script-exported functions** (exposed to scripting system):
  - `prefab_save(const char *filename, Entity root)` - Saves all filtered entities as a prefab file, using the specified `root` entity as the hierarchy root
  - `prefab_load(const char *filename)` - Loads a previously saved prefab file and returns the root `Entity` from the saved data

• **Internal C functions** (not script-exposed):
  - `prefab_save_all(Store *s)` - Saves all prefabs using the provided `Store` object
  - `prefab_load_all(Store *s)` - Loads all prefabs using the provided `Store` object

## Dependencies Referenced:

• **entity.h** - Provides the `Entity` type used throughout the prefab system
• **saveload.h** - Provides the `Store` type used for bulk save/load operations
• **script_export.h** - Provides the `SCRIPT()` macro and `EXPORT` keyword for making functions available to the scripting system

## System Architecture:

• The prefab system operates on two levels:
  - Individual prefab operations (`prefab_save`/`prefab_load`) for single prefab files
  - Bulk operations (`prefab_save_all`/`prefab_load_all`) for managing multiple prefabs through a Store system

• The save operation filters entities before serialization, suggesting selective saving based on some criteria
• The load operation reconstructs the entity hierarchy and returns the root entity for further manipulation

### Save/Load System Implementation with Hierarchical Data Storage (saveload.c)
This file implements a comprehensive save/load system for serializing and deserializing hierarchical data structures to/from files and strings. The system uses a tree-like structure with streams for flexible data storage.

## Core Data Structures

**Stream Structure**
- `Stream`: Growable string buffer with position tracking
  - `buf`: Character buffer for data storage
  - `pos`: Current read/write position
  - `cap`: Allocated buffer capacity

**Store Structure**
- `Store`: Hierarchical data container representing tree nodes
  - `name`: Node identifier string
  - `sm`: Embedded Stream for data storage
  - `compressed`: Boolean flag for compression format
  - `child`, `parent`, `sibling`: Tree navigation pointers
  - `iterchild`: Iterator for unnamed child traversal
  - `str`: Cached string result from `store_get_str()`

## Stream Operations

**Internal Stream Functions**
- `_stream_init()`: Initialize empty stream
- `_stream_deinit()`: Free stream resources
- `_stream_grow()`: Expand buffer capacity as needed
- `_stream_printf()`: Formatted writing with automatic growth
- `_stream_scanf_()` + `_stream_scanf()` macro: Formatted reading with position tracking

**String Serialization**
- `_stream_write_string()`: Serializes strings with quote escaping
  - NULL strings written as "n "
  - Non-NULL strings written as quoted with escaped quotes
- `_stream_read_string_()` + `_stream_read_string()` macro: Deserializes strings with unescaping

## Store Management

**Store Lifecycle**
- `_store_new()`: Create new Store node with parent linking
- `_store_free()`: Recursively free Store tree and resources

**Serialization Formats**
- `_store_write()`: Compact format using `{ }` for normal, `[ ]` for compressed
- `_store_write_pretty()`: Human-readable format with indentation
- `_store_read()`: Parse serialized Store from stream with format detection

## Public API Functions

**Store Creation/Destruction**
- `store_open()`: Create empty root Store
- `store_open_str()`: Parse Store from string
- `store_open_file()`: Load Store from file (format: `<length>\n<data>`)
- `store_write_str()`: Serialize Store to string
- `store_write_file()`: Save Store to file
- `store_close()`: Free entire Store tree

**Child Store Management**
- `store_child_save()`: Create child Store for saving data
- `store_child_save_compressed()`: Create compressed child Store
- `store_child_load()`: Find child Store by name or iterate unnamed children

**Data Type Serialization**
- `scalar_save()`/`scalar_load()`: Save/load Scalar values (handles infinity as 'i')
- `uint_save()`/`uint_load()`: Save/load unsigned integers
- `int_save()`/`int_load()`: Save/load signed integers
- `bool_save()`/`bool_load()`: Save/load boolean values
- `string_save()`/`string_load()`: Save/load strings with memory management

## Key Features

**Hierarchical Structure**
- Tree-based organization with named nodes
- Parent-child relationships maintained via pointers
- Sibling traversal for iteration

**Compression Support**
- Compressed stores flatten hierarchy for space efficiency
- Different serialization brackets distinguish formats

**Error Handling**
- Uses `error()` and `error_assert()` functions for validation
- Detects corrupt save data during parsing

**Memory Management**
- Automatic buffer growth for streams
- Proper cleanup of allocated strings and structures
- String result caching in Store nodes

The system provides a flexible framework for saving/loading game state or configuration data with support for nested structures, multiple data types, and both compact and human-readable formats.

### Save/Load System Header - Data Persistence Interface (saveload.h)
This header file defines a comprehensive save/load system for persisting various data types to and from storage. The system is built around a central `Store` data structure and provides both script-exportable and internal C functions.

## Core Data Structure
- **`Store`**: Opaque struct representing a storage container for save/load operations
  - Forward declared as `typedef struct Store Store`
  - Users must call `*_close()` functions to free resources when done

## Script-Exported Store Operations
The following functions are exported to scripts via the `SCRIPT(saveload, ...)` macro:

### Store Creation and Opening
- **`store_open()`**: Creates an empty store
- **`store_open_str(const char *str)`**: Opens store from string data
- **`store_open_file(const char *filename)`**: Opens store from file

### Store Writing and Cleanup
- **`store_write_str(Store *s)`**: Serializes store to string
- **`store_write_file(Store *s, const char *filename)`**: Writes store to file
- **`store_close(Store *s)`**: Closes store and frees resources

## Hierarchical Store Management
Support for nested store structures for backwards-compatible save/load:
- **`store_child_save(Store **sp, const char *name, Store *parent)`**: Creates child store
- **`store_child_save_compressed(Store **sp, const char *name, Store *parent)`**: Creates compressed child store
- **`store_child_load(Store **sp, const char *name, Store *parent)`**: Loads child store

## Type-Specific Save/Load Functions
All save/load functions follow a consistent pattern:
- **Save functions**: Take pointer to value, name string, and store
- **Load functions**: Take pointer to destination, name string, default value, and store
- **Load return value**: `bool` indicating whether value was found (true) or defaulted (false)

### Supported Data Types
- **`Scalar`**: Custom scalar type (depends on "scalar.h")
  - `scalar_save(const Scalar *f, const char *n, Store *s)`
  - `scalar_load(Scalar *f, const char *n, Scalar d, Store *s)`

- **`unsigned int`**: Unsigned integers
  - `uint_save(const unsigned int *u, const char *n, Store *s)`
  - `uint_load(unsigned int *u, const char *n, unsigned int d, Store *s)`

- **`int`**: Signed integers
  - `int_save(const int *i, const char *n, Store *s)`
  - `int_load(int *i, const char *n, int d, Store *s)`

- **`bool`**: Boolean values
  - `bool_save(const bool *b, const char *n, Store *s)`
  - `bool_load(bool *b, const char *n, bool d, Store *s)`

- **`char *`**: String data
  - `string_save(const char **c, const char *n, Store *s)`
  - `string_load(char **c, const char *n, const char *d, Store *s)`
  - Note: Caller must `free(*c)` after string_load

### Enum Support via Macros
- **`enum_save(val, n, s)`**: Macro that casts enum to int and saves
- **`enum_load(val, n, d, s)`**: Macro that loads as int and casts back to enum

## Dependencies
- **`<stdbool.h>`**: For boolean type support
- **`"scalar.h"`**: For custom Scalar type definition
- **`"script_export.h"`**: For SCRIPT macro and export functionality

## Key Design Patterns
- Consistent naming convention with type prefix
- Uniform parameter ordering: value, name, store
- Memory management responsibility clearly documented
- Support for default values in load operations
- Backwards compatibility through hierarchical store trees

### Scalar Type Definition and Mathematical Function Mappings (scalar.h)
This header file defines a standardized scalar type and mathematical function interface for the codebase, providing abstraction over floating-point operations and constants.

## Key Definitions

### Type Definition
- **`Scalar`**: Typedef that maps to `float` type, defined within a `SCRIPT` macro block for script export functionality

### Mathematical Constants
- **`SCALAR_PI`**: Defines π (pi) constant
  - Uses `M_PI` if available from math.h
  - Falls back to hardcoded value `3.14159265358979323846264338327950288` if `M_PI` is not defined
- **`SCALAR_INFINITY`**: Maps to the standard `INFINITY` constant
- **`SCALAR_EPSILON`**: Maps to `FLT_EPSILON` for floating-point precision comparisons

### Function Mappings
The file creates scalar-prefixed aliases for standard math library functions, specifically mapping to their float variants:

#### Trigonometric Functions
- **`scalar_cos`**: Maps to `cosf` (cosine)
- **`scalar_sin`**: Maps to `sinf` (sine)  
- **`scalar_atan2`**: Maps to `atan2f` (arctangent of y/x)

#### Utility Functions
- **`scalar_sqrt`**: Maps to `sqrtf` (square root)
- **`scalar_min`**: Maps to `fminf` (minimum of two values)
- **`scalar_max`**: Maps to `fmaxf` (maximum of two values)
- **`scalar_floor`**: Maps to `floor` (floor function)

## Dependencies
- **Standard Libraries**: `math.h` and `float.h` for mathematical functions and constants
- **Project Dependencies**: `script_export.h` for the `SCRIPT` macro functionality

## Design Purpose
This abstraction layer allows the codebase to:
- Maintain consistent scalar type usage across the project
- Easily switch underlying scalar precision (currently float-based)
- Provide a unified interface for mathematical operations
- Enable script export functionality for the scalar type

### Scratch File Monitor and Executor (scratch.c)
This section implements a file monitoring and execution system for a Lua scratch file, providing automatic reloading capabilities when the file is modified.

## Key Components Defined

**Static Variables:**
- `filename` - Static string set to `usr_path("scratch.lua")`, representing the path to the scratch file

**Helper Functions:**
- `_exists()` - Checks if the scratch file exists using `stat()` system call
- `_modified()` - Tracks whether the file has been modified since the last check using modification timestamps

**Public Interface Functions:**
- `scratch_run()` - Executes the scratch Lua file
- `scratch_update()` - Conditionally runs the scratch file if it has been modified

## Dependencies and References

**External Dependencies:**
- `scratch.h` - Header file for this module
- `dirs.h` - Provides `usr_path()` function for path resolution
- `script.h` - Provides `script_run_file()` function for Lua execution
- Standard libraries: `stdbool.h`, `sys/stat.h`, `time.h`

## Algorithm Details

**File Modification Tracking (`_modified()`):**
- Maintains static variables to track state:
  - `prev_time` - Previous modification timestamp (initialized to 0)
  - `first` - Boolean flag to handle first call (initialized to true)
- Logic flow:
  1. Attempts to get file stats using `stat()`
  2. If `stat()` fails (file doesn't exist or error), returns false and clears first flag
  3. If successful, compares current modification time (`st.st_mtime`) with previous time
  4. Returns true only if not the first call AND modification time has changed
  5. Updates `prev_time` with current modification time and clears first flag

**Update Mechanism (`scratch_update()`):**
- Simple conditional execution: calls `scratch_run()` only if `_modified()` returns true
- Provides automatic reload functionality for development workflows

**Execution (`scratch_run()`):**
- Delegates to `script_run_file()` with the scratch file path
- Provides a simple wrapper for executing the monitored Lua script

This module enables hot-reloading of a user's scratch Lua file, commonly used in development environments for rapid prototyping and testing.

### Scratch Module Header Declaration (scratch.h)
This is a header file that declares the public interface for a "scratch" module. The file follows standard C/C++ header guard conventions and defines a minimal API with two functions.

**Header Guard:**
• Uses `#ifndef SCRATCH_H` / `#define SCRATCH_H` / `#endif` pattern to prevent multiple inclusions

**Function Declarations:**
• `scratch_run()` - A function that takes no parameters and returns void
• `scratch_update()` - A function that takes no parameters and returns void

**Key Characteristics:**
• Pure function declarations with no implementation details
• No dependencies on other headers or external entities
• Minimal interface suggesting this might be a utility or testing module
• Function names suggest a pattern where one function runs/initializes something and another updates it

**Usage Context:**
This header would typically be included by other source files that need to call these scratch functions, while the actual implementation would be in a corresponding `scratch.c` or `scratch.cpp` file.

### Lua Script Engine Integration and Event System (script.c)
This file implements a Lua scripting engine that provides the core bridge between the C game engine and Lua scripts, handling initialization, event dispatching, and FFI bindings.

## Core Components

### Global State and Error Handling
- **L**: Static lua_State pointer that maintains the global Lua interpreter instance
- **errcheck**: Macro that handles Lua error checking, prints errors to console via `console_printf`, and cleans up the Lua stack
- **_traceback**: Internal function that provides enhanced error reporting with stack traces for Lua errors
- **_pcall**: Protected call wrapper that adds traceback functionality to standard lua_pcall

### Script Execution Functions
- **script_run_string**: Executes Lua code from a string parameter
- **script_run_file**: Loads and executes Lua code from a file
- **script_error**: Triggers a Lua error with specified message

### Data Exchange System
- **_push_cdata**: Pushes C data structures to Lua as FFI cdata objects
  - Takes a type specifier string and void pointer
  - Calls `cg.__deref_cdata(t, p)` to convert C pointers to Lua cdata
  - Example: `_push_cdata("Vec2 *", &v)` creates a Vec2 cdata on the Lua stack

### Event System
- **_push_event**: Helper function that prepares event firing by calling `cg.__fire_event`
- **Event handler functions** that bridge C events to Lua:
  - `script_key_down` / `script_key_up`: Keyboard events with KeyCode parameters
  - `script_mouse_down` / `script_mouse_up`: Mouse button events with MouseCode parameters  
  - `script_mouse_move`: Mouse movement with Vec2 position
  - `script_scroll`: Scroll wheel events with Vec2 scroll data
  - `script_update_all` / `script_post_update_all` / `script_draw_all`: Game loop events

### Initialization System
- **_forward_args**: Copies command line arguments to Lua global `cgame_args` table
  - Uses `game_get_argc()` and `game_get_argv()` to access arguments
- **_set_paths**: Sets up path globals for Lua scripts
  - `cgame_data_path`: Set via `data_path("")`
  - `cgame_usr_path`: Set via `usr_path("")`
- **_fix_exports**: String processing function that replaces "EXPORT" keywords with whitespace for FFI compatibility
- **_load_cgame_ffi**: Sets up FFI bindings for C interop
  - Requires the 'ffi' module
  - Accumulates all `cgame_ffi` definitions into a buffer
  - Processes exports and calls `ffi.cdef()` with the combined definitions

### Lifecycle Management
- **script_init**: Complete initialization sequence
  - Creates Lua state with `lua_open()` and `luaL_openlibs()`
  - Loads FFI bindings, forwards args, sets paths
  - Executes `data_path("script/main.lua")`
  - Fires "init" event
- **script_deinit**: Cleanup sequence
  - Fires "deinit" event
  - Closes Lua state with `lua_close()`

### Save/Load System
- **script_save_all**: Serializes Lua state to Store
  - Calls `cg.__save_all()` to get serialized string from Lua
  - Saves string to child store named "script"
- **script_load_all**: Deserializes Lua state from Store
  - Loads string from "script" child store
  - Calls `cg.__load_all(str)` to restore Lua state

## Key Dependencies
- **cgame_ffi.h**: Provides FFI definitions array (`cgame_ffi`, `n_cgame_ffi`)
- **Lua libraries**: lua.h, lauxlib.h, lualib.h for core Lua functionality
- **Game modules**: dirs.h, saveload.h, game.h, input.h, console.h for engine integration
- **Lua-side dependencies**: Expects `cg` global table with `__deref_cdata`, `__fire_event`, `__save_all`, `__load_all` functions

### Script System Interface Header (script.h)
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

### Script Export Header - Lua Integration and Export Macros (script_export.h)
This header file provides macros for integrating C code with Lua scripting and handling function exports across different platforms.

## Key Macros Defined

### SCRIPT(name, ...)
- **Purpose**: Makes C declarations visible from Lua while preserving normal C functionality
- **Conditional behavior based on `__CGAME_FFI__` preprocessor flag**:
  - **When `__CGAME_FFI__` is defined**: 
    - Creates a static string constant named `cgame_ffi_##name` containing the stringified version of the declarations
    - Also includes the original declarations unchanged
  - **When `__CGAME_FFI__` is not defined**:
    - Simply expands to the original declarations without modification
- **Requirements**:
  - Each `SCRIPT(...)` usage must have a unique `name` parameter
  - Users must also edit `cgame_ffi.h` (referenced as having explanatory comments)
- **Reference**: Points to `transform.h` as an example of usage

### EXPORT
- **Purpose**: Ensures function visibility from LuaJIT across different compilers
- **Platform-specific behavior**:
  - **MSVC (`_MSC_VER` defined)**: Expands to `__declspec(dllexport)` to explicitly export functions
  - **Other compilers**: Expands to nothing (empty), relying on default export behavior

## Dependencies and References
- **cgame_ffi.h**: Must be edited in conjunction with using `SCRIPT()` macro
- **transform.h**: Provided as example implementation
- **LuaJIT**: The target scripting engine for exported functions

## Usage Pattern
The file enables a dual-purpose approach where:
- C declarations can be used normally in C code through standard `#include`
- The same declarations are automatically made available to Lua when properly configured
- Functions are properly exported for dynamic linking regardless of compiler

### Sound System Implementation (sound.c)
This section implements a complete sound system for managing audio playback in a game engine using the Gorilla Audio library. The system provides entity-based sound management with features like looping, seeking, volume control, and automatic cleanup.

## Core Data Structures

• **Sound struct**: Main sound entity containing:
  - `EntityPoolElem pool_elem`: Entity pool management
  - `char *path`: File path to sound file
  - `ga_Handle *handle`: Gorilla Audio handle for playback control
  - `gau_SampleSourceLoop *loop_src`: Loop source for repeat functionality
  - `bool finish_destroy`: Flag to auto-destroy when playback finishes
  - `bool loop`: Loop enable/disable flag

• **Static global variables**:
  - `EntityPool *pool`: Entity pool for managing Sound instances
  - `gau_Manager *mgr`: Gorilla Audio manager
  - `ga_Mixer *mixer`: Audio mixer
  - `ga_StreamManager *stream_mgr`: Stream manager for file playback

## Internal Helper Functions

• **_release(Sound *sound)**: Cleanup function that:
  - Frees the path string
  - Destroys the Gorilla Audio handle
  - Sets pointers to NULL

• **_format(const char *path)**: Format detection that:
  - Scans path string for last dot character
  - Returns file extension as format string
  - Errors if no extension found

• **_update_loop(Sound *sound)**: Loop state synchronization that:
  - Sets infinite loop (-1) if sound->loop is true
  - Clears loop if sound->loop is false

• **_set_path(Sound *sound, const char *path)**: Core sound loading function that:
  - Saves current playing state
  - Determines audio format from file extension
  - Creates appropriate handle (buffered for OGG, direct for others)
  - Releases old resources and sets new path
  - Updates loop state
  - Restores playing state if previously playing

## Public API Functions

### Entity Management
• **sound_add(Entity ent)**: Adds sound component to entity with default "default.wav" file
• **sound_remove(Entity ent)**: Removes sound component and cleans up resources
• **sound_has(Entity ent)**: Checks if entity has sound component

### Sound Properties
• **sound_set_path/sound_get_path**: Set/get sound file path
• **sound_set_playing/sound_get_playing**: Control/query playback state
• **sound_set_seek/sound_get_seek**: Control/query playback position
• **sound_set_finish_destroy/sound_get_finish_destroy**: Control auto-destruction behavior
• **sound_set_loop/sound_get_loop**: Control/query loop behavior
• **sound_set_gain/sound_get_gain**: Control/query volume level

## System Management Functions

• **sound_init()**: System initialization that:
  - Initializes Gorilla Audio (`gc_initialize`)
  - Creates manager, mixer, and stream manager
  - Creates entity pool

• **sound_deinit()**: System cleanup that:
  - Releases all sound resources
  - Frees entity pool
  - Destroys Gorilla Audio manager
  - Shuts down Gorilla Audio

• **sound_update_all()**: Per-frame update that:
  - Destroys finished sounds with `finish_destroy` flag set
  - Removes destroyed entities from pool
  - Updates Gorilla Audio manager

## Persistence System

• **sound_save_all(Store *s)**: Serializes all sound state including:
  - File paths and configuration flags
  - Current playing state and seek position
  - Volume gain settings

• **sound_load_all(Store *s)**: Deserializes sound state and:
  - Recreates sound handles from saved paths
  - Restores playback state and position
  - Reapplies volume settings

## Key Dependencies

• **Gorilla Audio library**: `ga.h` and `gau.h` for audio playback
• **entitypool**: For entity-component system management
• **dirs**: For `data_path()` function to locate default sounds
• **error**: For error handling and assertions
• **Store system**: For save/load functionality

The system supports OGG and WAV formats, with OGG files using buffered streaming and other formats loaded directly into memory.

### Sound System Entity Component Header (sound.h)
This header file defines the sound system component interface for managing audio entities in what appears to be an entity-component-system (ECS) architecture.

## Key Dependencies
- `entity.h` - Provides the `Entity` type used throughout the interface
- `script_export.h` - Provides the `SCRIPT` macro and `EXPORT` functionality for script bindings

## Script-Exported Functions (Public API)
The following functions are exported to scripting systems via the `SCRIPT` macro:

### Entity Management
- `sound_add(Entity ent)` - Adds sound component to an entity
- `sound_remove(Entity ent)` - Removes sound component from an entity  
- `sound_has(Entity ent)` - Checks if entity has sound component

### Audio File Management
- `sound_set_path(Entity ent, const char *path)` - Sets audio file path
- `sound_get_path(Entity ent)` - Gets audio file path

### Playback Control
- `sound_set_playing(Entity ent, bool playing)` - Controls play/pause state
- `sound_get_playing(Entity ent)` - Gets current play/pause state
- `sound_set_seek(Entity ent, int seek)` - Sets playback position
- `sound_get_seek(Entity ent)` - Gets current playback position

### Playback Behavior
- `sound_set_finish_destroy(Entity ent, bool finish_destroy)` - Sets whether entity should be destroyed when audio finishes
- `sound_get_finish_destroy(Entity ent)` - Gets finish_destroy setting
- `sound_set_loop(Entity ent, bool loop)` - Sets looping behavior
- `sound_get_loop(Entity ent)` - Gets looping setting

### Audio Properties
- `sound_set_gain(Entity ent, Scalar gain)` - Sets audio volume/gain level
- `sound_get_gain(Entity ent)` - Gets current gain level

## System Management Functions (Internal API)
These functions are not exported to scripts and handle system-level operations:

- `sound_init()` - Initializes the sound system
- `sound_deinit()` - Cleans up the sound system
- `sound_update_all()` - Updates all sound entities (likely called each frame)
- `sound_save_all(Store *s)` - Serializes all sound components to storage
- `sound_load_all(Store *s)` - Deserializes sound components from storage

## Referenced Types
- `Entity` - Entity identifier type from entity system
- `Scalar` - Floating-point type for gain values
- `Store` - Storage/serialization system type

The header follows standard C practices with include guards (`#ifndef SOUND_H`) and provides a complete interface for managing audio components in an entity-based game or application system.

### Sprite Component System Implementation (sprite.c)
This file implements a complete sprite rendering system for a 2D game engine using OpenGL and an entity-component-system (ECS) architecture.

## Core Data Structure
- **`Sprite` struct**: Contains sprite properties for each entity
  - `pool_elem`: EntityPoolElem for ECS integration
  - `wmat`: Mat3 world transform matrix for shader rendering
  - `size`: Vec2 sprite dimensions in world space
  - `texcell`: Vec2 texture coordinates for sprite position in atlas
  - `texsize`: Vec2 texture dimensions for sprite size in atlas
  - `depth`: int for depth sorting (higher values render in front)

## Global State Management
- **`pool`**: EntityPool pointer managing all sprite entities
- **`atlas`**: char pointer to current texture atlas filename
- **OpenGL objects**: `program` (shader), `vao` (vertex array), `vbo` (vertex buffer)

## Atlas Management Functions
- **`_set_atlas(filename, err)`**: Internal function to load texture atlas
  - Loads texture using `texture_load()`
  - Updates atlas filename string
  - Sets atlas_size uniform in shader
  - `err` parameter controls error handling behavior
- **`sprite_set_atlas(filename)`**: Public wrapper that enables error reporting
- **`sprite_get_atlas()`**: Returns current atlas filename

## Entity Component Management
- **`sprite_add(Entity ent)`**: Adds sprite component to entity
  - Checks if sprite already exists to prevent duplicates
  - Automatically adds transform component dependency
  - Initializes sprite with default values (size: 1x1, texcell: 32x32, texsize: 32x32, depth: 0)
- **`sprite_remove(Entity ent)`**: Removes sprite component from entity
- **`sprite_has(Entity ent)`**: Checks if entity has sprite component

## Property Accessor Functions
Each sprite property has getter/setter pair with entity validation:
- **Size**: `sprite_set_size()` / `sprite_get_size()`
- **Texture cell**: `sprite_set_texcell()` / `sprite_get_texcell()`
- **Texture size**: `sprite_set_texsize()` / `sprite_get_texsize()`
- **Depth**: `sprite_set_depth()` / `sprite_get_depth()`

## System Lifecycle Functions
- **`sprite_init()`**: Initializes sprite system
  - Creates entity pool
  - Loads shader program from sprite.vert, sprite.geom, sprite.frag files
  - Sets up OpenGL vertex array and buffer objects
  - Binds vertex attributes for world matrix, size, and texture properties
  - Loads default atlas from "default.png"
- **`sprite_deinit()`**: Cleanup function
  - Deletes OpenGL objects
  - Frees entity pool and atlas string

## Update and Rendering Pipeline
- **`sprite_update_all()`**: Updates all sprites per frame
  - Removes destroyed entities from pool
  - Updates world transform matrices from transform component
  - Updates edit mode bounding boxes for sprite selection
- **`_depth_compare(a, b)`**: Comparison function for depth sorting
  - Sorts by depth descending (higher depth renders first)
  - Breaks ties using entity ID for stable sorting
- **`sprite_draw_all()`**: Renders all sprites
  - Depth sorts sprites using `_depth_compare()`
  - Binds shader program and updates camera uniforms
  - Binds texture atlas
  - Uploads sprite data to GPU and draws as point primitives

## Save/Load System
- **`sprite_save_all(Store *s)`**: Serializes all sprite data
  - Saves atlas filename
  - Saves each sprite's size, texcell, texsize, and depth properties
- **`sprite_load_all(Store *s)`**: Deserializes sprite data
  - Loads atlas (with fallback to current if not found)
  - Loads sprite properties with default fallback values

## Key Dependencies
- **EntityPool system**: For ECS entity management
- **Transform component**: Required dependency for world positioning
- **Texture system**: For atlas loading and binding
- **Camera system**: For view matrix uniforms
- **Edit system**: For selection bounding boxes
- **Save/Load system**: For persistence
- **OpenGL/GLEW**: For graphics rendering

### Sprite System Header - 2D Rendering Component Interface (sprite.h)
This header file defines a comprehensive sprite system for 2D rendering in what appears to be an entity-component-system (ECS) architecture. The system manages textured sprites that can be attached to entities and rendered from a texture atlas.

## Key Dependencies
- `saveload.h` - For serialization functionality
- `entity.h` - For Entity type definitions
- `vec2.h` - For Vec2 vector type
- `script_export.h` - For SCRIPT and EXPORT macros

## Exported Script Interface
The file uses a `SCRIPT(sprite, ...)` macro to define functions that are exported to a scripting system:

### Atlas Management
- `sprite_set_atlas(const char *filename)` - Sets the texture atlas file
- `sprite_get_atlas()` - Returns the current atlas filename

### Entity Component Management
- `sprite_add(Entity ent)` - Adds sprite component to an entity
- `sprite_remove(Entity ent)` - Removes sprite component from an entity
- `sprite_has(Entity ent)` - Checks if entity has a sprite component

### Sprite Properties
- **Size Control**:
  - `sprite_set_size(Entity ent, Vec2 size)` - Sets world-space size for drawing, centered at transform position
  - `sprite_get_size(Entity ent)` - Gets the sprite's world-space size

- **Texture Mapping**:
  - `sprite_set_texcell(Entity ent, Vec2 texcell)` - Sets bottom-left corner of atlas region in pixels
  - `sprite_get_texcell(Entity ent)` - Gets the texture cell position
  - `sprite_set_texsize(Entity ent, Vec2 texsize)` - Sets size of atlas region in pixels
  - `sprite_get_texsize(Entity ent)` - Gets the texture region size

- **Depth/Layering**:
  - `sprite_set_depth(Entity ent, int depth)` - Sets rendering depth (lower values drawn on top)
  - `sprite_get_depth(Entity ent)` - Gets the sprite's depth value

## Internal System Functions
These functions are not exported to scripts and handle system lifecycle:

- `sprite_init()` - Initializes the sprite system
- `sprite_deinit()` - Cleans up the sprite system
- `sprite_update_all()` - Updates all sprite components
- `sprite_draw_all()` - Renders all sprites
- `sprite_save_all(Store *s)` - Serializes all sprite data
- `sprite_load_all(Store *s)` - Deserializes sprite data

## Architecture Notes
- Uses texture atlas approach for efficient rendering
- Supports depth-based layering with integer depth values
- Integrates with a transform system (sprites are centered at transform position)
- Provides full serialization support for save/load functionality
- Designed as an ECS component that can be dynamically added/removed from entities

### System Manager - Core Game Engine Initialization and Main Loop (system.c)
This file implements the central system manager that coordinates all subsystems of the game engine. It serves as the main orchestrator for initialization, update cycles, rendering, and persistence operations.

## Key Functions Defined

• **system_init()** - Initializes all engine subsystems in proper dependency order
• **system_deinit()** - Cleans up all subsystems in reverse order
• **system_update_all()** - Executes the main game loop update cycle
• **system_draw_all()** - Handles all rendering operations
• **system_save_all(Store *s)** - Saves all subsystem state to storage
• **system_load_all(Store *s)** - Loads all subsystem state from storage

## Input Event Forwarding

The system implements input event delegation through static callback functions:

• **_key_down(KeyCode key)** - Forwards keyboard press events to GUI and script systems
• **_key_up(KeyCode key)** - Forwards keyboard release events to GUI and script systems
• **_char_down(unsigned int c)** - Forwards character input to GUI system
• **_mouse_down(MouseCode mouse)** - Forwards mouse press events to GUI and script systems
• **_mouse_up(MouseCode mouse)** - Forwards mouse release events to GUI and script systems
• **_mouse_move(Vec2 pos)** - Forwards mouse movement to script system
• **_scroll(Vec2 scroll)** - Forwards scroll events to script system

## Subsystem Dependencies

The system manages these subsystems in initialization order:
1. **input** - Input handling system
2. **entity** - Entity management system
3. **transform** - Transform/positioning system
4. **camera** - Camera system
5. **texture** - Texture management
6. **sprite** - Sprite rendering
7. **gui** - GUI system
8. **console** - Console/debugging
9. **sound** - Audio system
10. **physics** - Physics simulation
11. **edit** - Editor functionality
12. **script** - Scripting system

## Main Update Loop Sequence

**system_update_all()** executes subsystem updates in this order:
1. **edit_clear()** - Clear editor state
2. **timing_update()** - Update timing/frame information
3. **texture_update()** and **scratch_update()** - Update texture and scratch memory systems
4. **script_update_all()** - Execute script updates
5. **keyboard_controlled_update_all()** - Update keyboard-controlled entities
6. **physics_update_all()** - Update physics simulation
7. **transform_update_all()** - Update transforms
8. **camera_update_all()** - Update cameras
9. **gui_update_all()** - Update GUI elements
10. **sprite_update_all()** - Update sprites
11. **sound_update_all()** - Update audio
12. **edit_update_all()** - Update editor
13. **script_post_update_all()** and **physics_post_update_all()** - Post-update phases
14. **entity_update_all()** - Final entity updates
15. **gui_event_clear()** - Clear GUI events

## Rendering Pipeline

**system_draw_all()** renders in this order:
1. **script_draw_all()** - Script-driven rendering
2. **sprite_draw_all()** - Sprite rendering
3. **edit_draw_all()** - Editor overlay rendering
4. **physics_draw_all()** - Physics debug rendering
5. **gui_draw_all()** - GUI rendering (topmost layer)

## Save/Load System

The **_saveload_all()** helper function ensures consistent save/load ordering:
• Uses a macro pattern to call either save or load functions based on a boolean flag
• Maintains strict ordering: entity, prefab, timing, transform, camera, sprite, physics, gui, edit, sound, keyboard_controlled, script
• Wraps operations with **entity_load_all_begin()** and **entity_load_all_end()** for proper entity lifecycle management

## External Dependencies

• References **Store** type for persistence operations
• Uses **Vec2** type for 2D vector operations (mouse positions, scroll)
• Depends on **KeyCode** and **MouseCode** enums for input handling
• Includes test system **keyboard_controlled** for keyboard-controlled entities

### System Module Header - Core System Management Interface (system.h)
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

### Test Directory - Keyboard Control System and Scene Setup (test)
This directory contains a complete keyboard control system implementation and test scene setup for an entity-component-system (ECS) based game engine.

## Core Components

### Keyboard Control System (`keyboard_controlled.c` and `keyboard_controlled.h`)

**Architecture:**
- Single-entity keyboard control system using a boolean flag approach
- Global state variables: `kc_exists` (boolean flag) and `kc_entity` (current controlled entity)

**Public Interface Functions:**
- `keyboard_controlled_add(Entity ent)` - Adds keyboard control to an entity and ensures transform component
- `keyboard_controlled_remove(Entity ent)` - Removes keyboard control from specified entity
- `keyboard_controlled_has(Entity ent)` - Checks if entity has keyboard control (returns bool)

**Update Logic (`keyboard_controlled_update_all()`):**
- **Precondition checks:**
  - Removes destroyed entities automatically
  - Skips if game is paused (`timing_get_paused()`)
  - Skips if GUI has focus (`gui_has_focus()`)
- **Movement controls (Arrow keys):**
  - LEFT/RIGHT: X-axis movement at 5 units/second
  - UP/DOWN: Y-axis movement at 5 units/second
  - Movement is rotation-relative via `vec2_rot()`
- **Rotation controls:**
  - N key: Counterclockwise rotation at 0.35π radians/second
  - M key: Clockwise rotation at 0.35π radians/second
- **Scaling controls:**
  - K key: Increase scale at 12 units/second
  - I key: Decrease scale at 12 units/second (with minimum size protection)
  - Maintains aspect ratio by preserving Y/X scale ratio

**Save/Load System:**
- `keyboard_controlled_save_all(Store *s)` - Saves to "keyboard_controlled" store section
- `keyboard_controlled_load_all(Store *s)` - Loads and restores existence flag

**Key Dependencies:**
- `input.h` - For `input_key_down()` keyboard detection
- `transform.h` - For entity transformation operations
- `timing.h` - For delta time and pause state
- `gui.h` - For focus management
- `saveload.h` - For persistence operations
- `entity.h` - For Entity type and lifecycle management

### Test Scene Setup (`test.c` and `test.h`)

**Function Definitions:**
- `test_c()` - Main scene setup function creating complete game scene
- `test_init()` - Empty initialization function (no implementation)

**Entity Creation Logic:**
- **Camera Entity:**
  - Created with `entity_create()`
  - Components: `transform_add()`, `camera_add()`
  - Receives keyboard control via `keyboard_controlled_add()`

- **Block Entities (Randomly Generated):**
  - Random count: 0-49 blocks using `rand() % 50`
  - Each block:
    - Created with `entity_create()`
    - Transform with random position: x(-12 to 12), y(-4 to 4)
    - Sprite component with texture cell (32.0f, 32.0f) and size (32.0f, 32.0f)

- **Player Entity:**
  - Created with `entity_create()`
  - Transform positioned at origin (0.0f, 0.0f)
  - Sprite component with texture cell (0.0f, 32.0f) and size (32.0f, 32.0f)

**Referenced Systems:**
- Entity system: `entity_create()`, `Entity` type
- Transform system: `transform_add()`, `transform_set_position()`, `vec2()`
- Sprite system: `sprite_add()`, `sprite_set_texcell()`, `sprite_set_texsize()`
- Camera system: `camera_add()`

## Script Integration

The keyboard control system integrates with a scripting system via:
- `SCRIPT` macro defining script-exportable functions
- `EXPORT` functionality for runtime script access
- Script-accessible functions: `keyboard_controlled_add()`, `keyboard_controlled_remove()`, `keyboard_controlled_has()`

## Design Notes

- The implementation uses a boolean flag approach rather than `entity_nil` for non-existent entities (noted as simpler but potentially less robust)
- Camera receives keyboard control instead of player entity
- System follows typical ECS patterns with add/remove/has operations per entity
- Supports complete save/load functionality for game state persistence

### OpenGL Texture Loading and Management System (texture.c)
This section implements a comprehensive texture management system for OpenGL applications, providing functionality to load, cache, and bind textures from image files with automatic hot-reloading capabilities.

## Core Data Structure

• **`Texture` struct**: Contains texture metadata and OpenGL state
  - `filename`: Path to the texture file
  - `gl_name`: OpenGL texture ID (0 if uninitialized or error)
  - `width`, `height`, `components`: Image dimensions and channel count
  - `last_modified`: File modification timestamp for hot-reloading

• **`textures`**: Global static `Array` storing all loaded textures

## Key Functions

### Public API Functions

• **`texture_init()`**: Initializes the texture system by creating the textures array
• **`texture_deinit()`**: Cleanup function that frees all texture filenames and the textures array
• **`texture_load(const char *filename)`**: 
  - Loads a texture from file or returns existing one if already loaded
  - Returns `bool` indicating success/failure
  - Creates new `Texture` entry if not found, then calls `_load()`
• **`texture_bind(const char *filename)`**: Binds the specified texture for OpenGL rendering
• **`texture_get_size(const char *filename)`**: Returns texture dimensions as `Vec2`
• **`texture_update()`**: Updates all textures by calling `_load()` on each (enables hot-reloading)

### Internal Helper Functions

• **`_find(const char *filename)`**: 
  - Searches the textures array for a texture with matching filename
  - Returns pointer to `Texture` or `NULL` if not found

• **`_load(Texture *tex)`**: Core texture loading logic
  - Checks file modification time using `stat()` to avoid unnecessary reloads
  - Uses `stbi_load()` from stb_image library to read image data
  - Handles OpenGL texture creation and configuration:
    - Generates new OpenGL texture ID with `glGenTextures()`
    - Sets texture parameters (GL_NEAREST filtering)
    - Flips image vertically before uploading to OpenGL
    - Uploads texture data with `glTexImage2D()` using RGBA format
  - Updates modification timestamp
  - Provides console logging for load attempts

• **`_flip_image_vertical(unsigned char *data, unsigned int width, unsigned int height)`**: 
  - Flips image data vertically to match OpenGL's coordinate system
  - Allocates temporary buffer and copies rows in reverse order
  - Assumes 4 components per pixel (RGBA)

## Dependencies

• **External libraries**: `GL/glew.h`, `stb_image.h`
• **System libraries**: `string.h`, `stdlib.h`, `sys/stat.h`, `time.h`
• **Internal modules**: `error.h`, `array.h`, `console.h`, `texture.h`

## Key Features

• **Hot-reloading**: Automatically detects file changes and reloads textures
• **Caching**: Prevents duplicate loading of the same texture file
• **Error handling**: Gracefully handles missing files and load failures
• **Memory management**: Properly cleans up OpenGL resources and allocated memory
• **Console logging**: Provides feedback on texture loading operations

### Texture Management System Header (texture.h)
This header file defines the public interface for a texture management system that handles loading, binding, and querying texture resources.

## Function Definitions

The header declares the following functions for texture operations:

• **`texture_load(const char *filename)`** - Loads a texture from a file
  - Takes a filename string as input
  - Returns a boolean indicating success/failure

• **`texture_bind(const char *filename)`** - Binds a previously loaded texture for use
  - Takes a filename string to identify which texture to bind
  - Likely used in rendering contexts to set the active texture

• **`texture_get_size(const char *filename)`** - Retrieves texture dimensions
  - Takes a filename string to identify the texture
  - Returns a `Vec2` containing width and height values

• **`texture_init()`** - Initializes the texture system
  - No parameters
  - Likely sets up internal data structures and resources

• **`texture_deinit()`** - Cleans up the texture system
  - No parameters
  - Likely frees allocated memory and resources

• **`texture_update()`** - Updates the texture system
  - No parameters
  - Purpose unclear from interface alone, possibly handles pending operations

## Dependencies

The header has one external dependency:
• **`vec2.h`** - Required for the `Vec2` type used by `texture_get_size()`

## Design Notes

• The API uses filename strings as texture identifiers, suggesting an internal registry or cache system
• The separation of load and bind operations indicates a typical graphics programming pattern where textures are loaded once and bound as needed
• The boolean return from `texture_load()` provides error handling capability
• The init/deinit pattern suggests this is a managed subsystem requiring explicit lifecycle management

### Timing System Implementation (timing.c)
This section implements a timing system that manages delta time calculations with pause and scaling functionality for game/simulation timing control.

## Global Variables Defined
- `timing_dt`: Current scaled delta time (public)
- `timing_true_dt`: Actual unscaled delta time (public)
- `scale`: Time scaling factor (static, default 1.0f)
- `paused`: Pause state flag (static, default false)

## Functions Defined

### Time Scale Management
- `timing_set_scale(Scalar s)`: Sets the time scaling factor
- `timing_get_scale()`: Returns the current time scaling factor

### Pause Control
- `timing_set_paused(bool p)`: Sets the pause state
- `timing_get_paused()`: Returns the current pause state

### Core Timing Logic
- `_dt_update()`: Internal delta time calculation function
  - Uses static variable `last_time` to track previous frame time
  - On first call (when `last_time < 0`), initializes `last_time` with current time
  - Calculates `timing_true_dt` as difference between current and last time
  - Sets `timing_dt` to 0 if paused, otherwise scales `timing_true_dt` by `scale`
  - Updates `last_time` for next frame

- `timing_update()`: Public interface that calls `_dt_update()`

### Save/Load System
- `timing_save_all(Store *s)`: Saves timing state to storage
  - Creates child store named "timing"
  - Saves `scale` value with key "scale"
- `timing_load_all(Store *s)`: Loads timing state from storage
  - Loads from child store named "timing"
  - Loads `scale` value with key "scale" and default value 1

## Key Dependencies
- `timing.h`: Header file for timing system declarations
- `glew_glfw.h`: For `glfwGetTime()` function used in time measurement
- `Store` type: Used for save/load functionality
- `Scalar` type: Used for time values
- `store_child_save()`, `store_child_load()`: Storage system functions
- `scalar_save()`, `scalar_load()`: Scalar value persistence functions

## Algorithm Overview
The timing system works by:
1. Tracking real elapsed time between frames using GLFW's time function
2. Applying pause logic (setting delta time to 0 when paused)
3. Applying time scaling to create gameplay effects (slow motion, fast forward)
4. Providing both scaled and unscaled delta time values for different use cases

### Timing System Header - Game Time Management Interface (timing.h)
This header file defines the timing system interface for managing game time, including delta time tracking, time scaling, and pause functionality.

## External Dependencies
- `stdbool.h` - For boolean type support
- `scalar.h` - For `Scalar` type definitions
- `script_export.h` - For `SCRIPT` and `EXPORT` macros
- `saveload.h` - For `Store` type used in save/load operations

## Exported Variables (Script-Accessible)
- `timing_dt` - Current delta time value, affected by scaling and pause
- `timing_true_dt` - Actual unmodified delta time, unaffected by scale or pause operations

## Exported Functions (Script-Accessible)
- `timing_set_scale(Scalar s)` - Sets the time scale multiplier
- `timing_get_scale()` - Returns the current time scale value
- `timing_set_paused(bool p)` - Pauses/unpauses the timing system
  - When paused: sets scale to 0 and stores previous scale
  - When resumed: restores the previous scale value
- `timing_get_paused()` - Returns current pause state as boolean

## Internal Functions (C-Only)
- `timing_update()` - Updates the timing system (likely called each frame)
- `timing_save_all(Store *s)` - Saves timing state to storage
- `timing_load_all(Store *s)` - Loads timing state from storage

## Key Features
- **Time Scaling**: Allows speeding up or slowing down game time
- **Pause System**: Intelligent pause that preserves scale settings
- **Delta Time Tracking**: Maintains both scaled and unscaled time measurements
- **Script Integration**: Core timing functions exposed to scripting system
- **Persistence**: Save/load support for timing state

The system uses the `SCRIPT()` macro to define which entities are exported to the scripting interface, making timing control available to game scripts while keeping internal update and persistence functions private to the C code.

### Transform Component System Implementation (transform.c)
This file implements a hierarchical transform component system for entities in what appears to be a game engine or similar application. The system manages 2D transformations with parent-child relationships and matrix caching for performance.

## Core Data Structure

• **Transform struct**: Contains all transformation data for an entity
  - `EntityPoolElem pool_elem`: Entity pool integration
  - `Vec2 position`: Local position
  - `Scalar rotation`: Local rotation angle
  - `Vec2 scale`: Local scale factors
  - `Entity parent`: Parent entity reference (entity_nil if root)
  - `Array *children`: Dynamic array of child entities (NULL if no children)
  - `Mat3 mat_cache`: Cached local transformation matrix
  - `Mat3 worldmat_cache`: Cached world transformation matrix
  - `unsigned int dirty_count`: Counter for tracking modifications

• **Static Variables**:
  - `EntityPool *pool`: Global entity pool for managing Transform components

## Matrix Update System

The system implements an efficient matrix caching mechanism with hierarchical updates:

• **_modified()**: Core update function triggered when transform properties change
  - Increments dirty_count for change tracking
  - Recalculates mat_cache using `mat3_scaling_rotation_translation()`
  - Updates worldmat_cache by multiplying parent's world matrix with local matrix
  - Recursively updates all children's world matrices via `_update_child()`

• **_update_child()**: Recursive helper for updating child transformations
  - Multiplies parent's world matrix with child's local matrix
  - Recursively processes all grandchildren

## Parent-Child Relationship Management

• **Attachment System**:
  - `transform_set_parent()`: Safely changes parent-child relationships
    - Prevents self-parenting
    - Detaches from old parent before attaching to new one
    - Maintains bidirectional parent-child links
    - Triggers matrix updates

• **Detachment System**:
  - `_detach()`: Removes specific parent-child link
    - Clears child's parent reference
    - Removes child from parent's children array using `array_quick_remove()`
  - `_detach_all()`: Completely disconnects transform from hierarchy
    - Detaches from parent
    - Clears all children's parent references
    - Frees children array

## Public API Functions

• **Component Management**:
  - `transform_add()`: Adds transform component with default values (position=0,0, rotation=0, scale=1,1)
  - `transform_remove()`: Removes component and handles cleanup
  - `transform_has()`: Checks if entity has transform component

• **Property Accessors**:
  - Position: `transform_set_position()`, `transform_get_position()`, `transform_translate()`
  - Rotation: `transform_set_rotation()`, `transform_get_rotation()`, `transform_rotate()`
  - Scale: `transform_set_scale()`, `transform_get_scale()`

• **World Space Operations**:
  - `transform_get_world_position()`, `transform_get_world_rotation()`, `transform_get_world_scale()`
  - `transform_get_world_matrix()`, `transform_get_matrix()`
  - `transform_local_to_world()`, `transform_world_to_local()`: Coordinate space conversions

• **Hierarchy Operations**:
  - `transform_get_parent()`, `transform_get_num_children()`, `transform_get_children()`
  - `transform_detach_all()`: Public interface to `_detach_all()`
  - `transform_destroy_rec()`: Recursively destroys entity and all descendants

## System Management

• **Initialization**:
  - `transform_init()`: Creates entity pool
  - `transform_deinit()`: Cleans up children arrays and frees pool

• **Update Cycle**:
  - `transform_update_all()`: Removes destroyed entities and updates edit bboxes

• **Utility Functions**:
  - `transform_get_dirty_count()`: Returns modification counter
  - `transform_set_save_filter_rec()`: Recursively sets save filtering for hierarchy

## Save/Load System

• **Persistence Support**:
  - `transform_save_all()`, `transform_load_all()`: Handle serialization of entire transform system
  - `_children_save()`, `_children_load()`: Special handling for children arrays during serialization
  - Respects entity save filtering to avoid saving filtered entities

## Key Dependencies

• **External Systems Referenced**:
  - `entitypool.h`: Entity pool management
  - `array.h`: Dynamic array implementation
  - `saveload.h`: Serialization system
  - `bbox.h`: Bounding box operations
  - `edit.h`: Editor integration
  - Mathematics types: `Vec2`, `Scalar`, `Mat3`

### Transform Component System Header (transform.h)
This header file defines a comprehensive transform component system for an entity-component-system (ECS) architecture, providing hierarchical 2D transformations with parent-child relationships.

## Core Dependencies
- **scalar.h**: Provides `Scalar` type for rotation values
- **vec2.h**: Provides `Vec2` type for 2D positions and scales
- **mat3.h**: Provides `Mat3` type for transformation matrices
- **entity.h**: Provides `Entity` type for entity references
- **script_export.h**: Provides `SCRIPT` and `EXPORT` macros for scripting integration
- **saveload.h**: Provides `Store` type for persistence operations

## Key Design Principles
- **Transformation order**: Scale → Rotation → Position (applied in that order)
- **Hierarchical structure**: Supports parent-child relationships between entities
- **Root transforms**: Have parent set to `entity_nil`

## Component Management Functions
- `transform_add(Entity ent)`: Adds transform component to entity
- `transform_remove(Entity ent)`: Removes transform component from entity
- `transform_has(Entity ent)`: Checks if entity has transform component

## Hierarchy Management
- `transform_set_parent(Entity ent, Entity parent)`: Sets parent-child relationship
- `transform_get_parent(Entity ent)`: Returns parent entity
- `transform_get_num_children(Entity ent)`: Returns number of child entities
- `transform_get_children(Entity ent)`: Returns array of child entities
- `transform_detach_all(Entity ent)`: Detaches entity from parent and all children
- `transform_destroy_rec(Entity ent)`: Recursively destroys entity and all children

## Local Transform Properties
### Position
- `transform_set_position(Entity ent, Vec2 pos)`: Sets local position
- `transform_get_position(Entity ent)`: Gets local position
- `transform_translate(Entity ent, Vec2 trans)`: Applies translation offset

### Rotation
- `transform_set_rotation(Entity ent, Scalar rot)`: Sets local rotation
- `transform_get_rotation(Entity ent)`: Gets local rotation
- `transform_rotate(Entity ent, Scalar rot)`: Applies rotation offset

### Scale
- `transform_set_scale(Entity ent, Vec2 scale)`: Sets local scale
- `transform_get_scale(Entity ent)`: Gets local scale

## World Transform Properties
- `transform_get_world_position(Entity ent)`: Gets position in world space
- `transform_get_world_rotation(Entity ent)`: Gets rotation in world space
- `transform_get_world_scale(Entity ent)`: Gets scale in world space

## Matrix Operations
- `transform_get_world_matrix(Entity ent)`: Returns world-space transformation matrix
- `transform_get_matrix(Entity ent)`: Returns parent-space transformation matrix
- **Special behavior**: Returns `mat3_identity()` for `entity_nil` so root transforms have identity parent-space

## Coordinate Space Conversion
- `transform_local_to_world(Entity ent, Vec2 v)`: Converts point from local to world space
- `transform_world_to_local(Entity ent, Vec2 v)`: Converts point from world to local space

## System Management
- `transform_get_dirty_count(Entity ent)`: Returns dirty state counter for optimization
- `transform_set_save_filter_rec(Entity ent, bool filter)`: Sets save filter recursively for entity and descendants

## System Functions (Non-exported)
- `transform_init()`: Initializes transform system
- `transform_deinit()`: Cleans up transform system
- `transform_update_all()`: Updates all transform calculations
- `transform_save_all(Store *s)`: Saves all transform data to store
- `transform_load_all(Store *s)`: Loads all transform data from store

## Integration Features
- **Scripting support**: All main functions are exported via `SCRIPT` macro
- **Persistence support**: Includes save/load functionality with filtering
- **Performance optimization**: Includes dirty tracking system

### 2D Vector Operations Implementation (vec2.c)
This file implements a comprehensive 2D vector mathematics library with basic arithmetic, geometric operations, and serialization capabilities.

## Defined Entities

### Global Constants
- `vec2_zero`: A global Vec2 constant initialized to (0.0, 0.0)

### Arithmetic Operations
- `vec2_add(Vec2 u, Vec2 v)`: Component-wise addition of two vectors
- `vec2_sub(Vec2 u, Vec2 v)`: Component-wise subtraction of two vectors  
- `vec2_mul(Vec2 u, Vec2 v)`: Component-wise multiplication of two vectors
- `vec2_div(Vec2 u, Vec2 v)`: Component-wise division of two vectors
- `vec2_scalar_mul(Vec2 v, Scalar f)`: Multiply vector by scalar
- `vec2_scalar_div(Vec2 v, Scalar f)`: Divide vector by scalar
- `scalar_vec2_div(Scalar f, Vec2 v)`: Divide scalar by vector components
- `vec2_neg(Vec2 v)`: Negate vector components

### Geometric Operations
- `vec2_len(Vec2 v)`: Calculate vector length using Euclidean distance formula
  - Uses `scalar_sqrt(v.x * v.x + v.y * v.y)`
- `vec2_normalize(Vec2 v)`: Normalize vector to unit length
  - Returns original vector if both components are zero (avoiding division by zero)
  - Otherwise divides vector by its length
- `vec2_dot(Vec2 u, Vec2 v)`: Calculate dot product of two vectors
- `vec2_dist(Vec2 u, Vec2 v)`: Calculate distance between two points
  - Computes length of difference vector

### Rotation and Angle Operations
- `vec2_rot(Vec2 v, Scalar rot)`: Rotate vector by given angle in radians
  - Uses standard 2D rotation matrix formula:
    - x' = x*cos(θ) - y*sin(θ)
    - y' = x*sin(θ) + y*cos(θ)
- `vec2_atan2(Vec2 v)`: Get angle of vector using arctangent
  - Wraps `scalar_atan2(v.y, v.x)`

### Serialization Functions
- `vec2_save(Vec2 *v, const char *n, Store *s)`: Save vector to storage
  - Creates compressed child store
  - Saves x and y components separately using `scalar_save`
- `vec2_load(Vec2 *v, const char *n, Vec2 d, Store *s)`: Load vector from storage
  - Loads x and y components using `scalar_load`
  - Uses default value `d` if loading fails
  - Returns boolean indicating success

### Constructor Function
- `vec2(Scalar x, Scalar y)`: Create Vec2 struct from scalar components
  - Undefines a macro version before defining the function implementation

## Key Dependencies
- `vec2.h`: Header file defining Vec2 struct and function declarations
- `saveload.h`: Serialization system providing Store type and related functions
- Scalar math functions: `scalar_sqrt`, `scalar_cos`, `scalar_sin`, `scalar_atan2`
- Scalar serialization functions: `scalar_save`, `scalar_load`

## Notes
- The `#undef vec2` directive suggests there's a macro version of the constructor that gets replaced by the function implementation
- All operations work with `Scalar` type rather than primitive floating-point types
- The normalization function includes zero-vector safety check
- Serialization uses a compressed storage format for efficiency

### Vec2 Header - 2D Vector Structure and Operations (vec2.h)
This header file defines a 2D vector structure and comprehensive mathematical operations for 2D vector manipulation.

## Structure Definition
- **Vec2**: A structure containing two `Scalar` components:
  - `x`: horizontal component
  - `y`: vertical component

## Dependencies
- `scalar.h`: Provides the `Scalar` type used for vector components
- `saveload.h`: Provides save/load functionality via `Store` type
- `script_export.h`: Provides `SCRIPT` and `EXPORT` macros for script integration

## Constructor and Constants
- **vec2(x, y)**: Constructor function that creates a Vec2 from two Scalar values
- **vec2_zero**: Predefined constant representing the zero vector (0, 0)
- **vec2 macro**: C inline macro `#define vec2(x, y) ((Vec2) { (x), (y) })` for direct structure initialization

## Mathematical Operations
- **Basic arithmetic operations**:
  - `vec2_add(u, v)`: Vector addition
  - `vec2_sub(u, v)`: Vector subtraction
  - `vec2_mul(u, v)`: Component-wise multiplication
  - `vec2_div(u, v)`: Component-wise division
  - `vec2_neg(v)`: Vector negation

- **Scalar operations**:
  - `vec2_scalar_mul(v, f)`: Multiply vector by scalar
  - `vec2_scalar_div(v, f)`: Divide vector by scalar (v.x/f, v.y/f)
  - `scalar_vec2_div(f, v)`: Divide scalar by vector (f/v.x, f/v.y)

## Geometric Operations
- **Length and normalization**:
  - `vec2_len(v)`: Calculate vector magnitude/length
  - `vec2_normalize(v)`: Return unit vector in same direction
  - `vec2_dist(u, v)`: Calculate distance between two points

- **Dot product**:
  - `vec2_dot(u, v)`: Calculate dot product of two vectors

- **Rotation and angle operations**:
  - `vec2_rot(v, rot)`: Rotate vector by given angle
  - `vec2_atan2(v)`: Calculate angle of vector using atan2

## Persistence Operations
- **vec2_save(v, n, s)**: Save Vec2 to Store with given name
- **vec2_load(v, n, d, s)**: Load Vec2 from Store with given name and default value, returns boolean success status

## Script Integration
The entire interface is wrapped in a `SCRIPT` macro with `EXPORT` declarations, indicating this module is designed to be accessible from a scripting system while maintaining C compatibility.


### Related
- [Parent](../)

---
Generated: 2025-07-07T20:21:19-04:00
