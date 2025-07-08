# Summary: gui.c

## Table of Contents

- [GUI System Core Structure and Common Functionality](#gui-system-core-structure-and-common-functionality)
- [GUI System Common Functions and Rectangle Component](#gui-system-common-functions-and-rectangle-component)
- [GUI Rectangle Component Management and Layout System](#gui-rectangle-component-management-and-layout-system)
- [GUI Rectangle and Text System Implementation](#gui-rectangle-and-text-system-implementation)
- [Text System Save/Load and TextEdit Widget Implementation](#text-system-saveload-and-textedit-widget-implementation)
- [GUI Main Interface Functions](#gui-main-interface-functions)

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

## Sections

### GUI System Core Structure and Common Functionality
Source: [gui.c:1-269](../gui.c#L1-L269)

This section implements the foundational GUI system for the application, providing entity-based GUI components with event handling, focus management, and basic properties.

## Core Data Structures

• **`Gui` struct**: Main GUI component structure containing:
  - `EntityPoolElem pool_elem`: Pool management element
  - `bool setvisible`: Externally-set visibility flag
  - `bool visible`: Internal recursively computed visibility
  - `bool updated_visible`: Flag for recursive visibility computation
  - `bool focusable`: Whether entity can receive focus
  - `bool captures_events`: Whether entity captures input events
  - `Color color`: GUI element color
  - `BBox bbox`: Bounding box in entity space
  - `GuiAlign halign, valign`: Horizontal and vertical alignment
  - `Vec2 padding`: Padding values

## Global State Variables

• **Entity Management**:
  - `gui_root`: Root entity for all GUI elements (for screen movement)
  - `focused`: Currently focused entity (or `entity_nil` if none)
  - `gui_pool`: EntityPool for managing GUI components

• **Event System**:
  - `captured_event`: Boolean flag indicating if an event was captured
  - Multiple EntityMap instances for event handling:
    - `focus_enter_map`: Focus enter events
    - `focus_exit_map`: Focus exit events
    - `changed_map`: Change events
    - `mouse_down_map`, `mouse_up_map`: Mouse button events
    - `key_down_map`, `key_up_map`: Keyboard events

## Key Functions

• **Entity Management**:
  - `gui_add(Entity ent)`: Adds GUI component to entity with default properties
  - `gui_remove(Entity ent)`: Removes GUI component from entity
  - `gui_has(Entity ent)`: Checks if entity has GUI component
  - `gui_get_root()`: Returns the root GUI entity

• **Property Getters/Setters** (following standard pattern):
  - Color: `gui_set_color()`, `gui_get_color()`
  - Visibility: `gui_set_visible()`, `gui_get_visible()`
  - Focusability: `gui_set_focusable()`, `gui_get_focusable()`
  - Event capturing: `gui_set_captures_events()`, `gui_get_captures_events()`
  - Alignment: `gui_set_halign()`, `gui_get_halign()`, `gui_set_valign()`, `gui_get_valign()`
  - Padding: `gui_set_padding()`, `gui_get_padding()`

• **Focus Management**:
  - `gui_set_focused_entity(Entity ent)`: Sets focused entity and triggers focus events
  - `gui_get_focused_entity()`: Returns currently focused entity
  - `gui_set_focus()`, `gui_get_focus()`: Entity-specific focus control
  - `gui_has_focus()`: Checks if any entity has focus

• **Event System**:
  - `gui_fire_event_changed(Entity ent)`: Triggers change event for entity
  - Event query functions: `gui_event_focus_enter()`, `gui_event_focus_exit()`, `gui_event_changed()`, `gui_event_mouse_down()`, `gui_event_mouse_up()`, `gui_event_key_down()`, `gui_event_key_up()`
  - `gui_captured_event()`: Returns whether any event was captured

• **Initialization**:
  - `_common_init()`: Private function that initializes all pools and event maps

## Dependencies

• **External Systems**:
  - `entitypool.h`: Entity pool management
  - `transform.h`: Transform component (automatically added with GUI)
  - `error.h`: Error handling and assertions
  - Various utility headers: `mat3.h`, `array.h`, `gfx.h`, `texture.h`, etc.

## Default Values

When adding a GUI component via `gui_add()`:
- Visibility: `true` (both internal and external)
- Focusable: `false`
- Captures events: `true`
- Color: `color_gray`
- Bounding box: 32x32 pixels at origin
- Alignment: `GA_NONE` for both horizontal and vertical
- Padding: 5x5 pixels

### GUI System Common Functions and Rectangle Component
Source: [gui.c:270-574](../gui.c#L270-L574)

This section implements core GUI system functionality including cleanup, event handling, alignment, and introduces the rectangle GUI component.

## Common System Functions

### Cleanup and Lifecycle Management
- **`_common_deinit()`**: Frees all GUI system resources including event maps (key_up_map, key_down_map, mouse_up_map, mouse_down_map, changed_map, focus_enter_map, focus_exit_map) and the gui_pool entity pool
- **`_common_update_destroyed()`**: Cleans up destroyed entities by clearing focused entity if destroyed and removing destroyed entities from gui_pool

### Visibility System
- **`_common_update_visible_rec()`**: Recursively calculates visibility for GUI elements
  - Uses `updated_visible` flag to prevent redundant calculations
  - Explicit false visibility (`setvisible = false`) takes priority
  - Children inherit parent visibility through transform hierarchy
  - Root elements default to visible
- **`_common_update_visible()`**: Updates visibility for all GUI elements by resetting flags and running recursive calculation

### Alignment System
- **`_common_align()`**: Aligns GUI elements within their parent's bounding box
  - Takes `GuiAlign` parameters for horizontal and vertical alignment (halign, valign)
  - Supports alignment types: GA_MIN, GA_MAX, GA_MID, GA_NONE
  - Uses transform matrices and bounding boxes for positioning calculations
  - Accounts for padding in alignment calculations
  - Implements axis_align macro to handle both x and y axis alignment uniformly
- **`_common_reset_align()`**: Resets all GUI elements to top-left position for fit calculations
- **`_common_update_align()`**: Applies proper alignment to all GUI elements using their stored alignment settings

### Hierarchy Management
- **`_common_attach_root()`**: Attaches orphaned GUI entities (those without parents) to gui_root entity
- **`_common_update_all()`**: Main update function that attaches roots and updates edit bboxes when edit mode is enabled

### Event System
- **`_common_mouse_event()`**: Core mouse event handler
  - Converts mouse coordinates from unit space to world space via camera_unit_to_world
  - Tests mouse position against GUI bounding boxes using inverse transform matrices
  - Populates event maps with entities that received events
  - Handles focus management for focusable elements
  - Supports event capture to prevent propagation
  - Excludes editable entities when in edit mode
- **`_common_mouse_down()`** and **`_common_mouse_up()`**: Mouse event dispatchers with focus clearing behavior
- **`_common_key_down()`** and **`_common_key_up()`**: Keyboard event handlers that send events to focused entity
- **`_common_char_down()`**: Character input handler for focused entity
- **`_common_event_clear()`**: Clears all event maps and resets captured_event flag

### Serialization
- **`_common_save_all()`**: Saves GUI system state including all GUI component properties (color, visibility, focus settings, alignment, padding)
- **`_common_load_all()`**: Loads GUI system state with default values and reattaches roots

## Rectangle Component

### Data Structure
- **`Rect` struct**: GUI rectangle component containing:
  - `EntityPoolElem pool_elem`: Entity pool integration
  - `Mat3 wmat`: World transformation matrix
  - `Vec2 size`: Rectangle dimensions
  - `bool visible`: Visibility state
  - `Color color`: Rectangle color
  - `bool hfit, vfit`: Horizontal/vertical fit flags
  - `bool hfill, vfill`: Horizontal/vertical fill flags
  - `bool updated`: Update tracking flag
  - `int depth`: Draw order depth (children > parents)

### Functions
- **`gui_rect_add()`**: Adds rectangle component to entity
  - Creates GUI component if not exists
  - Initializes rectangle with default size (64x64)
  - Sets default fit behavior (hfit=true, vfit=true, hfill=false, vfill=false)

## Key Dependencies
- References `entitypool`, `entitymap`, `transform`, `camera`, `input`, `edit`, `bbox`, `color`, `vec2` systems
- Uses global entities: `focused`, `gui_root`, `gui_pool`
- Depends on various data structures: `Entity`, `Mat3`, `Vec2`, `BBox`, `Color`

### GUI Rectangle Component Management and Layout System
Source: [gui.c:579-887](../gui.c#L579-L887)

This section implements a comprehensive GUI rectangle component system that handles entity management, property access, rendering setup, and complex layout algorithms including table alignment, fitting, and filling behaviors.

## Entity Management Functions

• **gui_rect_remove(Entity ent)** - Removes a rectangle entity from the rect_pool
• **gui_rect_has(Entity ent)** - Checks if an entity has a rectangle component by testing if it exists in rect_pool

## Property Accessor Functions

• **Size Management**:
  - `gui_rect_set_size(Entity ent, Vec2 size)` - Sets the rectangle's size property
  - `gui_rect_get_size(Entity ent)` - Returns the rectangle's current size

• **Horizontal Fit Property**:
  - `gui_rect_set_hfit(Entity ent, bool fit)` - Sets horizontal fit behavior
  - `gui_rect_get_hfit(Entity ent)` - Gets horizontal fit state

• **Vertical Fit Property**:
  - `gui_rect_set_vfit(Entity ent, bool fit)` - Sets vertical fit behavior  
  - `gui_rect_get_vfit(Entity ent)` - Gets vertical fit state

• **Horizontal Fill Property**:
  - `gui_rect_set_hfill(Entity ent, bool fill)` - Sets horizontal fill behavior
  - `gui_rect_get_hfill(Entity ent)` - Gets horizontal fill state

• **Vertical Fill Property**:
  - `gui_rect_set_vfill(Entity ent, bool fill)` - Sets vertical fill behavior
  - `gui_rect_get_vfill(Entity ent)` - Gets vertical fill state

## OpenGL Rendering Setup

• **Static Variables**:
  - `rect_program` - OpenGL shader program handle
  - `rect_vao` - Vertex Array Object handle
  - `rect_vbo` - Vertex Buffer Object handle

• **_rect_init()** - Initialization function that:
  - Creates rect_pool using entitypool_new(Rect)
  - Creates shader program from rect.vert, rect.geom, rect.frag files
  - Sets up OpenGL vertex array and buffer objects
  - Binds vertex attributes for world matrix (wmat1, wmat2, wmat3), size, visible, and color

• **_rect_deinit()** - Cleanup function that:
  - Deletes OpenGL program, buffer, and vertex array objects
  - Frees the rect_pool

## Layout Update System

The system implements a complex two-pass layout algorithm with several specialized update functions:

### Table Alignment (_rect_update_table_align)
• Handles GA_TABLE alignment for child elements
• Algorithm steps:
  - Iterates through all children of the rectangle entity
  - For each visible child with table alignment:
    - Updates child layout first via recursive call
    - Calculates bounding box in world coordinates
    - For horizontal table alignment (halign == GA_TABLE):
      - Calculates delta to position child after current x position
      - Updates child position and advances current x
    - For vertical table alignment (valign == GA_TABLE):
      - Calculates delta to position child below current y position
      - Updates child position and advances current y

### Fit Behavior (_rect_update_fit)
• Automatically resizes rectangle to fit its children
• Algorithm steps:
  - Iterates through all visible children
  - Updates each child's layout first
  - Calculates world-space bounding box for each child
  - If hfit is enabled: sets width to maximum child extent plus padding
  - If vfit is enabled: sets height to minimum child extent minus padding

### Fill Behavior (_rect_update_fill)
• Resizes rectangle to fill its parent container
• Algorithm steps:
  - Gets parent entity and updates parent layout first
  - Transforms parent's bounding box to local coordinates
  - If hfill is enabled: sets width to parent width minus padding
  - If vfill is enabled: sets height to parent height minus padding

### Depth Calculation (_rect_update_depth)
• Calculates rendering depth based on hierarchy
• Sets depth to parent's depth + 1, or 0 if no parent

### Update Coordination Functions
• **_rect_update_child_first(Entity ent)** - Updates children before parent, handles table alignment and fitting
• **_rect_update_parent_first(Entity ent)** - Updates parent before children, handles filling and depth
• **_rect_update_all()** - Main update coordinator that:
  - Removes destroyed entities
  - Performs two-pass update (child-first, then parent-first)
  - Synchronizes properties between Rect and Gui components
  - Updates bounding boxes

• **_rect_update_wmat()** - Updates world matrices for all rectangles from transform system

## Key Dependencies
• **entitypool** - For entity component storage and management
• **transform** - For position, hierarchy, and matrix operations  
• **gui_pool** - For accessing base GUI component data
• **gfx** - For OpenGL shader and rendering operations
• **bbox** - For bounding box calculations and transformations

### GUI Rectangle and Text System Implementation
Source: [gui.c:888-1218](../gui.c#L888-L1218)

This section implements the core rendering, persistence, and management functionality for two key GUI components: rectangles and text elements.

## Rectangle System

### Core Functions
- **`_rect_depth_compare`**: Comparison function for depth-sorting rectangles
  - Primary sort: by `depth` field (ascending)
  - Secondary sort: by entity ID when depths are equal
- **`_rect_draw_all`**: Renders all rectangles in the system
  - Sorts rectangles by depth using `entitypool_sort` with `_rect_depth_compare`
  - Binds `rect_program` shader and sets up OpenGL state
  - Uploads all rectangle data to GPU via `rect_vbo` buffer
  - Renders using `glDrawArrays` with `GL_POINTS` primitive

### Persistence Functions
- **`_rect_save_all`**: Serializes all rectangles to storage
  - Saves: `size`, `color`, `hfit`, `vfit`, `hfill`, `vfill` properties
- **`_rect_load_all`**: Deserializes rectangles from storage
  - Default values: size=(64,64), color=gray, hfit/vfit=true, hfill/vfill=false

## Text System

### Data Structures
- **`TextChar`**: Per-character rendering data
  - `pos`: 2D position in text entity space
  - `cell`: Font atlas grid coordinates
  - `is_cursor`: Flag indicating cursor position (>0 for cursor, -1 for regular char)
- **`Text`**: Text entity data
  - `str`: Character string content
  - `chars`: Array of `TextChar` for GPU upload
  - `bounds`: Text dimensions (max x, min y)
  - `cursor`: Cursor position index (-1 if no cursor)

### Core Text Functions
- **`_text_add_cursor`**: Adds cursor character at specified position
- **`_text_set_str`**: Updates text content and rebuilds character array
  - Handles newlines by moving to next line
  - Maps characters to font grid coordinates
  - Updates bounding box calculations
  - Manages cursor positioning

### Public Text API
- **`gui_text_add`**: Creates text component for entity
- **`gui_text_remove`**: Destroys text component and frees memory
- **`gui_text_has`**: Checks if entity has text component
- **`gui_text_set_str`**: Sets text content
- **`gui_text_get_str`**: Gets text content
- **`_text_set_cursor`**: Sets cursor position (internal function)

### Text System Management
- **`_text_init`**: Initializes text rendering system
  - Creates `text_pool` entity pool
  - Sets up `text_program` shader with vertex/geometry/fragment shaders
  - Loads font texture (`font1.png`)
  - Configures OpenGL vertex attributes and buffers
- **`_text_deinit`**: Cleanup function for text system
- **`_text_update_all`**: Per-frame text system updates
  - Manages cursor blinking with `cursor_blink_time`
  - Updates GUI bounding boxes based on text dimensions
  - Handles focus events for cursor activation
- **`_text_draw_all`**: Renders all text entities
  - Sets up shader uniforms including cursor blink state
  - Iterates through visible text entities
  - Uploads character data and renders each text entity separately

### Constants and Configuration
- **Font Grid**: 16x16 character grid (`TEXT_GRID_W`, `TEXT_GRID_H`)
- **Font Size**: 10x12 pixel characters (`TEXT_FONT_W`, `TEXT_FONT_H`)

### Key Dependencies
- Uses `entitypool` system for entity management
- Relies on `transform_get_world_matrix` for positioning
- Integrates with camera system via `camera_get_inverse_view_matrix_ptr`
- Uses `array` data structure for dynamic character storage
- Depends on OpenGL rendering pipeline and shader programs

### Text System Save/Load and TextEdit Widget Implementation
Source: [gui.c:1226-1528](../gui.c#L1226-L1528)

This section completes the text system's persistence functionality and implements a complete text editing widget system for the GUI.

## Text System Persistence

**Functions defined:**
- `_text_save_all(Store *s)` - Saves all text entities to storage
- `_text_load_all(Store *s)` - Loads all text entities from storage

**Save/Load Process:**
- Saves/loads text data under "gui_text" section
- For each text entity in `text_pool`:
  - Saves: `str` (string content) and `cursor` (cursor position)
  - Loads: Creates new `TextChar` array, loads string and cursor (default -1), calls `_text_set_str()` for setup

## TextEdit Widget System

**Core Structure:**
- `TextEdit` struct contains:
  - `EntityPoolElem pool_elem` - Entity pool integration
  - `unsigned int cursor` - Cursor position (0 = beginning)
  - `bool numerical` - Whether input is restricted to numbers

**Entity Pool Management:**
- `textedit_pool` - Global entity pool for text edit widgets
- `gui_textedit_add(Entity ent)` - Adds text edit capability to entity
  - Automatically adds text component via `gui_text_add()`
  - Makes entity focusable via `gui_set_focusable()`
- `gui_textedit_remove(Entity ent)` - Removes text edit from entity
- `gui_textedit_has(Entity ent)` - Checks if entity has text edit component

**Configuration Functions:**
- `gui_textedit_set_numerical(Entity ent, bool numerical)` - Sets numerical input mode
- `gui_textedit_get_numerical(Entity ent)` - Gets numerical input mode
- `gui_textedit_get_num(Entity ent)` - Converts text content to number using `strtof()`

**Cursor Management:**
- `gui_textedit_set_cursor(Entity ent, unsigned int cursor)` - Sets cursor position
- `gui_textedit_get_cursor(Entity ent)` - Gets cursor position  
- `_textedit_fix_cursor(TextEdit *textedit)` - Ensures cursor doesn't exceed string length

**Text Modification:**
- `_textedit_set_str(TextEdit *textedit, const char *str)` - Updates text content
  - Calls `gui_text_set_str()` to update display
  - Marks entity as changed in `changed_map`

## Input Handling System

**Event Processing:**
- `_textedit_key_event(KeyCode key, unsigned int c)` - Unified handler for keyboard input
- `_textedit_char_down(unsigned int c)` - Character input handler
- `_textedit_key_down(KeyCode key)` - Key press handler

**Input Behaviors:**
- **Navigation:**
  - `KC_LEFT` - Move cursor left
  - `KC_RIGHT` - Move cursor right
- **Editing:**
  - `KC_BACKSPACE` - Delete character before cursor
  - `KC_DELETE` - Delete character at cursor
  - Printable characters - Insert at cursor position
- **Completion:**
  - `KC_ENTER`/`KC_ESCAPE` - End editing, unfocus widget
- **Visual feedback:** Sets `cursor_blink_time = 1` for cursor blinking

**String Manipulation Algorithm:**
- For character deletion: Allocates new string, copies before cursor, skips deleted char, copies after
- For character insertion: Allocates new string, copies before cursor, inserts new char, copies after cursor
- Always calls `free(new)` for memory cleanup

## Update and Lifecycle Management

**Update Process (`_textedit_update_all()`):**
- Removes destroyed entities via `entitypool_remove_destroyed()`
- For each text edit widget:
  - Fixes cursor position
  - Shows cursor if focused, hides if not focused (cursor = -1)

**Initialization/Cleanup:**
- `_textedit_init()` - Creates `textedit_pool` using `entitypool_new()`
- `_textedit_deinit()` - Frees `textedit_pool` using `entitypool_free()`

**Persistence:**
- `_textedit_save_all(Store *s)` - Saves all text edit widgets under "gui_textedit"
- `_textedit_load_all(Store *s)` - Loads all text edit widgets with defaults (cursor=0, numerical=false)

## GUI System Integration

**Main GUI Functions:**
- `gui_init()` - Initializes entire GUI system including text editing
- `gui_deinit()` - Cleanup entire GUI system
- `gui_update_all()` - Updates all GUI components including text editing
- `gui_event_clear()` - Clears GUI events

**Root GUI Element:**
- `_create_root()` - Creates root GUI entity with transform and rect components
- `_update_root()` - Updates root element size and positioning relative to camera and window

**Dependencies:**
- Depends on `entity_pool`, `transform`, `gui_rect`, `gui_text`, camera, and game window systems
- Uses global `focused` entity for input targeting
- Integrates with `changed_map` for change tracking

### GUI Main Interface Functions
Source: [gui.c:1529-1573](../gui.c#L1529-L1573)

This section defines the primary public interface functions for the GUI system, providing entry points for rendering, input handling, and data persistence.

## Main Functions Defined

• **`gui_draw_all()`** - Master rendering function
  - Calls `_rect_draw_all()` to render all rectangle elements
  - Calls `_text_draw_all()` to render all text elements
  - Coordinates the complete GUI rendering pipeline

• **Input Event Handlers**
  - **`gui_key_down(KeyCode key)`** - Handles key press events
    - Delegates to `_common_key_down(key)` for shared key handling
    - Delegates to `_textedit_key_down(key)` for text editing specific handling
  - **`gui_char_down(unsigned int c)`** - Handles character input events
    - Delegates to `_common_char_down(c)` for shared character handling
    - Delegates to `_textedit_char_down(c)` for text editing specific handling
  - **`gui_key_up(KeyCode key)`** - Handles key release events
    - Only delegates to `_common_key_up(key)` for shared handling
  - **`gui_mouse_down(MouseCode mouse)`** - Handles mouse button press events
    - Delegates to `_common_mouse_down(mouse)` for shared mouse handling
  - **`gui_mouse_up(MouseCode mouse)`** - Handles mouse button release events
    - Delegates to `_common_mouse_up(mouse)` for shared mouse handling

• **Data Persistence Functions**
  - **`gui_save_all(Store *s)`** - Saves all GUI state to storage
    - Calls `_common_save_all(s)` for shared state
    - Calls `_rect_save_all(s)` for rectangle elements
    - Calls `_text_save_all(s)` for text elements
    - Calls `_textedit_save_all(s)` for text editing state
  - **`gui_load_all(Store *s)`** - Loads all GUI state from storage
    - Calls `_common_load_all(s)` for shared state
    - Calls `_rect_load_all(s)` for rectangle elements
    - Calls `_text_load_all(s)` for text elements
    - Calls `_textedit_load_all(s)` for text editing state

## Key Dependencies

• **Type References**: `KeyCode`, `MouseCode`, `Store`
• **Internal Module Functions**: All functions delegate to private module-specific functions prefixed with underscores
• **GUI Modules**: Common, Rectangle, Text, and TextEdit subsystems

This section serves as the main facade for the GUI system, providing a unified interface while internally coordinating between specialized subsystems for different GUI element types and input handling.


### Related
- [Parent](../)

---
Hash: 3cec58f45ec8d8b05dec3bcff46aabf1f5b8e4db7de234225992b00afb3ddc5d
Generated: 2025-07-07T20:21:19-04:00
