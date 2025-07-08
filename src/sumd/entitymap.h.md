# Summary: entitymap.h

## Table of Contents

- [EntityMap Header - Entity-to-Integer Mapping Data Structure](#entitymap-header---entity-to-integer-mapping-data-structure)

This header file defines an interface for a hash map data structure that maps `Entity` objects to integer values.

## Data Structure Definition
- **EntityMap**: A typedef for `struct EntityMap` that represents the main hash map data structure
  - The actual struct definition is not shown (likely in the corresponding .c file)
  - Maps Entity keys to integer values
  - Supports a default value for unset/missing keys

## Dependencies
- **entity.h**: Required include that defines the `Entity` type used as keys in the map

## Function Interface
The header declares the following functions for EntityMap operations:

### Memory Management
- **entitymap_new(int def)**: Constructor function
  - Creates a new EntityMap instance
  - Takes a default integer value (`def`) that will be returned for unset keys
  - Returns a pointer to the new EntityMap
- **entitymap_free(EntityMap *emap)**: Destructor function
  - Frees all memory associated with the EntityMap

### Data Operations
- **entitymap_clear(EntityMap *emap)**: Clears all entries from the map
  - Removes all key-value pairs but keeps the map structure intact
- **entitymap_set(EntityMap *emap, Entity ent, int val)**: Sets a key-value pair
  - Maps the Entity `ent` to the integer value `val`
- **entitymap_get(EntityMap *emap, Entity ent)**: Retrieves a value by key
  - Returns the integer value associated with Entity `ent`
  - Returns the default value if the key is not found

## Design Notes
- The implementation uses opaque data structure pattern (struct definition hidden)
- Supports default values for missing keys, eliminating need for explicit "key not found" error handling
- All functions take EntityMap pointer as first parameter (object-oriented style in C)

## Sections

### EntityMap Header - Entity-to-Integer Mapping Data Structure
Source: [entitymap.h:1-17](../entitymap.h#L1-L17)

This header file defines the `EntityMap` data structure and its associated API for creating a mapping between `Entity` objects and integer values.

## Data Structure Definition
- **EntityMap**: An opaque struct typedef that represents a map from `Entity` to `int`
  - Implementation details are hidden (forward declaration only)
  - Supports a default value for unset keys

## Dependencies
- **entity.h**: Required include that defines the `Entity` type used as keys in the map

## API Functions
The header declares the following public interface functions:

- **entitymap_new(int def)**
  - Creates a new EntityMap instance
  - Takes a default value parameter that will be returned for unset keys
  - Returns pointer to newly allocated EntityMap

- **entitymap_clear(EntityMap *emap)**
  - Clears all entries from the map
  - Does not deallocate the map structure itself

- **entitymap_free(EntityMap *emap)**
  - Deallocates the EntityMap and frees associated memory

- **entitymap_set(EntityMap *emap, Entity ent, int val)**
  - Sets/updates the integer value for a given Entity key
  - Associates the Entity `ent` with integer value `val`

- **entitymap_get(EntityMap *emap, Entity ent)**
  - Retrieves the integer value associated with an Entity key
  - Returns the stored value if key exists, otherwise returns the default value set during creation

## Header Guard
- Uses standard include guard pattern with `ENTITYMAP_H` macro to prevent multiple inclusions


### Related
- [Parent](../)

---
Hash: 942567b17a099043f5a2e84e8d2ffbd508d81a471448f297c36595dc4d61d5dc
Generated: 2025-07-07T20:21:19-04:00
