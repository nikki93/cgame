# Summary: vec2.c

## Table of Contents

- [Vec2 2D Vector Math Library Implementation](#vec2-2d-vector-math-library-implementation)

This file implements a comprehensive 2D vector mathematics library with basic arithmetic, geometric operations, and serialization capabilities.

## Defined Entities

### Global Constants
- `vec2_zero`: A global Vec2 constant initialized to (0.0, 0.0)

### Arithmetic Operations
- `vec2_add(Vec2 u, Vec2 v)`: Component-wise addition of two vectors
- `vec2_sub(Vec2 u, Vec2 v)`: Component-wise subtraction of two vectors  
- `vec2_mul(Vec2 u, Vec2 v)`: Component-wise multiplication of two vectors
- `vec2_div(Vec2 u, Vec2 v)`: Component-wise division of two vectors
- `vec2_scalar_mul(Vec2 v, Scalar f)`: Multiply vector by scalar
- `vec2_scalar_div(Vec2 v, Scalar f)`: Divide vector by scalar
- `scalar_vec2_div(Scalar f, Vec2 v)`: Divide scalar by vector components
- `vec2_neg(Vec2 v)`: Negate vector components

### Geometric Operations
- `vec2_len(Vec2 v)`: Calculate vector length using Euclidean distance formula
  - Uses `scalar_sqrt(v.x * v.x + v.y * v.y)`
- `vec2_normalize(Vec2 v)`: Normalize vector to unit length
  - Returns original vector if both components are zero (avoiding division by zero)
  - Otherwise divides vector by its length
- `vec2_dot(Vec2 u, Vec2 v)`: Calculate dot product of two vectors
- `vec2_dist(Vec2 u, Vec2 v)`: Calculate distance between two points
  - Computes length of difference vector

### Rotation and Angle Operations
- `vec2_rot(Vec2 v, Scalar rot)`: Rotate vector by given angle in radians
  - Uses standard 2D rotation matrix formula:
    - x' = x*cos(θ) - y*sin(θ)
    - y' = x*sin(θ) + y*cos(θ)
- `vec2_atan2(Vec2 v)`: Get angle of vector using arctangent
  - Wraps `scalar_atan2(v.y, v.x)`

### Serialization Functions
- `vec2_save(Vec2 *v, const char *n, Store *s)`: Save vector to storage
  - Creates compressed child store
  - Saves x and y components separately using `scalar_save`
- `vec2_load(Vec2 *v, const char *n, Vec2 d, Store *s)`: Load vector from storage
  - Loads x and y components using `scalar_load`
  - Uses default value `d` if loading fails
  - Returns boolean indicating success

### Constructor Function
- `vec2(Scalar x, Scalar y)`: Create Vec2 struct from scalar components
  - Undefines a macro version before defining the function implementation

## Key Dependencies
- `vec2.h`: Header file defining Vec2 struct and function declarations
- `saveload.h`: Serialization system providing Store type and related functions
- Scalar math functions: `scalar_sqrt`, `scalar_cos`, `scalar_sin`, `scalar_atan2`
- Scalar serialization functions: `scalar_save`, `scalar_load`

## Notes
- The `#undef vec2` directive suggests there's a macro version of the constructor that gets replaced by the function implementation
- All operations work with `Scalar` type rather than primitive floating-point types
- The normalization function includes zero-vector safety check
- Serialization uses a compressed storage format for efficiency

## Sections

### Vec2 2D Vector Math Library Implementation
Source: [vec2.c:1-99](../vec2.c#L1-L99)

This file provides a complete implementation of 2D vector mathematics operations for the `Vec2` type. The implementation includes basic arithmetic, geometric operations, and serialization functionality.

## Constants and Global Variables
- `vec2_zero`: A global constant Vec2 initialized to (0.0, 0.0)

## Basic Vector Arithmetic Operations
- `vec2_add(Vec2 u, Vec2 v)`: Component-wise addition of two vectors
- `vec2_sub(Vec2 u, Vec2 v)`: Component-wise subtraction of two vectors  
- `vec2_mul(Vec2 u, Vec2 v)`: Component-wise multiplication of two vectors
- `vec2_div(Vec2 u, Vec2 v)`: Component-wise division of two vectors
- `vec2_neg(Vec2 v)`: Negation of a vector (returns (-v.x, -v.y))

## Scalar-Vector Operations
- `vec2_scalar_mul(Vec2 v, Scalar f)`: Multiplies vector by scalar
- `vec2_scalar_div(Vec2 v, Scalar f)`: Divides vector by scalar
- `scalar_vec2_div(Scalar f, Vec2 v)`: Divides scalar by vector components (returns (f/v.x, f/v.y))

## Geometric Operations
- `vec2_len(Vec2 v)`: Calculates vector magnitude using `scalar_sqrt(v.x * v.x + v.y * v.y)`
- `vec2_normalize(Vec2 v)`: Returns unit vector in same direction
  - Special case: returns original vector if it's the zero vector (0, 0)
  - Otherwise divides vector by its length
- `vec2_dot(Vec2 u, Vec2 v)`: Computes dot product `u.x * v.x + u.y * v.y`
- `vec2_dist(Vec2 u, Vec2 v)`: Calculates Euclidean distance between two points
- `vec2_rot(Vec2 v, Scalar rot)`: Rotates vector by angle using rotation matrix:
  - x' = x * cos(rot) - y * sin(rot)
  - y' = x * sin(rot) + y * cos(rot)
- `vec2_atan2(Vec2 v)`: Returns angle of vector using `scalar_atan2(v.y, v.x)`

## Serialization Functions
- `vec2_save(Vec2 *v, const char *n, Store *s)`: Saves vector to compressed store
  - Saves x and y components separately with names "x" and "y"
- `vec2_load(Vec2 *v, const char *n, Vec2 d, Store *s)`: Loads vector from store
  - Uses default value `d` if loading fails
  - Returns boolean indicating success

## Constructor Function
- `vec2(Scalar x, Scalar y)`: Creates Vec2 instance from x and y components
  - Note: This undefines a macro version before defining the function

## Key Dependencies
- `vec2.h`: Header file defining Vec2 type and function declarations
- `saveload.h`: Provides Store type and serialization functions (`store_child_save_compressed`, `store_child_load`, `scalar_save`, `scalar_load`)
- Scalar math functions: `scalar_sqrt`, `scalar_cos`, `scalar_sin`, `scalar_atan2`
- `Scalar` type: Used for floating-point components and calculations

The implementation follows standard 2D vector mathematics conventions and provides comprehensive functionality for vector manipulation, geometric calculations, and data persistence.


### Related
- [Parent](../)

---
Hash: b45c30f1ca83e4f50f317fedbe66bc398d500073cf5dbaa8bd8930d97c633ba4
Generated: 2025-07-07T20:21:19-04:00
