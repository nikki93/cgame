# Summary: physics.c

## Table of Contents

- [Physics System Core - Entity Management and Shape Creation](#physics-system-core---entity-management-and-shape-creation)
- [Physics Shape Properties and Dynamics API](#physics-shape-properties-and-dynamics-api)
- [Physics System - Collision Queries, Initialization, Updates, and Rendering](#physics-system---collision-queries-initialization-updates-and-rendering)
- [Physics System Serialization - Body and Shape Save/Load Implementation](#physics-system-serialization---body-and-shape-saveload-implementation)

This file implements a comprehensive physics system that wraps the Chipmunk physics engine for use in an entity-component-system (ECS) architecture. It provides 2D rigid body physics simulation with collision detection, shape management, and integration with a transform system.

## Core Data Structures

• **PhysicsInfo**: Per-entity physics information containing:
  - `EntityPoolElem pool_elem`: Entity pool management
  - `PhysicsBody type`: Body type (static, dynamic, kinematic)
  - `Scalar mass`: Mass stored separately for type conversions
  - `cpVect last_pos`, `cpFloat last_ang`: Previous position/angle for kinematic bodies
  - `unsigned int last_dirty_count`: Transform synchronization tracking
  - `cpBody *body`: Chipmunk physics body
  - `Array *shapes`: Collection of attached collision shapes
  - `Array *collisions`: Current frame collision information

• **ShapeInfo**: Per-shape information containing:
  - `PhysicsShape type`: Shape type (circle, polygon)
  - `cpShape *shape`: Chipmunk shape object

## Global State Management

• **Static Variables**:
  - `cpSpace *space`: Main Chipmunk physics simulation space
  - `Scalar period`: Fixed timestep for physics simulation (default 1/60 second)
  - `EntityPool *pool`: Entity pool for managing PhysicsInfo instances
  - `EntityMap *debug_draw_map`: Tracks entities marked for debug visualization

## Physics Body Management

• **Body Creation and Removal**:
  - `physics_add()`: Creates physics component for entity, initializes Chipmunk body
  - `physics_remove()`: Removes all physics objects and cleans up memory
  - `physics_has()`: Checks if entity has physics component

• **Body Type Management**:
  - `physics_set_type()`, `physics_get_type()`: Manage body types (PB_STATIC, PB_DYNAMIC, PB_KINEMATIC)
  - `_set_type()`: Internal function handling Chipmunk body type conversions
  - Special handling for kinematic bodies to track velocity computation

## Shape Management System

• **Shape Creation Functions**:
  - `physics_shape_add_circle()`: Creates circular collision shape
  - `physics_shape_add_box()`: Creates rectangular collision shape
  - `physics_shape_add_poly()`: Creates convex polygon shape with automatic convex hull computation
  - `_shape_add()`: Internal function for common shape initialization

• **Shape Properties**:
  - `physics_shape_set_surface_velocity()`, `physics_shape_get_surface_velocity()`: Surface velocity for conveyor belt effects
  - `physics_shape_set_sensor()`, `physics_shape_get_sensor()`: Sensor shapes (detect collisions without physical response)
  - `physics_shape_remove()`: Removes individual shapes and recalculates moment of inertia

• **Shape Utilities**:
  - `physics_convex_hull()`: Computes convex hull of vertex array
  - `_recalculate_moment()`: Recalculates moment of inertia based on all shapes

## Dynamics and Forces

• **Mass and Inertia**:
  - `physics_set_mass()`, `physics_get_mass()`: Mass management
  - `physics_set_freeze_rotation()`, `physics_get_freeze_rotation()`: Rotation locking

• **Velocity and Angular Velocity**:
  - `physics_set_velocity()`, `physics_get_velocity()`: Linear velocity
  - `physics_set_angular_velocity()`, `physics_get_angular_velocity()`: Angular velocity
  - Velocity limit functions for both linear and angular motion

• **Force Application**:
  - `physics_apply_force()`, `physics_apply_force_at()`: Apply forces
  - `physics_apply_impulse()`, `physics_apply_impulse_at()`: Apply impulses
  - `physics_reset_forces()`: Clear accumulated forces

## Collision Detection

• **Collision Queries**:
  - `physics_get_num_collisions()`, `physics_get_collisions()`: Access collision information
  - `_add_collision()`: Callback for gathering collisions from Chipmunk
  - `_update_collisions()`: Lazy collision list building

• **Spatial Queries**:
  - `physics_nearest()`: Find nearest shape to a point within maximum distance

## Simulation Update Loop

• **Main Update Process** (`physics_update_all()`):
  1. Remove destroyed entities from physics system
  2. Clear debug draw map
  3. Update kinematic bodies from transform system
  4. Step physics simulation with fixed timestep
  5. Synchronize transforms with physics bodies (bidirectional)
  6. Update dirty count tracking

• **Kinematic Body Updates** (`_update_kinematics()`):
  - Computes velocities based on position changes
  - Updates Chipmunk body positions from transform system
  - Reindexes spatial hash for collision detection

• **Fixed Timestep Simulation** (`_step()`):
  - Accumulates frame time and steps simulation at fixed intervals
  - Ensures consistent physics behavior regardless of framerate

## Debug Visualization

• **Debug Drawing System**:
  - OpenGL-based wireframe rendering of physics shapes
  - `physics_draw_all()`: Renders all shapes marked for debug display
  - `_circle_draw()`, `_polygon_draw()`: Shape-specific rendering functions
  - Integration with camera system for proper viewport transformation

## Serialization System

• **Save/Load Infrastructure**:
  - `physics_save_all()`, `physics_load_all()`: Complete physics state persistence
  - `_body_save()`, `_body_load()`: Chipmunk body properties
  - `_shapes_save()`, `_shapes_load()`: Shape collection with type-specific data
  - Automatic restoration of entity references and physics object relationships

## Key Dependencies

• **External Systems**:
  - Chipmunk Physics Engine (`chipmunk.h`)
  - Transform system for position/rotation synchronization
  - Entity pool management system
  - Graphics system for debug visualization
  - Timing system for fixed timestep simulation
  - Store system for serialization

• **Utility Functions**:
  - `cpv_of_vec2()`, `vec2_of_cpv()`: Vector format conversions
  - `_remove_body()`, `_remove_shape()`: Safe Chipmunk object cleanup

This physics system provides a complete 2D rigid body physics solution with proper integration into the game engine's ECS architecture, including transform synchronization, collision detection, debug visualization, and full serialization support.

## Sections

### Physics System Core - Entity Management and Shape Creation
Source: [physics.c:1-306](../physics.c#L1-L306)

This section implements the core physics system using the Chipmunk physics library, managing physics entities and their shapes within a game engine architecture.

## Key Data Structures

• **PhysicsInfo**: Per-entity physics information structure containing:
  - `EntityPoolElem pool_elem`: Pool management data
  - `PhysicsBody type`: Body type (static, dynamic, kinematic)
  - `Scalar mass`: Entity mass for dynamic conversion
  - `cpVect last_pos`, `cpFloat last_ang`: Previous position/angle for kinematic bodies
  - `unsigned int last_dirty_count`: Transform synchronization tracking
  - `cpBody *body`: Chipmunk physics body
  - `Array *shapes`: Collection of attached shapes
  - `Array *collisions`: Collision data storage

• **ShapeInfo**: Per-shape information structure containing:
  - `PhysicsShape type`: Shape type identifier
  - `cpShape *shape`: Chipmunk shape object

## Global System State

• **Static variables**:
  - `cpSpace *space`: Chipmunk physics world space
  - `Scalar period`: Simulation timestep (default 1/60 seconds)
  - `EntityPool *pool`: Entity management pool
  - `EntityMap *debug_draw_map`: Debug visualization tracking

## Core Physics Management Functions

• **Gravity control**:
  - `physics_set_gravity(Vec2 g)`: Sets world gravity vector
  - `physics_get_gravity()`: Returns current gravity vector

• **Simulation frequency control**:
  - `physics_set_simulation_frequency(Scalar freq)`: Sets physics update rate
  - `physics_get_simulation_frequency()`: Returns current update rate

• **Entity lifecycle management**:
  - `physics_add(Entity ent)`: Adds physics component to entity
    - Creates PhysicsInfo structure
    - Initializes Chipmunk body with default mass (1.0) and dynamic type
    - Synchronizes with transform system
    - Sets up shape and collision arrays
  - `physics_remove(Entity ent)`: Removes physics from entity
    - Calls internal `_remove()` to clean up Chipmunk objects
    - Removes from entity pool
  - `physics_has(Entity ent)`: Checks if entity has physics component

## Physics Body Type Management

• **`_set_type(PhysicsInfo *info, PhysicsBody type)`**: Internal function to change body type
  - **PB_KINEMATIC/PB_STATIC**: Converts to static body, removes from space
  - **PB_DYNAMIC**: Converts to dynamic body, adds to space, recalculates moment of inertia

• **`physics_set_type(Entity ent, PhysicsBody type)`**: Public interface for body type changes
• **`physics_get_type(Entity ent)`**: Returns current body type

## Shape Management System

• **Shape addition workflow** (`_shape_add()`):
  1. Creates ShapeInfo structure and adds to entity's shapes array
  2. Associates shape with Chipmunk body
  3. Adds shape to physics space
  4. Sets default friction (1.0) and user data mapping
  5. Updates moment of inertia for dynamic bodies

• **Shape creation functions**:
  - `physics_shape_add_circle(Entity ent, Scalar r, Vec2 offset)`: Creates circular collision shape
  - `physics_shape_add_box(Entity ent, BBox b, Scalar r)`: Creates rectangular collision shape with rounded corners
  - `physics_shape_add_poly(Entity ent, unsigned int nverts, const Vec2 *verts, Scalar r)`: Creates polygon collision shape
    - Performs convex hull calculation on input vertices
    - Uses temporary cpVect array for Chipmunk compatibility

• **`physics_get_num_shapes(Entity ent)`**: Returns number of shapes attached to entity

## Moment of Inertia Calculation

• **`_moment(cpBody *body, ShapeInfo *shapeInfo)`**: Calculates moment for individual shapes
  - **PS_CIRCLE**: Uses `cpMomentForCircle()` with radius and offset
  - **PS_POLYGON**: Uses `cpMomentForPoly()` with vertex data

• **`_recalculate_moment(PhysicsInfo *info)`**: Recalculates total moment by summing all shape moments

## Utility Functions

• **Vector conversion**:
  - `cpv_of_vec2(Vec2 v)`: Converts engine Vec2 to Chipmunk cpVect
  - `vec2_of_cpv(cpVect v)`: Converts Chipmunk cpVect to engine Vec2

• **Resource cleanup**:
  - `_remove_body(cpBody *body)`: Safely removes and frees Chipmunk body
  - `_remove_shape(cpShape *shape)`: Safely removes and frees Chipmunk shape

• **Debug visualization**:
  - `physics_debug_draw(Entity ent)`: Marks entity for debug rendering

## Key Dependencies

• **External systems**: `transform`, `entitypool`, `array`, `error`, `timing`, `gfx`, `camera`, `edit`, `entitymap`
• **Chipmunk physics library**: Complete integration for physics simulation
• **Entity system**: Uses Entity type for object identification and management

### Physics Shape Properties and Dynamics API
Source: [physics.c:307-565](../physics.c#L307-L565)

This section implements the public API for managing physics shape properties and rigid body dynamics in what appears to be a physics engine wrapper around the Chipmunk physics library.

## Shape Management Functions

• **physics_shape_get_type(Entity ent, unsigned int i)**
  - Returns the `PhysicsShape` type for shape `i` of entity `ent`
  - Validates entity exists and shape index is within bounds

• **physics_shape_remove(Entity ent, unsigned int i)**
  - Removes shape `i` from entity `ent`
  - Calls `_remove_shape()` to clean up the underlying physics shape
  - Uses `array_quick_remove()` for efficient removal
  - Recalculates moment of inertia via `_recalculate_moment()`

• **_get_shape(PhysicsInfo *info, unsigned int i)** (static helper)
  - Internal function to safely retrieve `ShapeInfo` pointer
  - Validates shape index is within bounds

## Polygon Shape Functions

• **physics_poly_get_num_verts(Entity ent, unsigned int i)**
  - Returns number of vertices for a polygon shape
  - Calls Chipmunk's `cpPolyShapeGetNumVerts()`

• **physics_convex_hull(unsigned int nverts, Vec2 *verts)**
  - Computes convex hull of input vertices
  - Algorithm steps:
    1. Converts `Vec2` array to Chipmunk `cpVect` array
    2. Calls `cpConvexHull()` to compute hull
    3. Converts result back to `Vec2` array
    4. Returns new vertex count
  - Modifies input vertex array in-place

## Shape Surface Properties

• **physics_shape_set_surface_velocity(Entity ent, unsigned int i, Vec2 v)**
  - Sets surface velocity for shape (used for conveyor belt effects)

• **physics_shape_get_surface_velocity(Entity ent, unsigned int i)**
  - Gets current surface velocity

• **physics_shape_set_sensor(Entity ent, unsigned int i, bool sensor)**
  - Configures shape as sensor (collision detection without physical response)

• **physics_shape_get_sensor(Entity ent, unsigned int i)**
  - Returns sensor status

## Rigid Body Dynamics Properties

• **Mass Management**
  - `physics_set_mass(Entity ent, Scalar mass)`: Sets mass, ignores values ≤ SCALAR_EPSILON
  - `physics_get_mass(Entity ent)`: Returns current mass

• **Rotation Control**
  - `physics_set_freeze_rotation(Entity ent, bool freeze)`: Freezes/unfreezes rotation by setting moment to infinity
  - `physics_get_freeze_rotation(Entity ent)`: Checks if rotation is frozen by testing if moment equals infinity

• **Linear Motion**
  - `physics_set_velocity(Entity ent, Vec2 vel)`: Sets linear velocity
  - `physics_get_velocity(Entity ent)`: Gets current linear velocity
  - `physics_set_force(Entity ent, Vec2 force)`: Sets applied force
  - `physics_get_force(Entity ent)`: Gets current force

• **Angular Motion**
  - `physics_set_angular_velocity(Entity ent, Scalar ang_vel)`: Sets angular velocity
  - `physics_get_angular_velocity(Entity ent)`: Gets current angular velocity
  - `physics_set_torque(Entity ent, Scalar torque)`: Sets applied torque
  - `physics_get_torque(Entity ent)`: Gets current torque

• **Velocity Limits**
  - `physics_set_velocity_limit(Entity ent, Scalar lim)`: Sets maximum linear velocity
  - `physics_get_velocity_limit(Entity ent)`: Gets velocity limit
  - `physics_set_angular_velocity_limit(Entity ent, Scalar lim)`: Sets maximum angular velocity
  - `physics_get_angular_velocity_limit(Entity ent)`: Gets angular velocity limit

## Force and Impulse Application

• **physics_reset_forces(Entity ent)**: Clears all applied forces

• **Force Application**
  - `physics_apply_force(Entity ent, Vec2 force)`: Applies force at center of mass
  - `physics_apply_force_at(Entity ent, Vec2 force, Vec2 at)`: Applies force at specific point

• **Impulse Application**
  - `physics_apply_impulse(Entity ent, Vec2 impulse)`: Applies impulse at center of mass
  - `physics_apply_impulse_at(Entity ent, Vec2 impulse, Vec2 at)`: Applies impulse at specific point

## Collision Detection System

• **_add_collision(cpBody *body, cpArbiter *arbiter, void *collisions)** (static)
  - Callback function to collect collision data
  - Ensures consistent body ordering in collision pairs
  - Creates `Collision` struct with entity references from both bodies

• **_update_collisions(PhysicsInfo *info)** (static)
  - Lazy collision data collection
  - Only processes if collisions haven't been gathered yet
  - Uses `cpBodyEachArbiter()` to iterate through all collision arbiters

## Key Dependencies

- **Chipmunk Physics**: All cp* functions (cpBodySetMass, cpShapeSetSensor, etc.)
- **Entity System**: `Entity` type, `entitypool_get()`, `pool` global
- **Data Structures**: `PhysicsInfo`, `ShapeInfo`, `Collision` structs
- **Utilities**: `array_*` functions, `vec2_of_cpv()`, `cpv_of_vec2()` conversion functions

### Physics System - Collision Queries, Initialization, Updates, and Rendering
Source: [physics.c:566-894](../physics.c#L566-L894)

This section implements the core physics system functionality including collision detection, system lifecycle management, physics simulation updates, and debug rendering capabilities.

## Collision Query Functions

• **`physics_get_num_collisions(Entity ent)`**
  - Retrieves the number of collisions for a specific entity
  - Calls `_update_collisions()` to ensure collision data is current
  - Returns the length of the entity's collisions array

• **`physics_get_collisions(Entity ent)`**
  - Returns a pointer to the collision array for a specific entity
  - Also calls `_update_collisions()` to refresh collision data
  - Provides direct access to collision data structures

## Spatial Queries

• **`physics_nearest(Vec2 point, Scalar max_dist)`**
  - Finds the nearest physics shape to a given point within maximum distance
  - Uses Chipmunk's `cpSpaceNearestPointQueryNearest()` function
  - Returns `NearestResult` struct containing:
    - `ent`: The nearest entity (or `entity_nil` if none found)
    - `p`: The nearest point on the shape
    - `d`: Distance to the nearest point
    - `g`: Gradient vector at the nearest point

## System Initialization and Cleanup

• **`physics_init()`**
  - Initializes core data structures:
    - `pool`: EntityPool for PhysicsInfo objects
    - `debug_draw_map`: EntityMap for debug rendering tracking
  - Creates Chipmunk physics space with gravity set to (0, -9.8)
  - Sets up OpenGL rendering resources:
    - Shader program from "phypoly.vert" and "phypoly.frag"
    - Vertex Array Object (VAO) and Vertex Buffer Object (VBO)
    - Binds vertex attributes for position data

• **`physics_deinit()`**
  - Cleans up in reverse order of initialization:
    - Removes all physics bodies and shapes via `_remove()`
    - Frees Chipmunk space with `cpSpaceFree()`
    - Destroys OpenGL resources (program, buffers, VAOs)
    - Frees entity pools and maps

## Physics Simulation Updates

• **`_step()`** - Fixed timestep physics simulation
  - Accumulates frame time (`timing_dt`) into a remainder
  - Steps physics simulation in fixed increments (`period`)
  - Ensures consistent physics behavior regardless of framerate

• **`_update_kinematics()`** - Kinematic body updates
  - Updates kinematic bodies based on transform changes
  - Calculates velocities from position/rotation deltas
  - Only processes bodies with type `PB_KINEMATIC`
  - Reindexes shapes in physics space after updates

• **`physics_update_all()`** - Main physics update loop
  - Removes destroyed entities from physics simulation
  - Clears debug draw map for fresh rendering data
  - Performs physics simulation (if not paused):
    - Updates kinematic bodies
    - Steps physics simulation
  - Synchronizes transform and physics data:
    - If transform is newer: updates physics from transform
    - If physics is newer (dynamic bodies): updates transform from physics
    - Tracks dirty counts to avoid unnecessary updates

• **`physics_post_update_all()`** - Post-update cleanup
  - Removes destroyed entities again (safety measure)
  - Clears all collision arrays to prepare for next frame

## Debug Rendering System

• **`_circle_draw(PhysicsInfo *info, ShapeInfo *shapeInfo)`**
  - Renders circular physics shapes as wireframe
  - Uses predefined octagon vertices scaled by radius
  - Applies world transformation matrix and shape offset
  - Draws as `GL_LINE_LOOP` and `GL_POINTS`

• **`_polygon_draw(PhysicsInfo *info, ShapeInfo *shapeInfo)`**
  - Renders polygon physics shapes as wireframe
  - Extracts vertices from Chipmunk polygon shape
  - Converts to Vec2 array for OpenGL rendering
  - Draws as `GL_LINE_LOOP` and `GL_POINTS`

• **`physics_draw_all()`**
  - Only renders when edit mode is enabled
  - Sets up shader uniforms (inverse view matrix)
  - Iterates through all physics entities marked for debug drawing
  - Dispatches to appropriate shape-specific drawing functions

## Data Persistence Helpers

• **Chipmunk data conversion functions:**
  - `_cpv_save()` and `_cpv_load()`: Convert between cpVect and Vec2 for serialization
  - `_cpf_save()` and `_cpf_load()`: Convert between cpFloat and Scalar for serialization
  - Default value constants for loading missing data

## Key Dependencies

• **External libraries:** Chipmunk Physics (cp* functions), OpenGL (gl* functions)
• **Internal systems:** Entity management, Transform system, Timing system, Camera system, Graphics utilities
• **Data structures:** PhysicsInfo, ShapeInfo, NearestResult, Collision

### Physics System Serialization - Body and Shape Save/Load Implementation
Source: [physics.c:897-1141](../physics.c#L897-L1141)

This section implements a comprehensive serialization system for physics entities, providing save and load functionality for physics bodies and shapes using the Chipmunk physics engine.

## Core Serialization Macros

• **`body_prop_save(type, f, n, prop)`** - Macro for saving individual body properties
  - Gets property value using `cpBodyGet##prop(info->body)`
  - Calls corresponding save function `f##_save(&v, n, body_s)`

• **`body_prop_load(type, f, n, prop)`** - Macro for loading individual body properties
  - Loads property value using `f##_load(&v, n, f##_default, body_s)`
  - Sets property using `cpBodySet##prop(info->body, v)`

• **`body_props_saveload(saveload)`** - Master macro that handles all body properties:
  - Mass (`cpFloat`)
  - Moment (`cpFloat`)
  - Velocity (`cpVect`)
  - Force (`cpVect`)
  - Angular velocity (`cpFloat`)
  - Torque (`cpFloat`)
  - Velocity limit (`cpFloat`)
  - Angular velocity limit (`cpFloat`)
  - Note: Position and angle are commented out (handled separately via transform system)

## Body Serialization Functions

• **`_body_save(PhysicsInfo *info, Store *s)`**
  - Creates child store named "body"
  - Uses `body_props_saveload(save)` to save all properties

• **`_body_load(PhysicsInfo *info, Store *s)`**
  - Loads from "body" child store
  - Creates new Chipmunk body: `cpSpaceAddBody(space, cpBodyNew(info->mass, 1.0))`
  - Restores all properties using `body_props_saveload(load)`
  - Sets user data to entity reference
  - Forces type change from default `PB_DYNAMIC` to saved type
  - Restores position and angle from transform system
  - Updates dirty count tracking

## Shape-Specific Serialization

### Circle Shapes
• **`_circle_save(PhysicsInfo *info, ShapeInfo *shapeInfo, Store *s)`**
  - Saves radius using `cpCircleShapeGetRadius()`
  - Saves offset using `cpCircleShapeGetOffset()`

• **`_circle_load(PhysicsInfo *info, ShapeInfo *shapeInfo, Store *s)`**
  - Loads radius and offset
  - Creates shape: `cpCircleShapeNew(info->body, radius, offset)`
  - Adds to space: `cpSpaceAddShape(space, shapeInfo->shape)`

### Polygon Shapes
• **`_polygon_save(PhysicsInfo *info, ShapeInfo *shapeInfo, Store *s)`**
  - Saves vertex count using `cpPolyShapeGetNumVerts()`
  - Saves radius using `cpPolyShapeGetRadius()`
  - Saves all vertices in "verts" child store using `cpPolyShapeGetVert()`

• **`_polygon_load(PhysicsInfo *info, ShapeInfo *shapeInfo, Store *s)`**
  - Loads vertex count and radius
  - Allocates temporary vertex array
  - Loads all vertices from "verts" child store
  - Creates shape: `cpPolyShapeNew2(info->body, n, vs, cpvzero, r)`
  - Adds to space and frees temporary array

## Shape Property Serialization Macros

• **`shape_prop_save(type, f, n, prop)`** - Saves individual shape properties
• **`shape_prop_load(type, f, n, prop)`** - Loads individual shape properties
• **`shape_props_saveload(saveload)`** - Handles all common shape properties:
  - Sensor flag (`bool`)
  - Elasticity (`cpFloat`)
  - Friction (`cpFloat`)
  - Surface velocity (`cpVect`)
  - Collision type (`unsigned int`)
  - Group (`unsigned int`)
  - Layers (`unsigned int`)

## Complete Shape Array Serialization

• **`_shapes_save(PhysicsInfo *info, Store *s)`**
  - Iterates through `info->shapes` array
  - For each shape:
    - Saves shape type enum
    - Calls type-specific save function (`_circle_save` or `_polygon_save`)
    - Saves common properties using `shape_props_saveload(save)`

• **`_shapes_load(PhysicsInfo *info, Store *s)`**
  - Initializes `info->shapes` array
  - For each saved shape:
    - Loads shape type enum
    - Calls type-specific load function
    - Loads common properties using `shape_props_saveload(load)`
    - Sets user data to entity reference

## Top-Level System Functions

• **`physics_save_all(Store *s)`**
  - Creates "physics" child store
  - Uses `entitypool_save_foreach` to iterate through physics entities
  - For each entity saves: type, mass, body data, shapes data

• **`physics_load_all(Store *s)`**
  - Loads from "physics" child store
  - Uses `entitypool_load_foreach` to restore physics entities
  - For each entity:
    - Loads type, mass, body data, shapes data
    - Initializes collisions to NULL
    - Sets last position/angle for kinematic body tracking

## Key Dependencies

• **Chipmunk Physics Engine**: `cpBody*`, `cpShape*`, `cpSpace*` functions
• **Transform System**: `transform_get_position()`, `transform_get_rotation()`, `transform_get_dirty_count()`
• **Entity Pool System**: `entitypool_save_foreach`, `entitypool_load_foreach`
• **Store System**: `store_child_save()`, `store_child_load()`
• **Type System**: `PhysicsInfo`, `ShapeInfo`, `PhysicsBody`, shape types (`PS_CIRCLE`, `PS_POLYGON`)


### Related
- [Parent](../)

---
Hash: 4b37fcfbf5b2a960593d53789fffcb9547f1e40dde70ec265decf6898490a956
Generated: 2025-07-07T20:21:19-04:00
