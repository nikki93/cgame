# Summary: test

## Table of Contents

- [Keyboard Control System Implementation (test/keyboard_controlled.c)](#keyboard-control-system-implementation-testkeyboard_controlledc)
  - [Keyboard Controlled Entity System Implementation](#keyboard-controlled-entity-system-implementation)
- [Keyboard Controlled Component System Header (test/keyboard_controlled.h)](#keyboard-controlled-component-system-header-testkeyboard_controlledh)
  - [Keyboard Controlled Component System Header](#keyboard-controlled-component-system-header)
- [Test Scene Setup with Entity Component System (test/test.c)](#test-scene-setup-with-entity-component-system-testtestc)
  - [Test Scene Setup with Camera, Blocks, and Player](#test-scene-setup-with-camera-blocks-and-player)
- [Header Guard and Function Declaration for Test Module (test/test.h)](#header-guard-and-function-declaration-for-test-module-testtesth)
  - [Header Guard and Function Declaration for TEST_H](#header-guard-and-function-declaration-for-test_h)

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

## Sections

### Keyboard Control System Implementation (test/keyboard_controlled.c)

This file implements a keyboard control system that allows a single entity to be controlled via keyboard input for movement, rotation, and scaling operations.

## Core System Architecture

The system maintains a single keyboard-controlled entity using a simple existence flag approach:
- `kc_exists`: Boolean flag indicating if a keyboard-controlled entity exists
- `kc_entity`: The Entity currently under keyboard control

## Public Interface Functions

• **keyboard_controlled_add(Entity ent)**
  - Adds keyboard control to an entity
  - Ensures the entity has a transform component via `transform_add()`
  - Sets the entity as the current keyboard-controlled entity

• **keyboard_controlled_remove(Entity ent)**
  - Removes keyboard control from the specified entity
  - Only removes if the entity matches the current keyboard-controlled entity

• **keyboard_controlled_has(Entity ent)**
  - Returns whether the specified entity has keyboard control
  - Checks both existence flag and entity equality

## Update Logic

**keyboard_controlled_update_all()** handles the main control logic:

• **Precondition Checks:**
  - Removes destroyed entities automatically
  - Skips processing if game is paused (`timing_get_paused()`)
  - Skips processing if GUI has focus (`gui_has_focus()`)

• **Movement Controls (Arrow Keys):**
  - LEFT/RIGHT: Move along X-axis at 5 units per second
  - UP/DOWN: Move along Y-axis at 5 units per second
  - Movement is relative to current rotation via `vec2_rot()`

• **Rotation Controls:**
  - N key: Rotate counterclockwise at 0.35π radians per second
  - M key: Rotate clockwise at 0.35π radians per second

• **Scaling Controls:**
  - K key: Increase scale at 12 units per second
  - I key: Decrease scale at 12 units per second (with minimum size protection)
  - Maintains aspect ratio by preserving the Y/X scale ratio

• **Transform Application:**
  - Applies position changes via `transform_translate()`
  - Updates rotation via `transform_set_rotation()`
  - Updates scale via `transform_set_scale()`

## Save/Load System

• **keyboard_controlled_save_all(Store *s)**
  - Saves the keyboard-controlled entity to a "keyboard_controlled" store section
  - Only saves if entity exists and passes save filter

• **keyboard_controlled_load_all(Store *s)**
  - Loads the keyboard-controlled entity from the store
  - Restores the existence flag upon successful load

## Key Dependencies

- **input.h**: For keyboard input detection (`input_key_down()`)
- **transform.h**: For entity transformation operations
- **timing.h**: For delta time and pause state
- **gui.h**: For focus management
- **saveload.h**: For persistence operations
- **entity system**: For entity management and lifecycle

## Design Notes

The code includes a comment indicating that using `entity_nil` for non-existent entities would be more ideal than the current boolean flag approach, suggesting this is a simpler but potentially less robust implementation.

#### Keyboard Controlled Entity System Implementation (test/keyboard_controlled.c)
Source: [keyboard_controlled.c:1-105](../test/keyboard_controlled.c#L1-L105)

This section implements a keyboard-controlled entity system that allows a single entity to be controlled via keyboard input for movement, rotation, and scaling operations.

## Core System Architecture

• **Single Entity Management**: The system tracks only one keyboard-controlled entity at a time using:
  - `kc_exists` (static bool): Flag indicating if a keyboard-controlled entity exists
  - `kc_entity` (static Entity): The current keyboard-controlled entity

• **Entity Management Functions**:
  - `keyboard_controlled_add(Entity ent)`: Adds an entity as keyboard-controlled and ensures it has transform component
  - `keyboard_controlled_remove(Entity ent)`: Removes keyboard control from specified entity
  - `keyboard_controlled_has(Entity ent)`: Checks if given entity is keyboard-controlled

## Main Update Logic

• **Update Function**: `keyboard_controlled_update_all()` processes keyboard input each frame with these steps:
  1. **Validation Checks**:
     - Verifies entity exists and isn't destroyed
     - Skips if game is paused (`timing_get_paused()`)
     - Skips if GUI has focus (`gui_has_focus()`)
  
  2. **Input Processing**:
     - **Movement Controls**:
       - `KC_LEFT`/`KC_RIGHT`: Horizontal movement (±5 units * delta time)
       - `KC_UP`/`KC_DOWN`: Vertical movement (±5 units * delta time)
     - **Rotation Controls**:
       - `KC_N`: Rotate counterclockwise (0.35π * delta time)
       - `KC_M`: Rotate clockwise (-0.35π * delta time)
     - **Scaling Controls**:
       - `KC_K`: Increase scale (12 units * delta time)
       - `KC_I`: Decrease scale (12 units * delta time, with minimum size check)
  
  3. **Transform Application**:
     - Rotates movement vector by current rotation
     - Applies translation via `transform_translate()`
     - Updates rotation via `transform_set_rotation()`
     - Updates scale via `transform_set_scale()` (maintains aspect ratio)

## Save/Load System

• **Save Function**: `keyboard_controlled_save_all(Store *s)` saves the keyboard-controlled entity if it exists and passes save filter
• **Load Function**: `keyboard_controlled_load_all(Store *s)` restores the keyboard-controlled entity from saved data

## Key Dependencies

• **External Systems**:
  - `transform.h`: For transform operations (`transform_add`, `transform_translate`, etc.)
  - `input.h`: For keyboard input detection (`input_key_down`)
  - `timing.h`: For delta time and pause state (`timing_dt`, `timing_get_paused`)
  - `gui.h`: For GUI focus checking (`gui_has_focus`)
  - `saveload.h`: For persistence operations (`Store` operations)

• **Mathematical Operations**:
  - `vec2()`, `vec2_add()`, `vec2_rot()`: Vector operations
  - `SCALAR_PI`: Mathematical constant for rotation calculations

## Design Notes

• The implementation uses a single global entity approach rather than the more ideal `entity_nil` pattern (as noted in the code comment)
• Scale operations maintain aspect ratio by preserving the y/x ratio
• All movement is frame-rate independent using `timing_dt`

### Keyboard Controlled Component System Header (test/keyboard_controlled.h)

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

#### Keyboard Controlled Component System Header (test/keyboard_controlled.h)
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

### Test Scene Setup with Entity Component System (test/test.c)

This section implements a test function that creates a basic game scene using an entity-component-system (ECS) architecture. The code demonstrates the creation and configuration of different game objects with various components.

## Function Definitions

**`test_c()`** - Main test function that sets up a complete game scene:
- Creates three main entities: camera, blocks, and player
- Configures each entity with appropriate components and properties
- Sets up keyboard control for the camera

**`test_init()`** - Empty initialization function (no implementation)

## Entity Creation and Component Setup

The function creates three types of entities with specific component configurations:

### Camera Entity
- Creates entity using `entity_create()`
- Adds `transform` component via `transform_add()`
- Adds `camera` component via `camera_add()`
- Receives keyboard control through `keyboard_controlled_add()`

### Block Entities (Randomly Generated)
- Generates random number of blocks (0-49) using `rand() % 50`
- For each block:
  - Creates entity with `entity_create()`
  - Adds transform component and sets random position using `transform_set_position()`
    - Position range: x(-12 to 12), y(-4 to 4)
  - Adds sprite component with `sprite_add()`
  - Configures sprite texture properties:
    - Texture cell: (32.0f, 32.0f) via `sprite_set_texcell()`
    - Texture size: (32.0f, 32.0f) via `sprite_set_texsize()`

### Player Entity
- Creates entity using `entity_create()`
- Adds transform component and sets position to origin (0.0f, 0.0f)
- Adds sprite component with specific texture coordinates:
  - Texture cell: (0.0f, 32.0f)
  - Texture size: (32.0f, 32.0f)

## Dependencies

The code references several external modules and functions:
- **Entity system**: `entity_create()`, `Entity` type
- **Transform system**: `transform_add()`, `transform_set_position()`, `vec2()`
- **Sprite system**: `sprite_add()`, `sprite_set_texcell()`, `sprite_set_texsize()`
- **Camera system**: `camera_add()`
- **Input system**: `keyboard_controlled_add()`
- **Standard library**: `rand()` for random number generation

## Key Logic Flow
1. Initialize camera entity with transform and camera components
2. Generate random number of block entities with randomized positions
3. Create single player entity at origin
4. Assign keyboard control to camera (not player)

#### Test Scene Setup with Camera, Blocks, and Player (test/test.c)
Source: [test.c:1-57](../test/test.c#L1-L57)

This section defines test functions for setting up a basic game scene with entities, transforms, sprites, and keyboard control.

## Functions Defined

### `test_c()`
This is the main test function that creates a simple game scene with three types of entities:

**Camera Setup:**
- Creates a camera entity using `entity_create()`
- Adds transform component via `transform_add()`
- Adds camera component via `camera_add()`
- Assigns keyboard control to the camera using `keyboard_controlled_add()`

**Block Generation:**
- Generates a random number of blocks (0-49) using `rand() % 50`
- For each block:
  - Creates entity with `entity_create()`
  - Adds transform component
  - Sets random position using `transform_set_position()` with coordinates ranging from (-12, -4) to (12, 4)
  - Adds sprite component via `sprite_add()`
  - Configures sprite texture cell at position (32.0f, 32.0f) using `sprite_set_texcell()`
  - Sets sprite texture size to 32x32 pixels using `sprite_set_texsize()`

**Player Setup:**
- Creates player entity
- Adds transform component
- Positions player at origin (0.0f, 0.0f)
- Adds sprite component
- Sets sprite texture cell at position (0.0f, 32.0f)
- Sets sprite texture size to 32x32 pixels

### `test_init()`
Empty initialization function with no implementation.

## Key Dependencies
- **Entity System**: `entity_create()`, `Entity` type
- **Transform System**: `transform_add()`, `transform_set_position()`, `vec2()`
- **Sprite System**: `sprite_add()`, `sprite_set_texcell()`, `sprite_set_texsize()`
- **Camera System**: `camera_add()`
- **Input System**: `keyboard_controlled_add()`
- **Standard Library**: `rand()` for random number generation

## Scene Layout
The test creates a scene where:
- The camera has keyboard control for navigation
- Blocks are randomly scattered across a 25x9 grid area
- The player is positioned at the world origin
- All sprites use 32x32 pixel textures from different positions in a texture atlas

### Header Guard and Function Declaration for Test Module (test/test.h)

This is a standard C/C++ header file that provides the interface for a test module. The file contains:

## Header Guard Protection
- Uses `#ifndef TEST_H` / `#define TEST_H` / `#endif` pattern to prevent multiple inclusions of the header file
- The guard macro is named `TEST_H`

## Function Declarations
- **`test_init()`**: A function declaration with void return type and no parameters
  - This appears to be an initialization function for the test module
  - Only the declaration is provided; the actual implementation would be in a corresponding `.c` file

## Key Characteristics
- Minimal header file with no dependencies on other headers
- No data structures, constants, or other entities defined
- Clean interface exposing only a single initialization function
- Follows standard C header file conventions

This header file serves as a simple public interface for a test module, allowing other parts of the codebase to call the `test_init()` function for test initialization purposes.

#### Header Guard and Function Declaration for TEST_H (test/test.h)
Source: [test.h:1-4](../test/test.h#L1-L4)

This section defines a standard C/C++ header file with include guard protection and declares a single initialization function.

**Header Guard Structure:**
- Uses `#ifndef TEST_H` / `#define TEST_H` / `#endif` pattern to prevent multiple inclusions of this header file
- The guard macro is named `TEST_H`, matching the likely filename convention

**Function Declaration:**
- Declares one function: `test_init()`
  - Return type: `void` (no return value)
  - Parameters: none (empty parameter list)
  - This appears to be an initialization function based on the naming convention
  - Only the declaration is provided; the actual implementation would be in a separate source file

**Code Entities Defined:**
- Header guard macro: `TEST_H`
- Function declaration: `test_init()`

**Dependencies:**
- No external dependencies or references to other entities in this section
- This is a minimal header file that could be included by other files that need to call the `test_init()` function

This header file follows standard C/C++ conventions and provides a clean interface for code that needs to perform some kind of test initialization functionality.


### Related
- [Parent](../)

---
Generated: 2025-07-07T20:21:19-04:00
