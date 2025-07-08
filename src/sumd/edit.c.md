# Summary: edit.c

## Table of Contents

- [Edit System - Entity Editability and Visual Bounding Box Management](#edit-system---entity-editability-and-visual-bounding-box-management)
- [Line Rendering System and Edit Module Main Interface](#line-rendering-system-and-edit-module-main-interface)

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

## Sections

### Edit System - Entity Editability and Visual Bounding Box Management
Source: [edit.c:1-306](../edit.c#L1-L306)

This section implements an edit system that manages entity editability and provides visual bounding box rendering with grid overlay functionality for a game/graphics editor.

## Core Edit System State Management

**Key Global Variables:**
- `enabled` - boolean flag controlling if edit mode is active
- `uneditable_pool` - EntityPool storing entities that cannot be edited

**Public Functions:**
- `edit_set_enabled(bool e)` - enables/disables edit mode
- `edit_get_enabled()` - returns current edit mode state
- `edit_set_editable(Entity ent, bool editable)` - marks entity as editable/uneditable by adding/removing from uneditable_pool
- `edit_get_editable(Entity ent)` - returns true if entity is NOT in uneditable_pool

## Bounding Box System

**Data Structure:**
- `BBoxPoolElem` struct contains:
  - `EntityPoolElem pool_elem` - for entity pool management
  - `Mat3 wmat` - world transformation matrix
  - `BBox bbox` - bounding box coordinates
  - `Scalar selected` - selection state (>0.5 means selected)

**Key Variables:**
- `bbox_pool` - EntityPool managing all bounding box elements
- `bboxes_program` - OpenGL shader program for rendering
- `bboxes_vao`, `bboxes_vbo` - OpenGL vertex array/buffer objects

**Bounding Box Management Functions:**
- `edit_bboxes_update(Entity ent, BBox bbox)` - updates or merges bounding box for editable entities
- `edit_bboxes_has(Entity ent)` - checks if entity has bounding box
- `edit_bboxes_get(Entity ent)` - retrieves bounding box for entity
- `edit_bboxes_get_num()` - returns total number of bounding boxes
- `edit_bboxes_get_nth(unsigned int n)` - gets nth EntityBBoxPair
- `edit_bboxes_set_selected(Entity ent, bool selected)` - marks entity as selected/unselected

**Rendering Pipeline:**
- `_bboxes_init()` - initializes OpenGL resources:
  - Creates shader program from bbox.vert, bbox.geom, bbox.frag files
  - Sets up VAO/VBO and binds vertex attributes (wmat1-3, bbmin, bbmax, selected)
- `_bboxes_deinit()` - cleans up OpenGL resources
- `_bboxes_update_all()` - updates world matrices for all bounding boxes:
  - Skips entities without transform component
  - Updates world matrix from transform system
  - Applies default bbox ({ -0.25, -0.25 }, { 0.25, 0.25 }) for invalid boxes
- `_bboxes_draw_all()` - renders all bounding boxes:
  - Sets shader uniforms (inverse_view_matrix, aspect ratio, is_grid=0)
  - Uploads all bbox data to GPU and draws as GL_POINTS

## Grid System

**Key Variables:**
- `grid_size` - Vec2 defining grid cell dimensions (default 1.0, 1.0)
- `grid_cells` - Array of BBoxPoolElem used for grid rendering

**Grid Management Functions:**
- `edit_set_grid_size(Vec2 size)` - sets grid size, clamping negative values to 0
- `edit_get_grid_size()` - returns current grid size

**Grid Generation Algorithm (`_grid_create_cells()`):**
1. **Camera Bounds Calculation:** Gets current camera's world-space bounding box
2. **Cell Size Determination:** 
   - Uses grid_size if > 0, otherwise uses camera size + 1
   - Doubles cell size if grid would exceed 70 cells per dimension
3. **Grid Alignment:** Snaps grid origin to lower grid boundary using floor division
4. **Cell Population:** Creates BBoxPoolElem for each grid cell with appropriate transformation matrix

**Grid Rendering (`_grid_draw()`):**
- Uses same shader program as bounding boxes
- Sets is_grid uniform to 1
- Generates grid cells dynamically based on current view
- Renders cells as GL_POINTS and clears array afterwards

**Dependencies:**
- Uses external systems: `gfx`, `entitymap`, `entitypool`, `transform`, `camera`, `game`
- Depends on shader files: `bbox.vert`, `bbox.geom`, `bbox.frag`
- Uses utility modules: `mat3`, `array`, `dirs`, `input`, `error`

### Line Rendering System and Edit Module Main Interface
Source: [edit.c:309-462](../edit.c#L309-L462)

This section implements a line rendering system and the main interface for an edit module, providing functionality to draw lines, manage uneditable entities, and handle module lifecycle.

## Line Rendering System

**Core Data Structures:**
- `LinePoint` struct containing:
  - `Vec2 position` - 2D position coordinates
  - `Scalar point_size` - size of rendered points
  - `Color color` - rendering color
- `line_points` - Array storing LinePoint instances where consecutive pairs form lines

**OpenGL Resources:**
- `line_program` - OpenGL shader program for line rendering
- `line_vao` - Vertex Array Object for line drawing
- `line_vbo` - Vertex Buffer Object for line data

**Key Functions:**
- `edit_line_add(Vec2 a, Vec2 b, Scalar point_size, Color color)`:
  - Adds a line segment between points a and b
  - Creates two LinePoint instances with identical styling
  - Appends both points to the line_points array

- `_line_init()`:
  - Initializes the line_points array
  - Creates OpenGL shader program from "edit_line.vert" and "edit_line.frag"
  - Sets up VAO and VBO
  - Binds vertex attributes: position (2 floats), point_size (1 float), color (4 floats)

- `_line_draw_all()`:
  - Updates shader uniforms with inverse view matrix from camera
  - Uploads line point data to GPU using GL_STREAM_DRAW
  - Renders geometry twice: once as GL_LINES, once as GL_POINTS

- `_line_deinit()`:
  - Cleans up OpenGL resources (program, buffers, VAO)
  - Frees the line_points array

## Edit Module Main Interface

**Entity Management:**
- `uneditable_pool` - EntityPool for managing entities that cannot be edited
- `_uneditable_remove(Entity ent)` - Removes entities from the uneditable pool

**Module Lifecycle Functions:**
- `edit_init()`:
  - Initializes uneditable_pool as EntityPool of EntityPoolElem
  - Calls initialization functions for bboxes, grid, and line subsystems

- `edit_deinit()`:
  - Calls cleanup functions for line, grid, and bboxes subsystems
  - Frees the uneditable_pool

- `edit_clear()`:
  - Clears bbox_pool entities
  - Clears all line_points

**Update and Rendering:**
- `edit_update_all()`:
  - Removes destroyed entities from uneditable_pool
  - Updates all bounding boxes

- `edit_draw_all()`:
  - Only renders if `enabled` flag is true
  - Draws bboxes, grid, and lines in sequence

**Persistence:**
- `edit_save_all(Store *s)`:
  - Saves grid_size using vec2_save
  - Saves uneditable_pool contents using entitypool_save_foreach

- `edit_load_all(Store *s)`:
  - Loads grid_size using vec2_load with fallback default
  - Loads uneditable_pool contents using entitypool_load_foreach

**Dependencies:**
- References external functions: `gfx_create_program`, `gfx_bind_vertex_attrib`, `camera_get_inverse_view_matrix_ptr`
- Uses external types: `Vec2`, `Scalar`, `Color`, `Entity`, `Store`, `EntityPoolElem`
- Depends on other edit subsystems: bboxes (`_bboxes_init`, `_bboxes_deinit`, `_bboxes_update_all`, `_bboxes_draw_all`) and grid (`_grid_init`, `_grid_deinit`, `_grid_draw`)


### Related
- [Parent](../)

---
Hash: 4aa0c2ad330c9a0ab18b3e58d7d70c7ab64a9a7781fd7ee62f0d0bd64f0dccd4
Generated: 2025-07-07T20:21:19-04:00
