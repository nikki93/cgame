# Summary: saveload.c

## Table of Contents

- [Save/Load System Core Implementation - Streams and Store Tree Structure](#saveload-system-core-implementation---streams-and-store-tree-structure)
- [Store Open/Close Operations and Primitive Data Type Serialization](#store-openclose-operations-and-primitive-data-type-serialization)

This file implements a comprehensive save/load system for serializing and deserializing hierarchical data structures to/from files and strings. The system uses a tree-like structure with streams for flexible data storage.

## Core Data Structures

**Stream Structure**
- `Stream`: Growable string buffer with position tracking
  - `buf`: Character buffer for data storage
  - `pos`: Current read/write position
  - `cap`: Allocated buffer capacity

**Store Structure**
- `Store`: Hierarchical data container representing tree nodes
  - `name`: Node identifier string
  - `sm`: Embedded Stream for data storage
  - `compressed`: Boolean flag for compression format
  - `child`, `parent`, `sibling`: Tree navigation pointers
  - `iterchild`: Iterator for unnamed child traversal
  - `str`: Cached string result from `store_get_str()`

## Stream Operations

**Internal Stream Functions**
- `_stream_init()`: Initialize empty stream
- `_stream_deinit()`: Free stream resources
- `_stream_grow()`: Expand buffer capacity as needed
- `_stream_printf()`: Formatted writing with automatic growth
- `_stream_scanf_()` + `_stream_scanf()` macro: Formatted reading with position tracking

**String Serialization**
- `_stream_write_string()`: Serializes strings with quote escaping
  - NULL strings written as "n "
  - Non-NULL strings written as quoted with escaped quotes
- `_stream_read_string_()` + `_stream_read_string()` macro: Deserializes strings with unescaping

## Store Management

**Store Lifecycle**
- `_store_new()`: Create new Store node with parent linking
- `_store_free()`: Recursively free Store tree and resources

**Serialization Formats**
- `_store_write()`: Compact format using `{ }` for normal, `[ ]` for compressed
- `_store_write_pretty()`: Human-readable format with indentation
- `_store_read()`: Parse serialized Store from stream with format detection

## Public API Functions

**Store Creation/Destruction**
- `store_open()`: Create empty root Store
- `store_open_str()`: Parse Store from string
- `store_open_file()`: Load Store from file (format: `<length>\n<data>`)
- `store_write_str()`: Serialize Store to string
- `store_write_file()`: Save Store to file
- `store_close()`: Free entire Store tree

**Child Store Management**
- `store_child_save()`: Create child Store for saving data
- `store_child_save_compressed()`: Create compressed child Store
- `store_child_load()`: Find child Store by name or iterate unnamed children

**Data Type Serialization**
- `scalar_save()`/`scalar_load()`: Save/load Scalar values (handles infinity as 'i')
- `uint_save()`/`uint_load()`: Save/load unsigned integers
- `int_save()`/`int_load()`: Save/load signed integers
- `bool_save()`/`bool_load()`: Save/load boolean values
- `string_save()`/`string_load()`: Save/load strings with memory management

## Key Features

**Hierarchical Structure**
- Tree-based organization with named nodes
- Parent-child relationships maintained via pointers
- Sibling traversal for iteration

**Compression Support**
- Compressed stores flatten hierarchy for space efficiency
- Different serialization brackets distinguish formats

**Error Handling**
- Uses `error()` and `error_assert()` functions for validation
- Detects corrupt save data during parsing

**Memory Management**
- Automatic buffer growth for streams
- Proper cleanup of allocated strings and structures
- String result caching in Store nodes

The system provides a flexible framework for saving/loading game state or configuration data with support for nested structures, multiple data types, and both compact and human-readable formats.

## Sections

### Save/Load System Core Implementation - Streams and Store Tree Structure
Source: [saveload.c:1-352](../saveload.c#L1-L352)

This section implements the core data structures and low-level functions for a hierarchical save/load system. The implementation consists of two main components: growable string streams for data serialization and a tree-based store structure for organizing saved data.

## Core Data Structures

• **`Stream`** - A growable string buffer structure containing:
  - `buf`: character buffer pointer
  - `pos`: current read/write position
  - `cap`: allocated buffer capacity

• **`Store`** - A tree node structure for hierarchical data storage containing:
  - `name`: string identifier for the store
  - `sm`: embedded Stream for data content
  - `compressed`: boolean flag for compression mode
  - `child`, `parent`, `sibling`: tree navigation pointers
  - `iterchild`: pointer for iterating through children when name is NULL
  - `str`: result buffer for `store_get_str()` function

## Stream Operations

• **Initialization and Cleanup**:
  - `_stream_init()`: initializes stream with NULL buffer and zero position/capacity
  - `_stream_deinit()`: frees the buffer memory

• **Memory Management**:
  - `_stream_grow()`: expands buffer capacity using doubling strategy when position exceeds current capacity

• **Formatted I/O**:
  - `_stream_printf()`: writes formatted data to stream, automatically growing buffer as needed
  - `_stream_scanf_()` and `_stream_scanf()` macro: reads formatted data from stream and advances position

• **String Serialization**:
  - `_stream_write_string()`: serializes strings with quote escaping, writes NULL strings as "n "
  - `_stream_read_string_()` and `_stream_read_string()`: deserializes strings with quote unescaping, handles NULL strings

## Store Tree Operations

• **Store Management**:
  - `_store_new()`: creates new store node and links it into parent's child list
  - `_store_free()`: recursively frees store tree and all associated memory

• **Serialization**:
  - `_store_write()`: writes store tree in compact format using `{ }` for normal stores, `[ ]` for compressed
  - `_store_write_pretty()`: writes store tree with indentation for readability (compressed stores remain compact)
  - `_store_read()`: parses serialized store data back into tree structure, handles both normal and compressed formats

• **Child Store Access**:
  - `store_child_save()`: creates child store for saving data, returns parent directly if compressed
  - `store_child_save_compressed()`: creates compressed child store
  - `store_child_load()`: locates child store for loading by name or iteration, handles compressed mode flattening

## Key Algorithms

• **Buffer Growing Strategy**: Doubles capacity until target position fits, starting from minimum capacity of 2

• **String Escaping**: Escapes quote characters with backslashes during serialization, unescapes during deserialization

• **Tree Traversal**: Uses sibling pointers for breadth-first child enumeration, `iterchild` pointer for sequential access

• **Compression Handling**: Compressed stores flatten their hierarchy - child operations return the parent store directly rather than creating actual child nodes

## Dependencies

• **External Headers**: `stdlib.h`, `string.h`, `stdarg.h`, `stdio.h`, `ctype.h`
• **Internal Headers**: `saveload.h`, `error.h`
• **Error Handling**: Uses `error()` and `error_assert()` functions for corruption detection and validation

The implementation provides a foundation for hierarchical data serialization with support for both human-readable and compressed formats, automatic memory management, and tree-based organization of saved data.

### Store Open/Close Operations and Primitive Data Type Serialization
Source: [saveload.c:356-603](../saveload.c#L356-L603)

This section implements the core store management functions for opening, closing, reading, and writing stores, along with serialization functions for primitive data types.

## Store Management Functions

• **store_open()** - Creates a new empty store by calling `_store_new(NULL)`

• **store_open_str(const char *str)** - Creates a store from a string representation
  - Creates a `Stream` object from the input string
  - Calls `_store_read()` to parse the string into a store structure

• **store_write_str(Store *s)** - Converts a store to its string representation
  - Initializes a `Stream` object with `_stream_init()`
  - Calls `_store_write_pretty()` to serialize the store with formatting
  - Replaces the store's internal string buffer with the serialized output
  - Returns the string representation without deallocating the stream buffer

• **store_open_file(const char *filename)** - Loads a store from a file
  - Opens file for reading with error checking via `error_assert()`
  - Reads file format: `<length>\n<string_data>`
  - Uses `fscanf()` to read the length, then `fread()` to read the data
  - Creates store from the string data using `store_open_str()`
  - Properly manages memory by freeing the temporary string buffer

• **store_write_file(Store *s, const char *filename)** - Saves a store to a file
  - Opens file for writing with error checking
  - Converts store to string using `store_write_str()`
  - Writes in format: `<length>\n<string_data>` using `fprintf()` and `fwrite()`

• **store_close(Store *s)** - Deallocates a store by calling `_store_free()`

## Internal Macros

• **_store_printf(s, fmt, ...)** - Wrapper for `_stream_printf()` for formatted output to store's stream
• **_store_scanf(s, fmt, ...)** - Wrapper for `_stream_scanf()` for formatted input from store's stream

## Primitive Data Type Serialization

Each primitive type has a pair of save/load functions following a consistent pattern:

### Scalar (Floating Point) Functions
• **scalar_save(const Scalar *f, const char *n, Store *s)**
  - Uses `store_child_save()` to create a child store
  - Saves `SCALAR_INFINITY` as "i", regular values with "%f" format

• **scalar_load(Scalar *f, const char *n, Scalar d, Store *s)**
  - Uses `store_child_load()` to access child store
  - Handles infinity by checking for 'i' character
  - Returns `true` if value was loaded, `false` if default value `d` was used

### Unsigned Integer Functions
• **uint_save(const unsigned int *u, const char *n, Store *s)** - Saves with "%u" format
• **uint_load(unsigned int *u, const char *n, unsigned int d, Store *s)** - Loads with "%u" format

### Integer Functions
• **int_save(const int *i, const char *n, Store *s)** - Saves with "%d" format
• **int_load(int *i, const char *n, int d, Store *s)** - Loads with "%d" format

### Boolean Functions
• **bool_save(const bool *b, const char *n, Store *s)** - Saves as integer with "%d" format
• **bool_load(bool *b, const char *n, bool d, Store *s)** - Loads as integer, converts to bool

### String Functions
• **string_save(const char **c, const char *n, Store *s)** - Uses `_stream_write_string()`
• **string_load(char **c, const char *n, const char *d, Store *s)**
  - Uses `_stream_read_string()` to allocate and read string
  - If loading fails, allocates memory for default string `d` or sets to NULL

## Dependencies

The code relies on several internal functions not defined in this section:
- `_store_new()`, `_store_free()`, `_store_read()`, `_store_write_pretty()`
- `_stream_init()`, `_stream_printf()`, `_stream_scanf()`, `_stream_write_string()`, `_stream_read_string()`
- `store_child_save()`, `store_child_load()`
- `error_assert()` for error handling

## Test Code

The section includes commented test code demonstrating:
- Creating hierarchical stores with nested children
- Saving various data types including strings, scalars, and infinity values
- Writing to and reading from files
- Iterating through child stores with `store_child_load()`


### Related
- [Parent](../)

---
Hash: 1c6c2827c0f7b3e0579aebf7e538e595b9e64669c566b4815441380a00243f62
Generated: 2025-07-07T20:21:19-04:00
