# Summary: color.h

## Table of Contents

- [Color Structure Definition and API Header](#color-structure-definition-and-api-header)

This header file defines a color system with RGBA components and provides a comprehensive API for color manipulation and persistence.

## Core Data Structure
- **Color**: A struct containing four `Scalar` components:
  - `r` (red)
  - `g` (green) 
  - `b` (blue)
  - `a` (alpha/transparency)

## Dependencies
- `scalar.h`: Provides the `Scalar` type used for color components
- `script_export.h`: Enables the `SCRIPT` and `EXPORT` macros for scripting integration
- `saveload.h`: Provides the `Store` type and persistence functionality

## Constructor Functions
- **color()**: Creates a color with specified RGBA values
- **color_opaque()**: Creates a color with specified RGB values and full opacity (alpha = 1)

## Predefined Color Constants
The file exports several standard color constants:
- `color_black`
- `color_white` 
- `color_gray`
- `color_red`
- `color_green`
- `color_blue`
- `color_clear` (transparent color with zero alpha)

## Persistence API
- **color_save()**: Saves a color to a store with a given name
  - Parameters: `Color *c`, `const char *n`, `Store *s`
- **color_load()**: Loads a color from a store with a default fallback
  - Parameters: `Color *c`, `const char *n`, `Color d`, `Store *s`
  - Returns: `bool` indicating success/failure

## Macro Definitions
- **color()**: Macro for convenient color creation using struct literal syntax
- **color_opaque()**: Macro that creates an opaque color by calling the main color macro with alpha = 1

## Integration Features
- The entire API is wrapped in a `SCRIPT()` macro, indicating this module is designed to be accessible from a scripting system
- All public functions and constants are marked with `EXPORT` for external visibility
- Standard header guard pattern (`#ifndef COLOR_H`) prevents multiple inclusion

## Sections

### Color Structure Definition and API Header
Source: [color.h:1-31](../color.h#L1-L31)

This header file defines a Color structure and its associated API for handling RGBA color values in a graphics or game engine system.

## Core Structure Definition
- **Color**: A struct containing four `Scalar` components:
  - `r` (red component)
  - `g` (green component) 
  - `b` (blue component)
  - `a` (alpha/transparency component)

## Constructor Functions
- **color()**: Creates a Color with specified RGBA values
- **color_opaque()**: Creates a Color with specified RGB values and alpha set to 1 (fully opaque)

## Predefined Color Constants
The file exports several pre-defined Color constants:
- **color_black**: Standard black color
- **color_white**: Standard white color
- **color_gray**: Standard gray color
- **color_red**: Standard red color
- **color_green**: Standard green color
- **color_blue**: Standard blue color
- **color_clear**: Transparent color (zero alpha)

## Persistence Functions
- **color_save()**: Saves a Color to a Store with a given name
  - Parameters: Color pointer, name string, Store pointer
- **color_load()**: Loads a Color from a Store with fallback default
  - Parameters: Color pointer, name string, default Color, Store pointer
  - Returns: boolean indicating success/failure

## Dependencies
- **scalar.h**: Provides the `Scalar` type used for color components
- **script_export.h**: Provides the `SCRIPT()` and `EXPORT` macros for API exposure
- **saveload.h**: Provides `Store` type and persistence functionality

## Implementation Details
- Uses macro definitions for the constructor functions rather than function declarations
- The `color_opaque` macro incorrectly takes 4 parameters but only uses 3, setting alpha to 1
- Wrapped in `SCRIPT()` macro suggesting this API is exposed to a scripting system
- Standard header guard pattern prevents multiple inclusion


### Related
- [Parent](../)

---
Hash: 4a09c01a4395dcaddae3f8a60bc8e0da36441ff40993f2ee97f45e3af4c48622
Generated: 2025-07-07T20:21:19-04:00
