# Summary: entitymap.c

## Table of Contents

- [EntityMap Implementation - Dynamic Array-Based Entity-Value Mapping](#entitymap-implementation---dynamic-array-based-entity-value-mapping)

This section implements a dynamic array data structure called `EntityMap` that maps entity IDs to integer values with automatic resizing capabilities.

## Data Structure Definition

• **EntityMap struct** contains:
  - `int *arr`: Dynamic array storing the actual values
  - `unsigned int bound`: One plus the maximum key currently in use
  - `unsigned int capacity`: Total allocated space in the array
  - `int def`: Default value returned for unset keys
  - Maintains invariants: `bound <= capacity` and `MIN_CAPACITY <= capacity`

• **MIN_CAPACITY** constant set to 2 (minimum array size)

## Core Functions

• **entitymap_new(int def)**:
  - Allocates new EntityMap with specified default value
  - Calls internal `_init()` to set up initial state
  - Returns pointer to new EntityMap

• **entitymap_clear(EntityMap *emap)**:
  - Frees current array and reinitializes to empty state
  - Preserves the default value

• **entitymap_free(EntityMap *emap)**:
  - Deallocates both the array and the EntityMap struct itself

## Memory Management Functions

• **_init(EntityMap *emap)** (internal):
  - Sets bound to 0, capacity to MIN_CAPACITY
  - Allocates initial array and fills with default values

• **_grow(EntityMap *emap)** (internal):
  - Finds next power of 2 that accommodates the current bound
  - Reallocates array to new capacity
  - Initializes new elements with default value

• **_shrink(EntityMap *emap)** (internal):
  - Shrinks capacity when bound is less than 1/4 of current capacity
  - Halves capacity while maintaining minimum size constraint
  - Only shrinks if capacity exceeds MIN_CAPACITY

## Access Functions

• **entitymap_set(EntityMap *emap, Entity ent, int val)**:
  - If setting to default value (deletion):
    - Updates array at entity ID
    - Potentially shrinks bound by scanning backwards for non-default values
    - Calls `_shrink()` if bound changed
  - If setting to non-default value:
    - Expands bound if necessary
    - Calls `_grow()` if entity ID exceeds current capacity
    - Updates array at entity ID

• **entitymap_get(EntityMap *emap, Entity ent)**:
  - Returns default value if entity ID is beyond current capacity
  - Otherwise returns value from array at entity ID

## Dependencies

• References **Entity** type (defined elsewhere, uses `.id` field)
• Uses standard library functions: `malloc`, `realloc`, `free`
• Includes "entitymap.h" header file

## Key Algorithms

• **Dynamic resizing**: Grows by powers of 2 when needed, shrinks when utilization drops below 25%
• **Bound management**: Tracks the highest used index plus one to optimize memory usage
• **Lazy deletion**: Setting values to default is treated as deletion, triggering potential shrinking

## Sections

### EntityMap Implementation - Dynamic Array-Based Entity-Value Mapping
Source: [entitymap.c:1-122](../entitymap.c#L1-L122)

This section implements a dynamic array-based data structure for mapping Entity objects to integer values, with automatic memory management and efficient storage optimization.

## Structure Definition

**EntityMap** struct contains:
- `int *arr`: Dynamic array storing the actual values
- `unsigned int bound`: One plus the maximum key (highest entity ID + 1)
- `unsigned int capacity`: Current heap-allocated array size
- `int def`: Default value returned for unset/deleted keys
- Invariants: `bound <= capacity` and `MIN_CAPACITY <= capacity` (where `MIN_CAPACITY = 2`)

## Core Functions

### Memory Management Functions
- **entitymap_new(int def)**: 
  - Creates new EntityMap with specified default value
  - Allocates memory for struct and calls `_init()` helper
  - Returns pointer to new EntityMap

- **entitymap_clear(EntityMap *emap)**:
  - Frees current array and reinitializes to minimum capacity
  - Resets bound to 0 while preserving default value

- **entitymap_free(EntityMap *emap)**:
  - Deallocates both array and struct memory
  - Complete cleanup function

### Data Access Functions
- **entitymap_set(EntityMap *emap, Entity ent, int val)**:
  - Sets value for given entity ID
  - **Deletion logic** (when `val == emap->def`):
    - Sets array value to default
    - If entity was at bound edge, shrinks bound by scanning backwards
    - Calls `_shrink()` to potentially reduce capacity
  - **Insertion logic** (when `val != emap->def`):
    - Expands bound if entity ID exceeds current bound
    - Calls `_grow()` if capacity needs expansion
    - Sets array value

- **entitymap_get(EntityMap *emap, Entity ent)**:
  - Returns value for entity ID, or default if ID exceeds capacity
  - Simple bounds-checked array access

## Internal Helper Functions

### Initialization
- **_init(EntityMap *emap)**:
  - Sets bound to 0, capacity to MIN_CAPACITY
  - Allocates initial array and fills with default values

### Dynamic Resizing
- **_grow(EntityMap *emap)**:
  - Finds next power of 2 that accommodates current bound
  - Reallocates array to new capacity
  - Initializes new elements with default value

- **_shrink(EntityMap *emap)**:
  - Triggered when bound is less than 1/4 of capacity
  - Halves capacity while maintaining minimum size constraint
  - Only shrinks if current capacity exceeds MIN_CAPACITY

## Key Dependencies
- References **Entity** type (defined elsewhere, accessed via `ent.id`)
- Uses standard library functions: `malloc`, `realloc`, `free`

## Algorithm Details
- **Growth strategy**: Powers of 2 expansion for amortized O(1) insertions
- **Shrinkage strategy**: Halves capacity when utilization drops below 25%
- **Bound management**: Tracks highest valid entity ID to optimize memory usage and iteration bounds


### Related
- [Parent](../)

---
Hash: 944820e25582a80231ff11326f3c1f9d0d367b951d262a38408335f31f02d012
Generated: 2025-07-07T20:21:19-04:00
