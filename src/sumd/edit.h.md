# Summary: edit.h

## Table of Contents

- [Edit System Header - Entity Selection and Grid-Based Editing Interface](#edit-system-header---entity-selection-and-grid-based-editing-interface)

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

## Sections

### Edit System Header - Entity Selection and Grid-Based Editing Interface
Source: [edit.h:1-48](../edit.h#L1-L48)

This header file defines the interface for an edit system that provides entity selection, grid-based editing, and visual feedback capabilities for a game or application editor.

## Main Components

### Script-Exported Functions (Available to scripting system)
The file uses a `SCRIPT(edit, ...)` macro to expose functions to a scripting system:

**Edit Mode Control:**
- `edit_set_enabled(bool e)` - Enable/disable the edit system
- `edit_get_enabled()` - Check if edit system is enabled

**Entity Editability:**
- `edit_set_editable(Entity ent, bool editable)` - Mark entities as editable/non-editable
- `edit_get_editable(Entity ent)` - Check if an entity can be edited
- Non-editable entities cannot be selected

**Grid System:**
- `edit_set_grid_size(Vec2 size)` - Set grid dimensions (non-negative values, zero means no grid)
- `edit_get_grid_size()` - Get current grid size

**Bounding Box Management:**
- `edit_bboxes_update(Entity ent, BBox bbox)` - Merge bounding box data for click selection
- `edit_bboxes_has(Entity ent)` - Check if entity has bounding box data
- `edit_bboxes_get(Entity ent)` - Retrieve entity's bounding box
- `edit_bboxes_get_num()` - Get total number of bounding boxes
- `edit_bboxes_get_nth(unsigned int n)` - Get nth bounding box as EntityBBoxPair
- `edit_bboxes_set_selected(Entity ent, bool selected)` - Set selection state

**Visual Debug Lines:**
- `edit_line_add(Vec2 a, Vec2 b, Scalar point_size, Color color)` - Draw lines between world-space coordinates

### Data Structures
- `EntityBBoxPair` - Struct containing an Entity and its associated BBox for pairing entity data with spatial bounds

### Internal System Functions
Functions for system lifecycle management (not script-exported):
- `edit_clear()` - Clear edit system state
- `edit_init()` - Initialize the edit system
- `edit_deinit()` - Clean up edit system resources
- `edit_update_all()` - Update all edit system components
- `edit_draw_all()` - Render all edit system visuals
- `edit_save_all(Store *s)` - Serialize edit system state
- `edit_load_all(Store *s)` - Deserialize edit system state

### Dependencies
The system depends on several external modules:
- `entity.h` - Entity system for managing game objects
- `bbox.h` - Bounding box utilities for spatial calculations
- `color.h` - Color representation for visual elements
- `script_export.h` - Scripting system integration macros

This edit system appears to be designed for in-game or in-editor entity manipulation, providing selection, grid snapping, and visual feedback capabilities commonly found in level editors or development tools.


### Related
- [Parent](../)

---
Hash: b0dfe3efd65805e7c04d821808a51de653d0bbb7f79dcbde77735bb1e7e5e148
Generated: 2025-07-07T20:21:19-04:00
