# Summary: transform.c

## Table of Contents

- [Transform System Implementation - Entity Transformations and Hierarchical Relationships](#transform-system-implementation---entity-transformations-and-hierarchical-relationships)
- [Transform System Matrix Operations and Lifecycle Management](#transform-system-matrix-operations-and-lifecycle-management)

This file implements a hierarchical transform component system for entities in what appears to be a game engine or similar application. The system manages 2D transformations with parent-child relationships and matrix caching for performance.

## Core Data Structure

• **Transform struct**: Contains all transformation data for an entity
  - `EntityPoolElem pool_elem`: Entity pool integration
  - `Vec2 position`: Local position
  - `Scalar rotation`: Local rotation angle
  - `Vec2 scale`: Local scale factors
  - `Entity parent`: Parent entity reference (entity_nil if root)
  - `Array *children`: Dynamic array of child entities (NULL if no children)
  - `Mat3 mat_cache`: Cached local transformation matrix
  - `Mat3 worldmat_cache`: Cached world transformation matrix
  - `unsigned int dirty_count`: Counter for tracking modifications

• **Static Variables**:
  - `EntityPool *pool`: Global entity pool for managing Transform components

## Matrix Update System

The system implements an efficient matrix caching mechanism with hierarchical updates:

• **_modified()**: Core update function triggered when transform properties change
  - Increments dirty_count for change tracking
  - Recalculates mat_cache using `mat3_scaling_rotation_translation()`
  - Updates worldmat_cache by multiplying parent's world matrix with local matrix
  - Recursively updates all children's world matrices via `_update_child()`

• **_update_child()**: Recursive helper for updating child transformations
  - Multiplies parent's world matrix with child's local matrix
  - Recursively processes all grandchildren

## Parent-Child Relationship Management

• **Attachment System**:
  - `transform_set_parent()`: Safely changes parent-child relationships
    - Prevents self-parenting
    - Detaches from old parent before attaching to new one
    - Maintains bidirectional parent-child links
    - Triggers matrix updates

• **Detachment System**:
  - `_detach()`: Removes specific parent-child link
    - Clears child's parent reference
    - Removes child from parent's children array using `array_quick_remove()`
  - `_detach_all()`: Completely disconnects transform from hierarchy
    - Detaches from parent
    - Clears all children's parent references
    - Frees children array

## Public API Functions

• **Component Management**:
  - `transform_add()`: Adds transform component with default values (position=0,0, rotation=0, scale=1,1)
  - `transform_remove()`: Removes component and handles cleanup
  - `transform_has()`: Checks if entity has transform component

• **Property Accessors**:
  - Position: `transform_set_position()`, `transform_get_position()`, `transform_translate()`
  - Rotation: `transform_set_rotation()`, `transform_get_rotation()`, `transform_rotate()`
  - Scale: `transform_set_scale()`, `transform_get_scale()`

• **World Space Operations**:
  - `transform_get_world_position()`, `transform_get_world_rotation()`, `transform_get_world_scale()`
  - `transform_get_world_matrix()`, `transform_get_matrix()`
  - `transform_local_to_world()`, `transform_world_to_local()`: Coordinate space conversions

• **Hierarchy Operations**:
  - `transform_get_parent()`, `transform_get_num_children()`, `transform_get_children()`
  - `transform_detach_all()`: Public interface to `_detach_all()`
  - `transform_destroy_rec()`: Recursively destroys entity and all descendants

## System Management

• **Initialization**:
  - `transform_init()`: Creates entity pool
  - `transform_deinit()`: Cleans up children arrays and frees pool

• **Update Cycle**:
  - `transform_update_all()`: Removes destroyed entities and updates edit bboxes

• **Utility Functions**:
  - `transform_get_dirty_count()`: Returns modification counter
  - `transform_set_save_filter_rec()`: Recursively sets save filtering for hierarchy

## Save/Load System

• **Persistence Support**:
  - `transform_save_all()`, `transform_load_all()`: Handle serialization of entire transform system
  - `_children_save()`, `_children_load()`: Special handling for children arrays during serialization
  - Respects entity save filtering to avoid saving filtered entities

## Key Dependencies

• **External Systems Referenced**:
  - `entitypool.h`: Entity pool management
  - `array.h`: Dynamic array implementation
  - `saveload.h`: Serialization system
  - `bbox.h`: Bounding box operations
  - `edit.h`: Editor integration
  - Mathematics types: `Vec2`, `Scalar`, `Mat3`

## Sections

### Transform System Implementation - Entity Transformations and Hierarchical Relationships
Source: [transform.c:1-301](../transform.c#L1-L301)

This section implements a comprehensive transform system for managing entity positions, rotations, scales, and parent-child hierarchical relationships in what appears to be a game engine or graphics system.

## Core Data Structure

• **Transform struct** - Main component containing:
  - `EntityPoolElem pool_elem` - Entity pool management data
  - `Vec2 position` - Local position
  - `Scalar rotation` - Local rotation angle
  - `Vec2 scale` - Local scale factors
  - `Entity parent` - Parent entity reference (entity_nil if root)
  - `Array *children` - Dynamic array of child entities (NULL if empty)
  - `Mat3 mat_cache` - Cached local transformation matrix
  - `Mat3 worldmat_cache` - Cached world transformation matrix
  - `unsigned int dirty_count` - Track modification count

• **Static EntityPool *pool** - Global entity pool for transform storage

## Key Internal Functions

• **_update_child(Transform *parent, Entity ent)**
  - Recursively updates world matrices for a child and all its descendants
  - Multiplies parent's world matrix with child's local matrix
  - Propagates updates down the hierarchy tree

• **_modified(Transform *transform)**
  - Called whenever transform properties change
  - Updates the local transformation matrix using `mat3_scaling_rotation_translation`
  - Recalculates world matrix by combining with parent's world matrix
  - Recursively updates all children's world matrices
  - Increments dirty_count for change tracking

• **_detach(Transform *p, Transform *c)**
  - Removes parent-child relationship between two transforms
  - Clears child's parent reference to entity_nil
  - Removes child from parent's children array using `array_quick_remove`
  - Triggers _modified on child

• **_detach_all(Transform *t)**
  - Completely detaches transform from hierarchy
  - Removes from parent (if exists)
  - Clears all children relationships
  - Frees children array memory

## Public API Functions

### Entity Management
• **transform_add(Entity ent)** - Creates new transform with default values (position=0,0, rotation=0, scale=1,1)
• **transform_remove(Entity ent)** - Removes transform after detaching all relationships
• **transform_has(Entity ent)** - Checks if entity has transform component

### Hierarchy Management
• **transform_set_parent(Entity ent, Entity parent)**
  - Sets parent-child relationship with cycle prevention
  - Handles detachment from old parent and attachment to new parent
  - Manages children arrays automatically

• **transform_get_parent(Entity ent)** - Returns parent entity
• **transform_get_num_children(Entity ent)** - Returns child count
• **transform_get_children(Entity ent)** - Returns pointer to children array
• **transform_detach_all(Entity ent)** - Public wrapper for _detach_all
• **transform_destroy_rec(Entity ent)** - Recursively destroys entity and all descendants

### Local Transform Properties
• **transform_set_position(Entity ent, Vec2 pos)** / **transform_get_position(Entity ent)**
• **transform_translate(Entity ent, Vec2 trans)** - Adds translation to current position
• **transform_set_rotation(Entity ent, Scalar rot)** / **transform_get_rotation(Entity ent)**
• **transform_rotate(Entity ent, Scalar rot)** - Adds rotation to current rotation
• **transform_set_scale(Entity ent, Vec2 scale)** / **transform_get_scale(Entity ent)**

### World Transform Properties
• **transform_get_world_position(Entity ent)** - Extracts position from world matrix
• **transform_get_world_rotation(Entity ent)** - Extracts rotation from world matrix
• **transform_get_world_scale(Entity ent)** - Extracts scale from world matrix

## Key Dependencies
• **EntityPool system** - For entity component storage and retrieval
• **Mat3 matrix operations** - For transformation math (`mat3_mul`, `mat3_scaling_rotation_translation`, `mat3_get_*`)
• **Vec2 vector operations** - For position and scale (`vec2_add`, `vec2`)
• **Array utilities** - For dynamic children storage (`array_new`, `array_foreach`, `array_quick_remove`)
• **Entity system** - For entity references and `entity_destroy`

The system maintains both local and world transformation matrices with efficient caching and hierarchical updates, ensuring parent transformations properly affect all descendants.

### Transform System Matrix Operations and Lifecycle Management
Source: [transform.c:301-468](../transform.c#L301-L468)

This section implements the core matrix operations and system lifecycle management for the transform system, providing essential functionality for coordinate transformations, persistence, and system initialization.

## Matrix and Coordinate Operations

• **transform_get_world_matrix(Entity ent)**
  - Returns the cached world transformation matrix for an entity
  - Returns identity matrix if entity is `entity_nil`
  - Accesses `Transform.worldmat_cache` field

• **transform_get_matrix(Entity ent)**
  - Returns the cached local transformation matrix for an entity
  - Returns identity matrix if entity is `entity_nil`
  - Accesses `Transform.mat_cache` field

• **transform_local_to_world(Entity ent, Vec2 v)**
  - Transforms a vector from local space to world space
  - Uses `mat3_transform()` with the world matrix cache
  - Essential for rendering and physics calculations

• **transform_world_to_local(Entity ent, Vec2 v)**
  - Transforms a vector from world space to local space
  - Uses `mat3_transform()` with the inverse of the world matrix
  - Critical for input handling and collision detection

• **transform_get_dirty_count(Entity ent)**
  - Returns the dirty count for tracking transformation changes
  - Accesses `Transform.dirty_count` field

## Hierarchy Operations

• **transform_set_save_filter_rec(Entity ent, bool filter)**
  - Recursively sets save filter for an entity and all its children
  - Uses `entity_set_save_filter()` for individual entities
  - Traverses the children array to apply filter recursively

## System Lifecycle Management

• **transform_init()**
  - Initializes the transform system
  - Creates the entity pool: `pool = entitypool_new(Transform)`

• **transform_deinit()**
  - Cleans up the transform system
  - Calls `_free_children_arrays()` to free all children arrays
  - Frees the entity pool with `entitypool_free(pool)`

• **_free_children_arrays()** (static helper)
  - Iterates through all transforms in the pool
  - Frees each transform's children array if it exists

• **transform_update_all()**
  - Updates all transforms in the system per frame
  - Removes destroyed entities with `entitypool_remove_destroyed(pool, transform_remove)`
  - Updates edit bounding boxes if edit mode is enabled
  - Uses empty bbox `{ { 0, 0 }, { 0, 0 } }` for edit bbox updates

## Persistence System

### Save Operations
• **transform_save_all(Store *s)**
  - Saves all transform data to storage
  - Iterates through entity pool using `entitypool_save_foreach`
  - Saves each transform's complete state:
    - Position, rotation, scale vectors/scalars
    - Parent entity (with save filter check)
    - Children array (via `_children_save`)
    - Both matrix caches (local and world)
    - Dirty count

• **_children_save(Transform *t, Store *s)** (static helper)
  - Saves only children that pass the save filter
  - Uses `entity_get_save_filter()` to check each child
  - Stores children in "children" subsection

### Load Operations
• **transform_load_all(Store *s)**
  - Loads all transform data from storage
  - Uses `entitypool_load_foreach` to restore transforms
  - Provides sensible defaults:
    - Position: `vec2_zero`
    - Rotation: `0`
    - Scale: `vec2(1, 1)`
    - Parent: `entity_nil`
    - Matrix caches: `mat3_identity()`
    - Dirty count: `0`

• **_children_load(Transform *t, Store *s)** (static helper)
  - Loads children array from storage
  - Initializes `children` to `NULL` (maintains NULL for childless transforms)
  - Creates array only when children exist in the saved data
  - Uses do-while loop to load all children entities

## Key Dependencies
- **Entity system**: `Entity`, `entity_nil`, `entity_eq`, `entity_set_save_filter`, `entity_get_save_filter`
- **Math library**: `Mat3`, `Vec2`, `mat3_identity`, `mat3_transform`, `mat3_inverse`
- **Memory management**: `entitypool_new`, `entitypool_free`, `entitypool_get`, `entitypool_foreach`
- **Storage system**: `Store`, various save/load functions
- **Edit system**: `edit_get_enabled`, `edit_bboxes_update`
- **Array utilities**: `array_new`, `array_free`, `array_foreach`, `array_add_val`


### Related
- [Parent](../)

---
Hash: 264d3b6a5e73553db872b1e7160271b8b1173a98537b833f07220103e99e89e8
Generated: 2025-07-07T20:21:19-04:00
