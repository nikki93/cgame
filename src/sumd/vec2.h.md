# Summary: vec2.h

## Table of Contents

- [Vec2 Header - 2D Vector Structure and Operations](#vec2-header---2d-vector-structure-and-operations)

This header file defines a 2D vector structure and comprehensive mathematical operations for 2D vector manipulation.

## Structure Definition
- **Vec2**: A structure containing two `Scalar` components:
  - `x`: horizontal component
  - `y`: vertical component

## Dependencies
- `scalar.h`: Provides the `Scalar` type used for vector components
- `saveload.h`: Provides save/load functionality via `Store` type
- `script_export.h`: Provides `SCRIPT` and `EXPORT` macros for script integration

## Constructor and Constants
- **vec2(x, y)**: Constructor function that creates a Vec2 from two Scalar values
- **vec2_zero**: Predefined constant representing the zero vector (0, 0)
- **vec2 macro**: C inline macro `#define vec2(x, y) ((Vec2) { (x), (y) })` for direct structure initialization

## Mathematical Operations
- **Basic arithmetic operations**:
  - `vec2_add(u, v)`: Vector addition
  - `vec2_sub(u, v)`: Vector subtraction
  - `vec2_mul(u, v)`: Component-wise multiplication
  - `vec2_div(u, v)`: Component-wise division
  - `vec2_neg(v)`: Vector negation

- **Scalar operations**:
  - `vec2_scalar_mul(v, f)`: Multiply vector by scalar
  - `vec2_scalar_div(v, f)`: Divide vector by scalar (v.x/f, v.y/f)
  - `scalar_vec2_div(f, v)`: Divide scalar by vector (f/v.x, f/v.y)

## Geometric Operations
- **Length and normalization**:
  - `vec2_len(v)`: Calculate vector magnitude/length
  - `vec2_normalize(v)`: Return unit vector in same direction
  - `vec2_dist(u, v)`: Calculate distance between two points

- **Dot product**:
  - `vec2_dot(u, v)`: Calculate dot product of two vectors

- **Rotation and angle operations**:
  - `vec2_rot(v, rot)`: Rotate vector by given angle
  - `vec2_atan2(v)`: Calculate angle of vector using atan2

## Persistence Operations
- **vec2_save(v, n, s)**: Save Vec2 to Store with given name
- **vec2_load(v, n, d, s)**: Load Vec2 from Store with given name and default value, returns boolean success status

## Script Integration
The entire interface is wrapped in a `SCRIPT` macro with `EXPORT` declarations, indicating this module is designed to be accessible from a scripting system while maintaining C compatibility.

## Sections

### Vec2 Header - 2D Vector Structure and Operations
Source: [vec2.h:1-41](../vec2.h#L1-L41)

This header file defines a 2D vector structure and comprehensive mathematical operations for 2D vector manipulation.

## Structure Definition
- **Vec2**: A structure containing two `Scalar` components:
  - `x`: horizontal component
  - `y`: vertical component

## Dependencies
- **scalar.h**: Provides the `Scalar` type used for vector components
- **saveload.h**: Enables serialization functionality via `Store` type
- **script_export.h**: Provides the `SCRIPT()` macro and `EXPORT` functionality for exposing definitions to scripting

## Exported Functions

### Construction and Constants
- **vec2(Scalar x, Scalar y)**: Constructor function for creating Vec2 instances
- **vec2_zero**: Pre-defined zero vector constant

### Arithmetic Operations
- **vec2_add(Vec2 u, Vec2 v)**: Vector addition
- **vec2_sub(Vec2 u, Vec2 v)**: Vector subtraction
- **vec2_mul(Vec2 u, Vec2 v)**: Component-wise multiplication
- **vec2_div(Vec2 u, Vec2 v)**: Component-wise division
- **vec2_neg(Vec2 v)**: Vector negation

### Scalar Operations
- **vec2_scalar_mul(Vec2 v, Scalar f)**: Multiply vector by scalar
- **vec2_scalar_div(Vec2 v, Scalar f)**: Divide vector by scalar (v.x/f, v.y/f)
- **scalar_vec2_div(Scalar f, Vec2 v)**: Divide scalar by vector components (f/v.x, f/v.y)

### Mathematical Functions
- **vec2_len(Vec2 v)**: Calculate vector length/magnitude
- **vec2_normalize(Vec2 v)**: Normalize vector to unit length
- **vec2_dot(Vec2 u, Vec2 v)**: Dot product of two vectors
- **vec2_dist(Vec2 u, Vec2 v)**: Distance between two points

### Geometric Operations
- **vec2_rot(Vec2 v, Scalar rot)**: Rotate vector by angle
- **vec2_atan2(Vec2 v)**: Calculate angle of vector using atan2

### Serialization
- **vec2_save(Vec2 *v, const char *n, Store *s)**: Save vector to storage
- **vec2_load(Vec2 *v, const char *n, Vec2 d, Store *s)**: Load vector from storage with default value

## Implementation Details
- Uses `SCRIPT()` macro to expose all definitions to scripting systems
- Includes a C macro definition `#define vec2(x, y) ((Vec2) { (x), (y) })` for convenient Vec2 literal creation
- Header guard `VEC2_H` prevents multiple inclusions
- All functions are marked with `EXPORT` for external visibility


### Related
- [Parent](../)

---
Hash: eeed890623227e0c770327996b4ad05d7abb953efae6e26523084e046d88ff85
Generated: 2025-07-07T20:21:19-04:00
