# Summary: bbox.c

## Table of Contents

- [Bounding Box Utility Functions](#bounding-box-utility-functions)

This section implements core bounding box operations for 2D geometric calculations. A bounding box (BBox) represents a rectangular region defined by minimum and maximum corner points.

## Functions Defined

• **`bbox_merge(BBox a, BBox b)`**
  - Merges two bounding boxes into a single bounding box that encompasses both
  - Returns a new BBox with:
    - min corner: component-wise minimum of both input min corners
    - max corner: component-wise maximum of both input max corners
  - Uses `scalar_min()` and `scalar_max()` utility functions

• **`bbox_bound(Vec2 a, Vec2 b)`**
  - Creates a bounding box from two arbitrary points
  - Returns a new BBox with:
    - min corner: component-wise minimum of the two input points
    - max corner: component-wise maximum of the two input points
  - Ensures proper min/max ordering regardless of input point order

• **`bbox_contains(BBox b, Vec2 p)`**
  - Tests if a point lies within a bounding box (inclusive bounds)
  - Returns `true` if point p is within bounds: `min.x ≤ p.x ≤ max.x` AND `min.y ≤ p.y ≤ max.y`
  - Uses inclusive comparison on all boundaries

• **`bbox(Vec2 min, Vec2 max)`**
  - Constructor function for creating a BBox from min/max corners
  - Directly assigns the provided min and max vectors to the BBox structure
  - No validation of min/max ordering

• **`bbox_transform(Mat3 m, BBox b)`**
  - Applies a 3x3 transformation matrix to a bounding box
  - Algorithm steps:
    1. Transform all four corners of the original bounding box using `mat3_transform()`
    2. Create two intermediate bounding boxes from the transformed corners
    3. Merge the intermediate boxes to get the final transformed bounding box
  - Handles arbitrary transformations (rotation, scaling, translation, etc.)

## Key Dependencies

• **Data Types**: `BBox`, `Vec2`, `Mat3`
• **Vector Operations**: `vec2()` constructor
• **Scalar Operations**: `scalar_min()`, `scalar_max()`
• **Matrix Operations**: `mat3_transform()`
• **Header**: `bbox.h` (included)

The implementation provides a complete set of fundamental bounding box operations commonly needed in 2D graphics, collision detection, and spatial algorithms.

## Sections

### Bounding Box Utility Functions
Source: [bbox.c:1-39](../bbox.c#L1-L39)

This section implements core utility functions for working with 2D bounding boxes (BBox structures). The code provides fundamental operations for creating, manipulating, and querying bounding boxes.

## Defined Functions

• **`bbox_merge(BBox a, BBox b)`**
  - Combines two bounding boxes into a single bounding box that encompasses both
  - Returns a new BBox with:
    - min coordinates: minimum of both input boxes' min coordinates (per axis)
    - max coordinates: maximum of both input boxes' max coordinates (per axis)
  - Uses `scalar_min()` and `scalar_max()` helper functions

• **`bbox_bound(Vec2 a, Vec2 b)`**
  - Creates a bounding box from two arbitrary points
  - Returns a BBox where:
    - min coordinates: minimum values from both points (per axis)
    - max coordinates: maximum values from both points (per axis)
  - Ensures proper min/max ordering regardless of input point order

• **`bbox_contains(BBox b, Vec2 p)`**
  - Tests if a point lies within a bounding box (inclusive boundaries)
  - Returns true if point p is within bounds: `b.min.x ≤ p.x ≤ b.max.x` AND `b.min.y ≤ p.y ≤ b.max.y`

• **`bbox(Vec2 min, Vec2 max)`**
  - Constructor function for creating BBox structures
  - Takes explicit min and max corner points
  - Returns initialized BBox with the provided min/max values

• **`bbox_transform(Mat3 m, BBox b)`**
  - Applies a 3x3 matrix transformation to a bounding box
  - Algorithm:
    1. Transforms all four corners of the original bounding box using `mat3_transform()`
    2. Corner points: (min.x, min.y), (max.x, min.y), (max.x, max.y), (min.x, max.y)
    3. Creates bounding boxes from pairs of transformed corners using `bbox_bound()`
    4. Merges the resulting bounding boxes using `bbox_merge()` to get final encompassing box

## Key Dependencies

• **BBox structure**: 2D bounding box type with `.min` and `.max` Vec2 fields
• **Vec2 structure**: 2D vector type with `.x` and `.y` scalar fields  
• **Mat3 structure**: 3x3 matrix type for transformations
• **Helper functions**: `scalar_min()`, `scalar_max()`, `vec2()`, `mat3_transform()`

This implementation provides a complete set of basic bounding box operations commonly needed in 2D graphics, collision detection, and spatial partitioning systems.


### Related
- [Parent](../)

---
Hash: b9ba218429bd2869df1c45aa6b6d71a1051e32757aac6c176856b4f85dde194b
Generated: 2025-07-07T20:21:19-04:00
