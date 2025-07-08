# Summary: entitypool.c

## Table of Contents

- [EntityPool Implementation - Dynamic Pool for Entity-Associated Objects](#entitypool-implementation---dynamic-pool-for-entity-associated-objects)

This file implements an `EntityPool` data structure that provides efficient storage and retrieval of objects indexed by Entity IDs. The pool combines an EntityMap for fast lookups with an Array for contiguous storage.

## Core Data Structure

• **EntityPool struct** contains:
  - `emap`: EntityMap pointer that maps Entity IDs to array indices (-1 if doesn't exist)
  - `array`: Array pointer for contiguous object storage

## Memory Management Functions

• **entitypool_new_()**: Creates new EntityPool with specified object size
  - Allocates EntityPool struct
  - Initializes EntityMap with -1 as default value
  - Creates new Array with given object size

• **entitypool_free()**: Deallocates EntityPool and its components
  - Frees the internal array
  - Frees the entity map
  - Frees the pool struct itself

## Core Operations

• **entitypool_add()**: Adds object for given Entity
  - First checks if entity already exists (returns existing if found)
  - Adds new element to array and stores Entity ID in the element
  - Updates entity map to point to new array index (length - 1)
  - Returns pointer to the new element

• **entitypool_remove()**: Removes object associated with Entity
  - Looks up array index from entity map
  - Uses `array_quick_remove()` which may swap with last element
  - If swap occurred, updates the swapped element's mapping
  - Sets entity mapping to -1 to mark as removed

• **entitypool_get()**: Retrieves object for given Entity
  - Looks up array index from entity map
  - Returns array element if found, NULL otherwise

## Iteration and Access Functions

• **entitypool_begin()**: Returns pointer to first array element
• **entitypool_end()**: Returns pointer past last array element  
• **entitypool_nth()**: Returns nth element in array
• **entitypool_size()**: Returns number of elements in pool

## Utility Operations

• **entitypool_clear()**: Empties the pool
  - Clears both entity map and array

• **entitypool_sort()**: Sorts pool elements using comparison function
  - Sorts the underlying array
  - Rebuilds all Entity-to-index mappings since array order changed
  - Iterates through sorted array to update EntityMap

## Serialization Support

• **entitypool_elem_save()**: Saves EntityPoolElem to Store
  - Extracts Entity ID from element and saves it with "pool_elem" key

• **entitypool_elem_load()**: Loads EntityPoolElem from Store
  - Loads Entity ID from "pool_elem" key
  - Adds element to pool with that Entity ID
  - Updates the loaded element pointer

## Key Dependencies

• **EntityMap**: For Entity ID to array index mapping
• **Array**: For contiguous object storage
• **Entity**: Entity ID type and operations
• **Store**: For serialization operations
• **EntityPoolElem**: Element type that contains Entity ID field

## Important Implementation Details

• Uses -1 as sentinel value for non-existent mappings
• Remove operation uses quick removal (swap with last) for O(1) performance
• Sort operation requires full remapping since array indices change
• Elements store their Entity ID internally for reverse lookup during sorting

## Sections

### EntityPool Implementation - Dynamic Pool for Entity-Associated Objects
Source: [entitypool.c:1-134](../entitypool.c#L1-L134)

This section implements a complete EntityPool data structure that manages a collection of objects associated with Entity identifiers, providing efficient storage, retrieval, and iteration capabilities.

## Data Structure Definition

• **EntityPool struct**: Contains two main components:
  - `EntityMap *emap`: Maps Entity IDs to array indices (-1 indicates non-existence)
  - `Array *array`: Stores the actual EntityPoolElem objects in contiguous memory

## Core Functions

• **Memory Management**:
  - `entitypool_new_(size_t object_size)`: Creates new pool with specified object size
  - `entitypool_free(EntityPool *pool)`: Deallocates pool and its components

• **Element Operations**:
  - `entitypool_add(EntityPool *pool, Entity ent)`: 
    - Returns existing element if Entity already exists
    - Otherwise creates new EntityPoolElem, adds to array, and maps Entity to array index
    - Returns pointer to the element
  - `entitypool_remove(EntityPool *pool, Entity ent)`:
    - Finds element by Entity ID using entitymap
    - Uses `array_quick_remove()` which may swap with last element
    - Updates mapping for swapped element if necessary
    - Removes Entity mapping by setting to -1
  - `entitypool_get(EntityPool *pool, Entity ent)`: Returns element pointer or NULL if not found

• **Iterator/Access Functions**:
  - `entitypool_begin(EntityPool *pool)`: Returns pointer to first element
  - `entitypool_end(EntityPool *pool)`: Returns pointer past last element
  - `entitypool_nth(EntityPool *pool, unsigned int n)`: Returns nth element
  - `entitypool_size(EntityPool *pool)`: Returns number of elements

• **Utility Operations**:
  - `entitypool_clear(EntityPool *pool)`: Removes all elements from both map and array
  - `entitypool_sort(EntityPool *pool, int (*compar)(const void *, const void *))`:
    - Sorts the underlying array using provided comparison function
    - Rebuilds the Entity-to-index mapping after sorting to maintain consistency

## Serialization Support

• **Save/Load Functions**:
  - `entitypool_elem_save(EntityPool *pool, void *elem, Store *s)`: Saves Entity ID of element
  - `entitypool_elem_load(EntityPool *pool, void *elem, Store *s)`: 
    - Loads Entity ID from store
    - Adds element to pool with that Entity ID
    - Updates the loaded element pointer

## Key Dependencies

• **External Components**:
  - `EntityMap`: For Entity ID to array index mapping
  - `Array`: For dynamic array storage
  - `Entity`: Entity identifier type and related functions
  - `Store`: For serialization operations
  - `error.h`: For error handling (error_assert)

## Algorithm Details

• **Add Operation Logic**:
  1. Check if Entity already exists in pool
  2. If exists, return existing element
  3. If not, add new element to array
  4. Set Entity field in new element
  5. Map Entity ID to new array index

• **Remove Operation Logic**:
  1. Look up array index for Entity
  2. Remove from array (potentially swapping with last element)
  3. If swap occurred, update mapping for the moved element
  4. Remove Entity mapping by setting to -1

• **Sort Operation Logic**:
  1. Sort the underlying array using provided comparator
  2. Iterate through sorted array to rebuild Entity-to-index mappings
  3. Ensures mapping consistency after reordering


### Related
- [Parent](../)

---
Hash: 0910768d852374d6184fcd8095313dc1ba8c80f0c9d72ed90c16b9b56fb445a0
Generated: 2025-07-07T20:21:19-04:00
