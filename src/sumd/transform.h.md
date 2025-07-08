# Summary: transform.h

## Table of Contents

- [Transform System Header - 2D Hierarchical Transform Component](#transform-system-header---2d-hierarchical-transform-component)

This header file defines a comprehensive transform component system for an entity-component-system (ECS) architecture, providing hierarchical 2D transformations with parent-child relationships.

## Core Dependencies
- **scalar.h**: Provides `Scalar` type for rotation values
- **vec2.h**: Provides `Vec2` type for 2D positions and scales
- **mat3.h**: Provides `Mat3` type for transformation matrices
- **entity.h**: Provides `Entity` type for entity references
- **script_export.h**: Provides `SCRIPT` and `EXPORT` macros for scripting integration
- **saveload.h**: Provides `Store` type for persistence operations

## Key Design Principles
- **Transformation order**: Scale → Rotation → Position (applied in that order)
- **Hierarchical structure**: Supports parent-child relationships between entities
- **Root transforms**: Have parent set to `entity_nil`

## Component Management Functions
- `transform_add(Entity ent)`: Adds transform component to entity
- `transform_remove(Entity ent)`: Removes transform component from entity
- `transform_has(Entity ent)`: Checks if entity has transform component

## Hierarchy Management
- `transform_set_parent(Entity ent, Entity parent)`: Sets parent-child relationship
- `transform_get_parent(Entity ent)`: Returns parent entity
- `transform_get_num_children(Entity ent)`: Returns number of child entities
- `transform_get_children(Entity ent)`: Returns array of child entities
- `transform_detach_all(Entity ent)`: Detaches entity from parent and all children
- `transform_destroy_rec(Entity ent)`: Recursively destroys entity and all children

## Local Transform Properties
### Position
- `transform_set_position(Entity ent, Vec2 pos)`: Sets local position
- `transform_get_position(Entity ent)`: Gets local position
- `transform_translate(Entity ent, Vec2 trans)`: Applies translation offset

### Rotation
- `transform_set_rotation(Entity ent, Scalar rot)`: Sets local rotation
- `transform_get_rotation(Entity ent)`: Gets local rotation
- `transform_rotate(Entity ent, Scalar rot)`: Applies rotation offset

### Scale
- `transform_set_scale(Entity ent, Vec2 scale)`: Sets local scale
- `transform_get_scale(Entity ent)`: Gets local scale

## World Transform Properties
- `transform_get_world_position(Entity ent)`: Gets position in world space
- `transform_get_world_rotation(Entity ent)`: Gets rotation in world space
- `transform_get_world_scale(Entity ent)`: Gets scale in world space

## Matrix Operations
- `transform_get_world_matrix(Entity ent)`: Returns world-space transformation matrix
- `transform_get_matrix(Entity ent)`: Returns parent-space transformation matrix
- **Special behavior**: Returns `mat3_identity()` for `entity_nil` so root transforms have identity parent-space

## Coordinate Space Conversion
- `transform_local_to_world(Entity ent, Vec2 v)`: Converts point from local to world space
- `transform_world_to_local(Entity ent, Vec2 v)`: Converts point from world to local space

## System Management
- `transform_get_dirty_count(Entity ent)`: Returns dirty state counter for optimization
- `transform_set_save_filter_rec(Entity ent, bool filter)`: Sets save filter recursively for entity and descendants

## System Functions (Non-exported)
- `transform_init()`: Initializes transform system
- `transform_deinit()`: Cleans up transform system
- `transform_update_all()`: Updates all transform calculations
- `transform_save_all(Store *s)`: Saves all transform data to store
- `transform_load_all(Store *s)`: Loads all transform data from store

## Integration Features
- **Scripting support**: All main functions are exported via `SCRIPT` macro
- **Persistence support**: Includes save/load functionality with filtering
- **Performance optimization**: Includes dirty tracking system

## Sections

### Transform System Header - 2D Hierarchical Transform Component
Source: [transform.h:1-69](../transform.h#L1-L69)

This header file defines a comprehensive 2D transform system for entities with hierarchical parent-child relationships. The system manages position, rotation, and scale transformations applied in that specific order.

## Core System Structure

• **Transform Component**: Manages 2D transformations (position, rotation, scale) for entities
• **Hierarchical System**: Supports parent-child relationships between entities with tree-like structures
• **Script Integration**: All main functions are exported via `SCRIPT()` macro for scripting access
• **Save/Load Support**: Includes persistence functionality through the saveload system

## Key Dependencies

• **scalar.h**: Provides `Scalar` type for rotation values
• **vec2.h**: Provides `Vec2` type for 2D positions and scales  
• **mat3.h**: Provides `Mat3` type for transformation matrices
• **entity.h**: Provides `Entity` type for entity references
• **script_export.h**: Enables script binding via `SCRIPT()` and `EXPORT` macros
• **saveload.h**: Provides `Store` type for persistence operations

## Component Management Functions

• **transform_add()**: Adds transform component to an entity
• **transform_remove()**: Removes transform component from an entity  
• **transform_has()**: Checks if entity has transform component

## Hierarchy Management

• **Parent-Child Relationships**:
  - `transform_set_parent()`: Sets parent entity (root transforms use `entity_nil`)
  - `transform_get_parent()`: Retrieves parent entity
  - `transform_get_num_children()`: Gets count of child entities
  - `transform_get_children()`: Returns array of child entities

• **Hierarchy Operations**:
  - `transform_detach_all()`: Detaches entity from parent and all children
  - `transform_destroy_rec()`: Recursively destroys entity and all its children
  - `transform_set_save_filter_rec()`: Recursively sets save filter for entity tree

## Local Transform Properties

• **Position Management**:
  - `transform_set_position()`: Sets local position as `Vec2`
  - `transform_get_position()`: Gets local position
  - `transform_translate()`: Applies translation offset

• **Rotation Management**:
  - `transform_set_rotation()`: Sets local rotation as `Scalar`
  - `transform_get_rotation()`: Gets local rotation
  - `transform_rotate()`: Applies rotation offset

• **Scale Management**:
  - `transform_set_scale()`: Sets local scale as `Vec2`
  - `transform_get_scale()`: Gets local scale

## World Space Calculations

• **World Transform Access**:
  - `transform_get_world_position()`: Calculates world-space position
  - `transform_get_world_rotation()`: Calculates world-space rotation
  - `transform_get_world_scale()`: Calculates world-space scale

• **Matrix Operations**:
  - `transform_get_world_matrix()`: Returns world-space transformation matrix
  - `transform_get_matrix()`: Returns parent-space transformation matrix
  - Special behavior: Returns `mat3_identity()` for `entity_nil` to provide identity parent-space for root transforms

## Coordinate Space Conversion

• **transform_local_to_world()**: Converts vector from entity's local space to world space
• **transform_world_to_local()**: Converts vector from world space to entity's local space

## System Utilities

• **transform_get_dirty_count()**: Returns dirty count for change tracking
• **System Lifecycle**:
  - `transform_init()`: Initializes transform system
  - `transform_deinit()`: Cleans up transform system
  - `transform_update_all()`: Updates all transform calculations

## Persistence System

• **transform_save_all()**: Saves all transform data to Store
• **transform_load_all()**: Loads all transform data from Store

The system implements a standard 2D transformation pipeline where transformations are applied in the order: scale → rotation → position, with full support for hierarchical inheritance of transformations from parent to child entities.


### Related
- [Parent](../)

---
Hash: b020a5dbf40eeca8c68995a46ca0d7d3d93eddb16da25d2ad381046098fc01d8
Generated: 2025-07-07T20:21:19-04:00
