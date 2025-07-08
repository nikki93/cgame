# Summary: test/keyboard_controlled.c

## Table of Contents

- [Keyboard Controlled Entity System Implementation](#keyboard-controlled-entity-system-implementation)

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

## Sections

### Keyboard Controlled Entity System Implementation
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


### Related
- [Parent](../)

---
Hash: dbb71f5edffaf034c33c1feefcd091bf711a31f1f871e01ab7e2b87c8bc8f035
Generated: 2025-07-07T20:21:19-04:00
