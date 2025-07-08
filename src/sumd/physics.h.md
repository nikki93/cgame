# Summary: physics.h

## Table of Contents

- [Physics System Header Definition](#physics-system-header-definition)

This header file defines the complete interface for a 2D physics engine system, providing comprehensive physics simulation capabilities for game entities.

## Dependencies
- References external headers: `saveload.h`, `script_export.h`, `scalar.h`, `entity.h`, `vec2.h`, `bbox.h`
- Uses `stdbool.h` for boolean types
- All functions are wrapped in `SCRIPT(physics, ...)` macro for script export

## Global Physics Configuration
- **physics_set_gravity(Vec2 g)** / **physics_get_gravity()** - Controls world gravity vector
- **physics_set_simulation_frequency(Scalar freq)** / **physics_get_simulation_frequency()** - Controls physics update rate

## Physics Body Management
- **PhysicsBody enum** defines three body types:
  - `PB_STATIC` (0) - Immovable objects like walls
  - `PB_KINEMATIC` (1) - Moving objects not subject to dynamics (e.g., moving platforms)
  - `PB_DYNAMIC` (2) - Full physics simulation objects (e.g., bowling pins)
- **physics_add(Entity ent)** - Adds entity to physics system (defaults to PB_DYNAMIC)
- **physics_remove(Entity ent)** / **physics_has(Entity ent)** - Entity management
- **physics_set_type(Entity ent, PhysicsBody type)** / **physics_get_type(Entity ent)** - Body type control
- **physics_debug_draw(Entity ent)** - Debug visualization for one frame

## Shape System
- **PhysicsShape enum** defines shape types:
  - `PS_CIRCLE` (0) - Circular collision shapes
  - `PS_POLYGON` (1) - Polygonal collision shapes
- Shape creation functions (all return shape index):
  - **physics_shape_add_circle(Entity ent, Scalar r, Vec2 offset)** - Adds circle with radius and offset
  - **physics_shape_add_box(Entity ent, BBox b, Scalar r)** - Adds box with rounding radius
  - **physics_shape_add_poly(Entity ent, unsigned int nverts, const Vec2 *verts, Scalar r)** - Adds custom polygon
- Shape management:
  - **physics_get_num_shapes(Entity ent)** / **physics_shape_get_type(Entity ent, unsigned int i)** - Shape queries
  - **physics_shape_remove(Entity ent, unsigned int i)** - Shape removal
  - **physics_poly_get_num_verts(Entity ent, unsigned int i)** - Polygon vertex count (-1 if not polygon)
- **physics_convex_hull(unsigned int nverts, Vec2 *verts)** - In-place convex hull computation
- Shape properties:
  - **physics_shape_set_sensor(Entity ent, unsigned int i, bool sensor)** / **physics_shape_get_sensor(Entity ent, unsigned int i)** - Sensor flag (collision detection without physical response)
  - **physics_shape_set_surface_velocity(Entity ent, unsigned int i, Vec2 v)** / **physics_shape_get_surface_velocity(Entity ent, unsigned int i)** - Surface velocity for conveyor belt effects

## Dynamics System
- **Mass**: physics_set_mass() / physics_get_mass()
- **Rotation constraints**: physics_set_freeze_rotation() / physics_get_freeze_rotation()
- **Linear motion**:
  - Velocity: physics_set_velocity() / physics_get_velocity()
  - Force: physics_set_force() / physics_get_force()
  - Velocity limits: physics_set_velocity_limit() / physics_get_velocity_limit()
- **Angular motion**:
  - Angular velocity: physics_set_angular_velocity() / physics_get_angular_velocity()
  - Torque: physics_set_torque() / physics_get_torque()
  - Angular velocity limits: physics_set_angular_velocity_limit() / physics_get_angular_velocity_limit()
- **Force application**:
  - **physics_reset_forces(Entity ent)** - Clears all forces
  - **physics_apply_force(Entity ent, Vec2 force)** - Applies force at center of mass
  - **physics_apply_force_at(Entity ent, Vec2 force, Vec2 at)** - Applies force at specific point
  - **physics_apply_impulse(Entity ent, Vec2 impulse)** - Applies impulse at center of mass
  - **physics_apply_impulse_at(Entity ent, Vec2 impulse, Vec2 at)** - Applies impulse at specific point

## Collision Detection
- **Collision struct** contains:
  - `Entity a, b` - The two colliding entities
- **physics_get_num_collisions(Entity ent)** - Returns collision count for entity
- **physics_get_collisions(Entity ent)** - Returns array of Collision structs

## Spatial Queries
- **NearestResult struct** contains:
  - `Entity ent` - Closest entity (entity_nil if none in range)
  - `Vec2 p` - Closest point on shape surface
  - `Scalar d` - Distance to point (negative if inside shape)
  - `Vec2 g` - Gradient of distance function
- **physics_nearest(Vec2 point, Scalar max_dist)** - Finds nearest entity within max distance

## System Management Functions
- **physics_init()** / **physics_deinit()** - System initialization/cleanup
- **physics_update_all()** / **physics_post_update_all()** - Simulation step functions
- **physics_draw_all()** - Debug rendering for all physics objects
- **physics_save_all(Store *s)** / **physics_load_all(Store *s)** - Serialization support

## Sections

### Physics System Header Definition
Source: [physics.h:1-158](../physics.h#L1-L158)

This header file defines a comprehensive physics system API for a game engine or simulation framework. The system provides 2D physics functionality with support for rigid bodies, collision detection, and dynamics simulation.

## Core Dependencies
- References external types: `Entity`, `Vec2`, `Scalar`, `BBox`
- Uses `Store` type for save/load functionality
- Includes standard `stdbool.h` and custom headers: `saveload.h`, `script_export.h`, `scalar.h`, `entity.h`, `vec2.h`, `bbox.h`

## Global Physics Configuration
- **physics_set_gravity(Vec2 g)** / **physics_get_gravity()**: Control global gravity vector
- **physics_set_simulation_frequency(Scalar freq)** / **physics_get_simulation_frequency()**: Control physics update rate

## Physics Body Management
- **PhysicsBody enum**: Defines three body types:
  - `PB_STATIC` (0): Immovable objects like walls
  - `PB_KINEMATIC` (1): Movable but not physics-driven (e.g., moving platforms)
  - `PB_DYNAMIC` (2): Full physics simulation (e.g., bowling pins)
- **physics_add(Entity ent)**: Adds entity to physics system (defaults to PB_DYNAMIC)
- **physics_remove(Entity ent)** / **physics_has(Entity ent)**: Remove/check physics bodies
- **physics_set_type(Entity ent, PhysicsBody type)** / **physics_get_type(Entity ent)**: Manage body types
- **physics_debug_draw(Entity ent)**: Debug visualization for one frame

## Shape System
- **PhysicsShape enum**: Two shape types:
  - `PS_CIRCLE` (0): Circular collision shapes
  - `PS_POLYGON` (1): Polygonal collision shapes
- **Shape Creation Functions**:
  - **physics_shape_add_circle(Entity ent, Scalar r, Vec2 offset)**: Add circular shape with radius and offset
  - **physics_shape_add_box(Entity ent, BBox b, Scalar r)**: Add box shape with rounding radius
  - **physics_shape_add_poly(Entity ent, unsigned int nverts, const Vec2 *verts, Scalar r)**: Add polygon from vertices
- **Shape Management**:
  - **physics_get_num_shapes(Entity ent)**: Get shape count for entity
  - **physics_shape_get_type(Entity ent, unsigned int i)**: Get shape type by index
  - **physics_shape_remove(Entity ent, unsigned int i)**: Remove shape by index
  - **physics_poly_get_num_verts(Entity ent, unsigned int i)**: Get vertex count for polygon shapes
- **Convex Hull Utility**:
  - **physics_convex_hull(unsigned int nverts, Vec2 *verts)**: Modifies vertex array in-place to create convex hull
- **Shape Properties**:
  - **physics_shape_set_sensor(Entity ent, unsigned int i, bool sensor)** / **physics_shape_get_sensor()**: Configure collision detection vs. trigger behavior
  - **physics_shape_set_surface_velocity(Entity ent, unsigned int i, Vec2 v)** / **physics_shape_get_surface_velocity()**: Set surface movement for conveyor belt effects

## Dynamics System
- **Mass Properties**:
  - **physics_set_mass(Entity ent, Scalar mass)** / **physics_get_mass()**: Control object mass
  - **physics_set_freeze_rotation(Entity ent, bool freeze)** / **physics_get_freeze_rotation()**: Lock rotational movement
- **Linear Motion**:
  - **physics_set_velocity(Entity ent, Vec2 vel)** / **physics_get_velocity()**: Direct velocity control
  - **physics_set_force(Entity ent, Vec2 force)** / **physics_get_force()**: Apply continuous forces
  - **physics_set_velocity_limit(Entity ent, Scalar lim)** / **physics_get_velocity_limit()**: Limit maximum velocity
- **Angular Motion**:
  - **physics_set_angular_velocity(Entity ent, Scalar ang_vel)** / **physics_get_angular_velocity()**: Control rotational velocity
  - **physics_set_torque(Entity ent, Scalar torque)** / **physics_get_torque()**: Apply rotational forces
  - **physics_set_angular_velocity_limit(Entity ent, Scalar lim)** / **physics_get_angular_velocity_limit()**: Limit rotational speed
- **Force Application**:
  - **physics_reset_forces(Entity ent)**: Clear all applied forces
  - **physics_apply_force(Entity ent, Vec2 force)**: Apply force at center of mass
  - **physics_apply_force_at(Entity ent, Vec2 force, Vec2 at)**: Apply force at specific point
  - **physics_apply_impulse(Entity ent, Vec2 impulse)**: Apply instantaneous velocity change
  - **physics_apply_impulse_at(Entity ent, Vec2 impulse, Vec2 at)**: Apply impulse at specific point

## Collision System
- **Collision struct**: Contains entities `a` and `b` involved in collision
- **physics_get_num_collisions(Entity ent)**: Get collision count for entity
- **physics_get_collisions(Entity ent)**: Get array of collisions for entity

## Spatial Queries
- **NearestResult struct**: Contains query results:
  - `ent`: Closest entity (or entity_nil if none found)
  - `p`: Closest point on shape surface
  - `d`: Distance to point (negative if inside shape)
  - `g`: Gradient vector of distance function
- **physics_nearest(Vec2 point, Scalar max_dist)**: Find nearest entity within maximum distance

## System Functions
- **physics_init()** / **physics_deinit()**: Initialize/cleanup physics system
- **physics_update_all()**: Step physics simulation
- **physics_post_update_all()**: Post-simulation cleanup
- **physics_draw_all()**: Render all physics debug information
- **physics_save_all(Store *s)** / **physics_load_all(Store *s)**: Serialize/deserialize physics state


### Related
- [Parent](../)

---
Hash: 6ef353f5a414f31a77997a766edbbbc7d573adc1c250b1ad2a69d42e32949682
Generated: 2025-07-07T20:21:19-04:00
