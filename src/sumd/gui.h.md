# Summary: gui.h

## Table of Contents

- [GUI System Header - Entity-Based UI Framework](#gui-system-header---entity-based-ui-framework)

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

## Sections

### GUI System Header - Entity-Based UI Framework
Source: [gui.h:1-129](../gui.h#L1-L129)

This header file defines a comprehensive GUI system built on an entity-component architecture, providing a complete interface for creating and managing user interface elements.

## Core System Structure

• **Entity-based GUI system**: All GUI elements are entities with components for different behaviors
• **Script exportable**: Uses `SCRIPT()` macro to export functions for scripting language binding
• **Dependencies**: Relies on `entity.h`, `color.h`, `vec2.h`, and `input.h` modules

## Root Entity Management

• **`gui_get_root()`**: Returns the root entity that serves as parent for all GUI entities
  - Root entity's transform uses screen pixel coordinates
  - Children remain in camera view automatically

## Basic GUI Entity Management

• **Entity lifecycle**:
  - `gui_add(Entity ent)`: Add entity to GUI system
  - `gui_remove(Entity ent)`: Remove entity from GUI system
  - `gui_has(Entity ent)`: Check if entity is in GUI system

• **Basic properties**:
  - Color: `gui_set_color()`, `gui_get_color()`
  - Visibility: `gui_set_visible()`, `gui_get_visible()`
  - Focus capability: `gui_set_focusable()`, `gui_get_focusable()`
  - Event capture: `gui_set_captures_events()`, `gui_get_captures_events()`

## Layout System

• **`GuiAlign` enumeration** defines alignment modes:
  - `GA_MIN` (0): Left horizontal, bottom vertical
  - `GA_MID` (1): Center horizontal, center vertical
  - `GA_MAX` (2): Right horizontal, top vertical
  - `GA_TABLE` (3): Table layout (left-right, top-down)
  - `GA_NONE` (4): Manual positioning

• **Alignment controls**:
  - `gui_set_halign()`, `gui_get_halign()`: Horizontal alignment
  - `gui_set_valign()`, `gui_get_valign()`: Vertical alignment
  - `gui_set_padding()`, `gui_get_padding()`: Padding as Vec2 (horizontal, vertical)

## Focus Management

• **Focus system**:
  - `gui_set_focused_entity(Entity ent)`: Set focus (use `entity_nil` for no focus)
  - `gui_get_focused_entity()`: Get currently focused entity
  - `gui_set_focus()`, `gui_get_focus()`: Set/get focus state for specific entity
  - `gui_has_focus()`: Check if any GUI element has focus

## Event System

• **Event triggering**:
  - `gui_fire_event_changed()`: Manually trigger change event

• **Event queries** (return true/false or key/mouse codes):
  - `gui_event_focus_enter()`, `gui_event_focus_exit()`: Focus events
  - `gui_event_changed()`: Input value changed
  - `gui_event_mouse_down()`, `gui_event_mouse_up()`: Mouse events (return `MouseCode`)
  - `gui_event_key_down()`, `gui_event_key_up()`: Keyboard events (return `KeyCode`)

• **Event capture**:
  - `gui_captured_event()`: Check if GUI system captured current event

## GUI Components

### Rectangle Component (`gui_rect`)
• **Management**: `gui_rect_add()`, `gui_rect_remove()`, `gui_rect_has()`
• **Size**: `gui_rect_set_size()`, `gui_rect_get_size()` (uses `Vec2`)
• **Fit behavior**: 
  - `gui_rect_set_hfit()`, `gui_rect_get_hfit()`: Horizontal fit
  - `gui_rect_set_vfit()`, `gui_rect_get_vfit()`: Vertical fit
• **Fill behavior**:
  - `gui_rect_set_hfill()`, `gui_rect_get_hfill()`: Horizontal fill
  - `gui_rect_set_vfill()`, `gui_rect_get_vfill()`: Vertical fill

### Text Component (`gui_text`)
• **Management**: `gui_text_add()`, `gui_text_remove()`, `gui_text_has()`
• **Content**: `gui_text_set_str()`, `gui_text_get_str()`
• **Cursor**: `gui_text_set_cursor()` for text cursor positioning

### Text Edit Component (`gui_textedit`)
• **Management**: `gui_textedit_add()`, `gui_textedit_remove()`, `gui_textedit_has()`
• **Cursor**: `gui_textedit_set_cursor()`, `gui_textedit_get_cursor()` (unsigned int)
• **Numerical mode**: 
  - `gui_textedit_set_numerical()`, `gui_textedit_get_numerical()`: Enable/check numerical input
  - `gui_textedit_get_num()`: Get numerical value (returns 0 if not numerical, uses `Scalar` type)

## Internal System Functions

• **System lifecycle**: `gui_init()`, `gui_deinit()`
• **Update cycle**: `gui_update_all()`, `gui_draw_all()`
• **Input handling**: 
  - `gui_key_down()`, `gui_key_up()`: Keyboard input
  - `gui_char_down()`: Character input
  - `gui_mouse_down()`, `gui_mouse_up()`: Mouse input
• **Event management**: `gui_event_clear()`
• **Persistence**: `gui_save_all()`, `gui_load_all()` (uses `Store` type)

## Key External Dependencies

• **`Entity`**: Core entity type from entity system
• **`Color`**: Color representation
• **`Vec2`**: 2D vector for positions and sizes
• **`KeyCode`, `MouseCode`**: Input event codes
• **`Scalar`**: Numerical value type
• **`Store`**: Persistence/serialization system


### Related
- [Parent](../)

---
Hash: 34f94afe2cbbdfdf832691697b43d2d20653aff2b6d89e025aa55c3eccf6e962
Generated: 2025-07-07T20:21:19-04:00
