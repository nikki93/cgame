# Summary: test/test.c

## Table of Contents

- [Test Scene Setup with Camera, Blocks, and Player](#test-scene-setup-with-camera-blocks-and-player)

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

## Sections

### Test Scene Setup with Camera, Blocks, and Player
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


### Related
- [Parent](../)

---
Hash: c99103c85e33108de472ea8bd667df32dd6c9161ba840046b179cd4a5019ddb9
Generated: 2025-07-07T20:21:19-04:00
