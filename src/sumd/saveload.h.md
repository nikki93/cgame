# Summary: saveload.h

## Table of Contents

- [Save/Load System Header - Store Management and Data Serialization](#saveload-system-header---store-management-and-data-serialization)

This header file defines a comprehensive save/load system for persisting various data types to and from storage. The system is built around a central `Store` data structure and provides both script-exportable and internal C functions.

## Core Data Structure
- **`Store`**: Opaque struct representing a storage container for save/load operations
  - Forward declared as `typedef struct Store Store`
  - Users must call `*_close()` functions to free resources when done

## Script-Exported Store Operations
The following functions are exported to scripts via the `SCRIPT(saveload, ...)` macro:

### Store Creation and Opening
- **`store_open()`**: Creates an empty store
- **`store_open_str(const char *str)`**: Opens store from string data
- **`store_open_file(const char *filename)`**: Opens store from file

### Store Writing and Cleanup
- **`store_write_str(Store *s)`**: Serializes store to string
- **`store_write_file(Store *s, const char *filename)`**: Writes store to file
- **`store_close(Store *s)`**: Closes store and frees resources

## Hierarchical Store Management
Support for nested store structures for backwards-compatible save/load:
- **`store_child_save(Store **sp, const char *name, Store *parent)`**: Creates child store
- **`store_child_save_compressed(Store **sp, const char *name, Store *parent)`**: Creates compressed child store
- **`store_child_load(Store **sp, const char *name, Store *parent)`**: Loads child store

## Type-Specific Save/Load Functions
All save/load functions follow a consistent pattern:
- **Save functions**: Take pointer to value, name string, and store
- **Load functions**: Take pointer to destination, name string, default value, and store
- **Load return value**: `bool` indicating whether value was found (true) or defaulted (false)

### Supported Data Types
- **`Scalar`**: Custom scalar type (depends on "scalar.h")
  - `scalar_save(const Scalar *f, const char *n, Store *s)`
  - `scalar_load(Scalar *f, const char *n, Scalar d, Store *s)`

- **`unsigned int`**: Unsigned integers
  - `uint_save(const unsigned int *u, const char *n, Store *s)`
  - `uint_load(unsigned int *u, const char *n, unsigned int d, Store *s)`

- **`int`**: Signed integers
  - `int_save(const int *i, const char *n, Store *s)`
  - `int_load(int *i, const char *n, int d, Store *s)`

- **`bool`**: Boolean values
  - `bool_save(const bool *b, const char *n, Store *s)`
  - `bool_load(bool *b, const char *n, bool d, Store *s)`

- **`char *`**: String data
  - `string_save(const char **c, const char *n, Store *s)`
  - `string_load(char **c, const char *n, const char *d, Store *s)`
  - Note: Caller must `free(*c)` after string_load

### Enum Support via Macros
- **`enum_save(val, n, s)`**: Macro that casts enum to int and saves
- **`enum_load(val, n, d, s)`**: Macro that loads as int and casts back to enum

## Dependencies
- **`<stdbool.h>`**: For boolean type support
- **`"scalar.h"`**: For custom Scalar type definition
- **`"script_export.h"`**: For SCRIPT macro and export functionality

## Key Design Patterns
- Consistent naming convention with type prefix
- Uniform parameter ordering: value, name, store
- Memory management responsibility clearly documented
- Support for default values in load operations
- Backwards compatibility through hierarchical store trees

## Sections

### Save/Load System Header - Store Management and Data Serialization
Source: [saveload.h:1-56](../saveload.h#L1-L56)

This header file defines a comprehensive save/load system for data serialization with support for various data types and hierarchical storage structures.

## Core Components

### Store Structure and Management
- **`Store`**: Opaque structure type for managing serialized data storage
- **Store Creation Functions**:
  - `store_open()`: Creates empty store
  - `store_open_str(const char *str)`: Creates store from string data
  - `store_open_file(const char *filename)`: Creates store from file
- **Store Output Functions**:
  - `store_write_str(Store *s)`: Converts store to string representation
  - `store_write_file(Store *s, const char *filename)`: Writes store to file
- **Resource Management**:
  - `store_close(Store *s)`: Frees store resources (must be called when done)

### Hierarchical Storage Support
- **`store_child_save(Store **sp, const char *name, Store *parent)`**: Creates child store for nested data structures
- **`store_child_save_compressed(Store **sp, const char *name, Store *parent)`**: Creates compressed child store
- **`store_child_load(Store **sp, const char *name, Store *parent)`**: Loads child store from parent

## Data Type Serialization Functions

### Function Patterns
- **Save functions**: Take format `type_save(const type *value, const char *name, Store *store)`
- **Load functions**: Take format `type_load(type *value, const char *name, type default, Store *store)`
  - Return `bool` indicating whether value was found (true) or defaulted (false)

### Supported Data Types
- **`Scalar`**: Custom scalar type (depends on "scalar.h")
  - `scalar_save(const Scalar *f, const char *n, Store *s)`
  - `scalar_load(Scalar *f, const char *n, Scalar d, Store *s)`
- **Unsigned Integer**:
  - `uint_save(const unsigned int *u, const char *n, Store *s)`
  - `uint_load(unsigned int *u, const char *n, unsigned int d, Store *s)`
- **Signed Integer**:
  - `int_save(const int *i, const char *n, Store *s)`
  - `int_load(int *i, const char *n, int d, Store *s)`
- **Boolean**:
  - `bool_save(const bool *b, const char *n, Store *s)`
  - `bool_load(bool *b, const char *n, bool d, Store *s)`
- **String**:
  - `string_save(const char **c, const char *n, Store *s)`
  - `string_load(char **c, const char *n, const char *d, Store *s)`
  - Note: Caller must `free(*c)` after loading

### Enum Support via Macros
- **`enum_save(val, n, s)`**: Macro that casts enum to int and saves
- **`enum_load(val, n, d, s)`**: Macro that loads as int and casts back to enum

## Dependencies and Integration
- **Headers**: Depends on `<stdbool.h>`, `"scalar.h"`, and `"script_export.h"`
- **Script Export**: Uses `SCRIPT(saveload, ...)` and `EXPORT` macros for external interface exposure
- **Backwards Compatibility**: Store trees specifically designed to support backwards-compatible save/load operations

## Key Design Features
- Consistent naming convention for all save/load function pairs
- Default value support for all load operations
- Resource management with explicit cleanup requirement
- Hierarchical data organization through child stores
- Compression support for space optimization
- Memory management responsibility clearly documented (especially for strings)


### Related
- [Parent](../)

---
Hash: 1a6919b6f8edcb980c1c754b73482a9b2c078a383060e6a16003b4825d1817eb
Generated: 2025-07-07T20:21:19-04:00
