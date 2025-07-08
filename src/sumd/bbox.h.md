# Summary: bbox.h

## Table of Contents

- [BBox Header - 2D Bounding Box Structure and Operations](#bbox-header---2d-bounding-box-structure-and-operations)

This header file defines a 2D bounding box data structure and associated operations for spatial calculations and transformations.

## Core Data Structure
- **BBox**: A structure representing a 2D axis-aligned bounding box
  - Contains two `Vec2` fields: `min` and `max`
  - Represents the minimum and maximum coordinates of the bounding box

## Dependencies
- **vec2.h**: Provides the `Vec2` type used for the min/max coordinates
- **mat3.h**: Provides the `Mat3` type used for transformations
- **script_export.h**: Provides the `SCRIPT` and `EXPORT` macros for code generation/binding

## Exported Functions
- **bbox(Vec2 min, Vec2 max)**: Constructor function that creates a BBox from minimum and maximum coordinate vectors
- **bbox_bound(Vec2 a, Vec2 b)**: Creates a bounding box that encompasses two points
- **bbox_merge(BBox a, BBox b)**: Combines two bounding boxes into a single bounding box that contains both
- **bbox_contains(BBox b, Vec2 p)**: Returns a boolean indicating whether a point is contained within the bounding box
- **bbox_transform(Mat3 m, BBox b)**: Applies a 3x3 transformation matrix to a bounding box and returns the resulting bounding box that encompasses the transformed box

## Code Organization Features
- Uses header guards (`#ifndef BBOX_H`, `#define BBOX_H`) to prevent multiple inclusions
- All definitions are wrapped in a `SCRIPT` macro block, suggesting this code is designed to be exported to a scripting system
- All function declarations use the `EXPORT` macro, indicating they are part of a public API

This header provides a complete interface for 2D bounding box operations commonly needed in graphics, collision detection, and spatial partitioning systems.

## Sections

### BBox Header - 2D Bounding Box Structure and Operations
Source: [bbox.h:1-16](../bbox.h#L1-L16)

This header file defines a 2D bounding box structure and its associated operations for geometric computations.

## Structure Definition
- **BBox**: A structure representing a 2D axis-aligned bounding box
  - `Vec2 min`: The minimum corner coordinates (bottom-left)
  - `Vec2 max`: The maximum corner coordinates (top-right)

## Dependencies
- **vec2.h**: Required for `Vec2` type used in bounding box corners
- **mat3.h**: Required for `Mat3` type used in transformation operations
- **script_export.h**: Required for `SCRIPT` and `EXPORT` macros that handle code generation/binding

## Exported Functions
- **bbox(Vec2 min, Vec2 max)**: Constructor function that creates a BBox from minimum and maximum corner points
- **bbox_bound(Vec2 a, Vec2 b)**: Creates a bounding box that encompasses two arbitrary points (automatically determines min/max)
- **bbox_merge(BBox a, BBox b)**: Combines two bounding boxes to create a new bounding box that contains both
- **bbox_contains(BBox b, Vec2 p)**: Tests whether a point lies within the given bounding box (returns boolean)
- **bbox_transform(Mat3 m, BBox b)**: Applies a 3x3 transformation matrix to a bounding box and returns the axis-aligned bounding box of the transformed result

## Implementation Notes
- Uses header guard `BBOX_H` to prevent multiple inclusions
- All functionality is wrapped in a `SCRIPT()` macro, indicating this code is likely part of a scripting interface or code generation system
- The `EXPORT` macro suggests these functions are made available to external systems or scripting languages


### Related
- [Parent](../)

---
Hash: 16096a7c2e0f0e694ac0dc1fc4a444a2cf171d5a4aa45d51cb4de289df124249
Generated: 2025-07-07T20:21:19-04:00
