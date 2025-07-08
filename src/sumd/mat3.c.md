# Summary: mat3.c

## Table of Contents

- [3x3 Matrix Operations and Transformations](#3x3-matrix-operations-and-transformations)

This section implements a comprehensive 3x3 matrix mathematics library in C, providing essential matrix operations for 2D transformations and linear algebra.

## Core Functions Defined

• **Matrix Construction Functions:**
  - `mat3()` - Creates a Mat3 from 9 scalar values (m00, m01, m02, m10, m11, m12, m20, m21, m22)
  - `mat3_identity()` - Returns a 3x3 identity matrix with 1.0f on diagonal, 0.0f elsewhere

• **Matrix Arithmetic Operations:**
  - `mat3_mul()` - Performs matrix multiplication of two Mat3 matrices using standard row-column dot product algorithm
    - Implements the mathematical operation: result[i][j] = Σ(m[i][k] * n[k][j])
    - Returns new Mat3 containing the product

• **Transformation Matrix Operations:**
  - `mat3_scaling_rotation_translation()` - Creates a 2D transformation matrix from:
    - `Vec2 scale` - scaling factors for x and y axes
    - `Scalar rot` - rotation angle in radians
    - `Vec2 trans` - translation offsets
    - Combines operations in SRT order using trigonometric functions

• **Matrix Decomposition Functions:**
  - `mat3_get_translation()` - Extracts translation vector from matrix elements m[2][0] and m[2][1]
  - `mat3_get_rotation()` - Calculates rotation angle using `scalar_atan2(m[0][1], m[0][0])`
  - `mat3_get_scale()` - Computes scale factors by calculating magnitude of first two column vectors

• **Matrix Inversion:**
  - `mat3_inverse()` - Computes matrix inverse using cofactor method:
    - Calculates 3x3 cofactor matrix
    - Computes determinant using first row expansion
    - Checks for near-zero determinant (threshold: 10e-8)
    - Divides all elements by determinant if invertible
    - Returns cofactor matrix unchanged if not invertible (TODO: handle properly)

• **Vector Transformation:**
  - `mat3_transform()` - Applies matrix transformation to Vec2:
    - Treats Vec2 as homogeneous coordinate (x, y, 1)
    - Returns transformed 2D point: (m[0][0]*x + m[1][0]*y + m[2][0], m[0][1]*x + m[1][1]*y + m[2][1])

• **Serialization Support:**
  - `mat3_save()` - Saves matrix to Store using compressed child storage
  - `mat3_load()` - Loads matrix from Store with default fallback value
  - Both functions iterate through 3x3 matrix elements using nested loops

## Key Dependencies

• **External Types Referenced:**
  - `Mat3` - 3x3 matrix type with `m[3][3]` member array
  - `Vec2` - 2D vector type with `x`, `y` members  
  - `Scalar` - floating-point scalar type
  - `Store` - serialization storage type

• **External Functions Called:**
  - `scalar_cos()`, `scalar_sin()`, `scalar_atan2()`, `scalar_sqrt()` - trigonometric operations
  - `vec2()` - Vec2 constructor
  - `scalar_save()`, `scalar_load()` - scalar serialization
  - `store_child_save_compressed()`, `store_child_load()` - storage operations

## Implementation Notes

• The file uses `#undef` directives for `mat3_identity` and `mat3`, suggesting these may be defined as macros elsewhere
• Matrix inverse function has incomplete error handling for non-invertible matrices
• All matrix operations follow standard mathematical conventions for 3x3 matrices
• The transformation matrix function creates matrices suitable for 2D graphics transformations

## Sections

### 3x3 Matrix Operations and Transformations
Source: [mat3.c:1-136](../mat3.c#L1-L136)

This section implements a comprehensive 3x3 matrix library with mathematical operations and 2D transformation utilities. The code defines several key functions for matrix manipulation:

## Core Matrix Operations

• **mat3_mul(Mat3 m, Mat3 n)** - Performs standard 3x3 matrix multiplication
  - Implements the mathematical formula for matrix multiplication
  - Returns a new Mat3 with the computed product
  - Uses explicit element-wise calculations for each result position

• **mat3_inverse(Mat3 m)** - Computes the inverse of a 3x3 matrix
  - Uses the adjugate matrix method divided by determinant
  - First calculates all cofactor elements of the adjugate matrix
  - Computes determinant using the first row expansion
  - Handles near-zero determinants (≤ 10e-8) by returning the non-normalized adjugate matrix
  - Includes a TODO comment indicating incomplete handling of non-invertible matrices

## 2D Transformation Functions

• **mat3_scaling_rotation_translation(Vec2 scale, Scalar rot, Vec2 trans)** - Creates a 2D transformation matrix
  - Combines scaling, rotation, and translation into a single transformation matrix
  - Uses trigonometric functions (scalar_cos, scalar_sin) for rotation
  - Applies scaling to the rotation components
  - Places translation values in the third column
  - Returns a homogeneous transformation matrix

• **mat3_transform(Mat3 m, Vec2 v)** - Applies matrix transformation to a 2D vector
  - Transforms a Vec2 point using the matrix
  - Assumes homogeneous coordinates (treats Vec2 as having z=1)
  - Returns the transformed 2D point

## Matrix Property Extractors

• **mat3_get_translation(Mat3 m)** - Extracts translation component
  - Returns Vec2 from the third column (m[2][0], m[2][1])

• **mat3_get_rotation(Mat3 m)** - Extracts rotation angle
  - Uses scalar_atan2(m[0][1], m[0][0]) to compute angle from first column

• **mat3_get_scale(Mat3 m)** - Extracts scaling factors
  - Computes scale by calculating the magnitude of the first two columns
  - Returns Vec2 with x and y scale factors

## Matrix Construction and Utilities

• **mat3_identity()** - Creates a 3x3 identity matrix
  - Returns standard identity matrix with 1.0f on diagonal, 0.0f elsewhere

• **mat3(Scalar m00, Scalar m01, Scalar m02, ...)** - Constructor function
  - Takes 9 scalar parameters for all matrix elements
  - Returns initialized Mat3 structure

## Serialization Support

• **mat3_save(Mat3 *m, const char *n, Store *s)** - Saves matrix to storage
  - Uses nested loops to save all 9 matrix elements
  - Utilizes compressed storage format
  - Depends on scalar_save and Store system

• **mat3_load(Mat3 *m, const char *n, Mat3 d, Store *s)** - Loads matrix from storage
  - Loads all 9 elements using nested loops
  - Falls back to default value 'd' if loading fails
  - Returns boolean indicating success
  - Depends on scalar_load and Store system

## Dependencies

The code references several external entities:
- **Vec2** type and **vec2()** constructor function
- **Scalar** type for floating-point values
- Trigonometric functions: **scalar_cos**, **scalar_sin**, **scalar_atan2**, **scalar_sqrt**
- Serialization functions: **scalar_save**, **scalar_load**
- Storage system: **Store** type, **store_child_save_compressed**, **store_child_load**

The implementation uses #undef directives for mat3_identity and mat3, suggesting these may be defined as macros in the header file.


### Related
- [Parent](../)

---
Hash: 445de177b4df834fcdcab68052297391717bafe3716bd20e07f6240e28b3200e
Generated: 2025-07-07T20:21:19-04:00
