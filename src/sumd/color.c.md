# Summary: color.c

## Table of Contents

- [Color Structure Implementation and Utilities](#color-structure-implementation-and-utilities)

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

## Sections

### Color Structure Implementation and Utilities
Source: [color.c:1-51](../color.c#L1-L51)

This section implements a color system with predefined colors, serialization functions, and color creation utilities.

## Predefined Color Constants

The file defines seven global `Color` constants with RGBA values:
- `color_black`: (0.0, 0.0, 0.0, 1.0)
- `color_white`: (1.0, 1.0, 1.0, 1.0)
- `color_gray`: (0.5, 0.5, 0.5, 1.0)
- `color_red`: (1.0, 0.0, 0.0, 1.0)
- `color_green`: (0.0, 1.0, 0.0, 1.0)
- `color_blue`: (0.0, 0.0, 1.0, 1.0)
- `color_clear`: (0.0, 0.0, 0.0, 0.0) - fully transparent

## Serialization Functions

### `color_save` Function
- **Purpose**: Saves a `Color` structure to a data store
- **Parameters**: 
  - `Color *c`: pointer to color to save
  - `const char *n`: name/key for storage
  - `Store *s`: parent store object
- **Logic**:
  - Creates a child store using `store_child_save`
  - If successful, saves each RGBA component using `scalar_save`
  - Components saved as "r", "g", "b", "a"

### `color_load` Function
- **Purpose**: Loads a `Color` structure from a data store with fallback
- **Parameters**:
  - `Color *c`: pointer to color to populate
  - `const char *n`: name/key to load from
  - `Color d`: default color if loading fails
  - `Store *s`: parent store object
- **Logic**:
  - Attempts to load child store using `store_child_load`
  - If successful, loads each RGBA component using `scalar_load` (defaulting to 0)
  - If loading fails, assigns the default color `d`
  - Returns boolean indicating success

## Color Creation Functions

### `color_opaque` Function
- **Purpose**: Creates an opaque color (alpha = 1.0)
- **Parameters**: `Scalar r, g, b` for RGB components
- **Implementation**: Calls `color(r, g, b, 1)` with alpha set to 1
- **Note**: Uses `#undef color_opaque`, suggesting it may be defined as a macro elsewhere

### `color` Function
- **Purpose**: Creates a color with specified RGBA values
- **Parameters**: `Scalar r, g, b, a` for all four components
- **Implementation**: Returns a `Color` struct literal with the provided values
- **Note**: Uses `#undef color`, suggesting it may be defined as a macro elsewhere

## Dependencies

The implementation depends on:
- `Color` structure (defined in "color.h")
- `Store` type and related functions (`store_child_save`, `store_child_load`)
- `scalar_save` and `scalar_load` functions for component serialization
- `Scalar` type for color component values


### Related
- [Parent](../)

---
Hash: 517457fcf563cd76f8114eace6157d75e10009442f96a06631b26230589e58c5
Generated: 2025-07-07T20:21:19-04:00
