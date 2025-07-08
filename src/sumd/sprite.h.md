# Summary: sprite.h

## Table of Contents

- [Sprite Component System Header Definition](#sprite-component-system-header-definition)

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

## Sections

### Sprite Component System Header Definition
Source: [sprite.h:1-43](../sprite.h#L1-L43)

This header file defines a sprite component system for rendering 2D sprites in a game engine using an entity-component architecture.

## Key Components

### Header Guards and Dependencies
- Uses standard header guard pattern with `SPRITE_H`
- Dependencies on other engine modules:
  - `saveload.h` - for persistence functionality
  - `entity.h` - for Entity type definitions
  - `vec2.h` - for 2D vector operations
  - `script_export.h` - for script binding support

### Exported Script Interface
The file defines a comprehensive script-exportable API wrapped in `SCRIPT(sprite, ...)` macro:

**Atlas Management:**
- `sprite_set_atlas(const char *filename)` - sets the texture atlas file
- `sprite_get_atlas()` - retrieves current atlas filename

**Entity Component Management:**
- `sprite_add(Entity ent)` - adds sprite component to entity
- `sprite_remove(Entity ent)` - removes sprite component from entity  
- `sprite_has(Entity ent)` - checks if entity has sprite component

**Visual Properties:**
- `sprite_set_size(Entity ent, Vec2 size)` / `sprite_get_size(Entity ent)` - controls world-space rendering size, centered at transform position
- `sprite_set_texcell(Entity ent, Vec2 texcell)` / `sprite_get_texcell(Entity ent)` - sets bottom-left corner of atlas region in pixels
- `sprite_set_texsize(Entity ent, Vec2 texsize)` / `sprite_get_texsize(Entity ent)` - defines atlas region size in pixels
- `sprite_set_depth(Entity ent, int depth)` / `sprite_get_depth(Entity ent)` - controls rendering order (lower depth renders on top)

### Internal System Functions
Functions for engine-internal use (not script-exported):
- `sprite_init()` - initializes the sprite system
- `sprite_deinit()` - cleans up the sprite system
- `sprite_update_all()` - updates all sprite components
- `sprite_draw_all()` - renders all sprites
- `sprite_save_all(Store *s)` / `sprite_load_all(Store *s)` - handles persistence using Store system

## Architecture Notes
- Uses entity-component system where sprites are components attached to entities
- Supports texture atlas-based rendering for efficient sprite batching
- Integrates with transform system for positioning
- Provides depth-based rendering order control
- Includes full save/load functionality for game state persistence


### Related
- [Parent](../)

---
Hash: 4a37d0fe53b246b25b2bdfc54938bc2794923503260a2863127a1ebffd1ad7b3
Generated: 2025-07-07T20:21:19-04:00
