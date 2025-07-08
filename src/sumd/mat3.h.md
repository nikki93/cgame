# Summary: mat3.h

## Table of Contents

- [Mat3 Header - 3x3 Matrix Structure and Operations](#mat3-header---3x3-matrix-structure-and-operations)

This header file defines a 3x3 matrix data structure and its associated operations for 2D transformations and linear algebra.

## Core Data Structure
- **Mat3**: A 3x3 matrix structure containing a 2D array `Scalar m[3][3]`
  - Stored in column-major order for efficient graphics operations
  - Matrix element access: `m.m[column][row]`
  - Visual layout shows columns as vertical vectors in the matrix representation

## Dependencies
- **scalar.h**: Provides the `Scalar` type used for matrix elements
- **vec2.h**: Provides `Vec2` type for 2D vector operations
- **script_export.h**: Provides `SCRIPT` and `EXPORT` macros for code generation
- **saveload.h**: Provides `Store` type and `bool` for serialization functions

## Matrix Construction Functions
- **mat3()**: Creates a matrix from 9 scalar values (m00-m22 in row-major parameter order)
- **mat3_identity()**: Returns a 3x3 identity matrix
- **mat3_scaling_rotation_translation()**: Creates a transformation matrix combining scale, rotation, and translation operations in sequence

## Matrix Operations
- **mat3_mul()**: Multiplies two Mat3 matrices (m * n)
- **mat3_inverse()**: Computes the inverse of a Mat3 matrix
- **mat3_transform()**: Transforms a Vec2 vector by a Mat3 matrix

## Transformation Extraction
- **mat3_get_translation()**: Extracts translation component as Vec2
- **mat3_get_rotation()**: Extracts rotation component as Scalar
- **mat3_get_scale()**: Extracts scale component as Vec2

## Serialization Functions
- **mat3_save()**: Saves a Mat3 to a Store with a given name
- **mat3_load()**: Loads a Mat3 from a Store with a given name and default value

## C Inline Implementations
- **mat3 macro**: Inline constructor macro that creates Mat3 literals with proper column-major ordering
- **mat3_identity macro**: Inline macro that creates identity matrix with 1.0f on diagonal, 0.0f elsewhere

The header uses a SCRIPT wrapper macro system, likely for automatic binding generation to scripting languages, with all functions marked as EXPORT for external visibility.

## Sections

### Mat3 Header - 3x3 Matrix Structure and Operations
Source: [mat3.h:1-69](../mat3.h#L1-L69)

This header file defines a 3x3 matrix structure and its associated operations for 2D transformations and linear algebra.

## Core Structure Definition

• **Mat3**: A 3x3 matrix structure stored in column-major order
  - Contains a 2D array `Scalar m[3][3]`
  - Matrix elements are accessed as `m[column][row]`
  - Visual representation shows how elements map to mathematical matrix notation

## Dependencies

• **scalar.h**: Provides the `Scalar` data type used for matrix elements
• **vec2.h**: Provides `Vec2` type for 2D vector operations and transformations
• **script_export.h**: Provides `SCRIPT` and `EXPORT` macros for API exposure
• **saveload.h**: Provides `Store` type and `bool` for save/load functionality

## Function Declarations (Exported API)

• **mat3()**: Constructor function taking 9 scalar parameters (m00-m22) in row-major order
• **mat3_identity()**: Returns a 3x3 identity matrix
• **mat3_mul()**: Multiplies two Mat3 matrices (m * n)
• **mat3_scaling_rotation_translation()**: Creates transformation matrix from scale, rotation, and translation components
• **mat3_get_translation()**: Extracts translation vector from transformation matrix
• **mat3_get_rotation()**: Extracts rotation scalar from transformation matrix  
• **mat3_get_scale()**: Extracts scale vector from transformation matrix
• **mat3_inverse()**: Computes matrix inverse
• **mat3_transform()**: Applies matrix transformation to a Vec2 vector
• **mat3_save()**: Serializes matrix to storage with given name
• **mat3_load()**: Deserializes matrix from storage with fallback default value

## C Inline Implementations

• **mat3 macro**: Inline constructor macro that creates Mat3 struct literal
  - Takes 9 parameters in row-major order (m00, m01, m02, m10, m11, m12, m20, m21, m22)
  - Maps to column-major storage format in the struct

• **mat3_identity macro**: Inline identity matrix creation
  - Returns Mat3 with 1.0f on diagonal, 0.0f elsewhere
  - Uses the mat3 constructor macro internally

## Key Design Notes

• Matrix storage uses column-major order internally but constructor parameters are in row-major order
• Designed for 2D transformations (scale, rotation, translation operations)
• Includes both function declarations (for external implementation) and inline macro definitions
• Supports serialization/deserialization for persistence


### Related
- [Parent](../)

---
Hash: 6611da6b404b88ded3a934423fb1f4db7feb336c9b7cb06b6863bbc9d19090ea
Generated: 2025-07-07T20:21:19-04:00
