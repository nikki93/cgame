# Summary: sprite.c

## Table of Contents

- [Sprite Component System Implementation](#sprite-component-system-implementation)

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

## Sections

### Sprite Component System Implementation
Source: [sprite.c:1-285](../sprite.c#L1-L285)

This section implements a complete sprite rendering system for a 2D game engine using OpenGL and an Entity Component System (ECS) architecture.

## Core Data Structure

**`Sprite` struct** - The main component containing:
- `EntityPoolElem pool_elem` - ECS integration
- `Mat3 wmat` - World transform matrix for shaders
- `Vec2 size` - Sprite dimensions
- `Vec2 texcell` - Texture atlas cell position
- `Vec2 texsize` - Texture atlas cell size
- `int depth` - Z-order for rendering

## Global State Management

**Static variables:**
- `EntityPool *pool` - Manages all sprite entities
- `char *atlas` - Current texture atlas filename
- `GLuint program` - OpenGL shader program
- `GLuint vao` - Vertex Array Object
- `GLuint vbo` - Vertex Buffer Object

## Key Functionality

### Atlas Management
- **`sprite_set_atlas(const char *filename)`** - Loads texture atlas with error handling
- **`_set_atlas(const char *filename, bool err)`** - Internal atlas loader
  - Loads texture using `texture_load()`
  - Updates atlas filename string
  - Sets atlas_size uniform in shader
- **`sprite_get_atlas()`** - Returns current atlas filename

### Entity Management
- **`sprite_add(Entity ent)`** - Adds sprite component to entity
  - Checks for existing sprite to avoid duplicates
  - Adds transform component dependency
  - Initializes default values: size(1,1), texcell(32,32), texsize(32,32), depth(0)
- **`sprite_remove(Entity ent)`** - Removes sprite component
- **`sprite_has(Entity ent)`** - Checks if entity has sprite component

### Property Accessors
Complete getter/setter pairs for all sprite properties:
- **Size**: `sprite_set_size()` / `sprite_get_size()`
- **Texture cell**: `sprite_set_texcell()` / `sprite_get_texcell()`
- **Texture size**: `sprite_set_texsize()` / `sprite_get_texsize()`
- **Depth**: `sprite_set_depth()` / `sprite_get_depth()`

### System Lifecycle

**`sprite_init()`** - System initialization:
- Creates entity pool
- Loads shader program from "sprite.vert", "sprite.geom", "sprite.frag"
- Sets up OpenGL uniforms and texture binding
- Loads default atlas "default.png"
- Creates VAO/VBO and binds vertex attributes for instanced rendering

**`sprite_deinit()`** - Cleanup:
- Deletes OpenGL resources (program, buffers, VAO)
- Frees entity pool and atlas string

### Frame Processing

**`sprite_update_all()`** - Per-frame updates:
- Removes destroyed entities
- Updates world transform matrices from transform system
- Updates edit bounding boxes when editor is enabled
- Uses bbox with corners at (-0.5, -0.5) to (0.5, 0.5) scaled by sprite size

**`sprite_draw_all()`** - Rendering pipeline:
- **Depth sorting**: Uses `_depth_compare()` for descending depth order
  - Breaks ties using Entity ID for stability
- **Shader setup**: Binds program, updates camera inverse view matrix uniform
- **Texture binding**: Activates and binds current atlas
- **Instanced rendering**: Uploads all sprite data to VBO and draws as GL_POINTS

### Save/Load System

**`sprite_save_all(Store *s)`** - Serialization:
- Saves atlas filename
- Saves all sprite properties: size, texcell, texsize, depth

**`sprite_load_all(Store *s)`** - Deserialization:
- Loads atlas with error tolerance
- Restores sprite properties with sensible defaults

## Key Dependencies
- **EntityPool system** - ECS entity management
- **Transform system** - World matrix calculations
- **Texture system** - Atlas loading and binding
- **Camera system** - View matrix for rendering
- **Edit system** - Bounding box updates for editor
- **Save/Load system** - Persistence functionality
- **OpenGL/GLEW** - Graphics rendering
- **Mat3/Vec2** - Math utilities

The system uses instanced rendering where each sprite becomes a point in OpenGL, with the geometry shader likely expanding points into quads using the provided transform and texture data.


### Related
- [Parent](../)

---
Hash: 8c54d1d70f97b6ac28986e70e86ada23bd166df2b0e60a555883c169ff6185cf
Generated: 2025-07-07T20:21:19-04:00
