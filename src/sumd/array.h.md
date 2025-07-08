# Summary: array.h

## Table of Contents

- [Dynamic Array Data Structure Header Definition](#dynamic-array-data-structure-header-definition)

This header file defines a generic dynamic array data structure that stores objects contiguously in memory with automatic resizing capabilities.

## Core Data Structure
- **`Array`**: Opaque struct type representing the dynamic array container
- **Important constraint**: Objects may move in memory during operations, so pointers to array elements should not be retained

## Memory Management Functions
- **`array_new_(size_t object_size)`**: Constructor function that takes element size in bytes
- **`array_new(type)`**: Macro wrapper that automatically calculates `sizeof(type)` for convenience
- **`array_free(Array *arr)`**: Destructor to deallocate array memory

## Element Access Functions
- **`array_get(Array *arr, unsigned int i)`**: Returns pointer to element at index `i`
- **`array_get_val(type, arr, i)`**: Macro that dereferences the pointer to return the actual value
- **`array_top(Array *arr)`**: Returns pointer to the last element
- **`array_top_val(type, arr)`**: Macro that dereferences to return the last element's value
- **`array_length(Array *arr)`**: Returns the number of elements currently stored

## Iterator Support
- **`array_begin(Array *arr)`**: Returns pointer to first element
- **`array_end(Array *arr)`**: Returns pointer to one-past-last element (standard iterator pattern)
- **Warning**: These pointers may become invalid after add/remove operations

## Modification Functions
- **`array_add(Array *arr)`**: Adds new uninitialized element, returns pointer to it (index becomes `length - 1`)
- **`array_add_val(type, arr)`**: Macro that returns reference to newly added element for direct assignment
- **`array_reset(Array *arr, unsigned int num)`**: Resizes array to exactly `num` elements with undefined data
- **`array_clear(arr)`**: Macro that resets array to zero elements
- **`array_pop(Array *arr)`**: Removes the last element
- **`array_quick_remove(Array *arr, unsigned int i)`**: Efficient removal that may swap another element into position `i`
  - Returns `true` if a swap occurred, `false` otherwise

## Utility Functions
- **`array_sort(Array *arr, int (*compar)(const void *, const void *))`**: Sorts array using provided comparator function compatible with `qsort(3)`

## Iteration Macro
- **`array_foreach(var, arr)`**: Provides clean iteration syntax
  - `var` must be a pre-declared pointer variable of appropriate element type
  - Visits elements in index order
  - **Warning**: Do not add/remove elements during iteration

## Dependencies
- **`<stddef.h>`**: For `size_t` type
- **`<stdbool.h>`**: For `bool` type and `true`/`false` values

## Usage Pattern
The design follows a generic container pattern where:
1. Arrays store elements of uniform size specified at creation
2. Automatic memory management handles resizing
3. Type-safe macros provide convenient access while maintaining generic implementation
4. Iterator support enables standard C-style iteration patterns

## Sections

### Dynamic Array Data Structure Header Definition
Source: [array.h:1-58](../array.h#L1-L58)

This header file defines a dynamic array data structure that stores objects contiguously in memory with automatic resizing capabilities.

## Core Data Structure
- **Array**: An opaque struct typedef that represents the dynamic array container
- Objects are stored contiguously in memory but may move during operations (pointers to elements should not be relied upon)

## Memory Management Functions
- **array_new_(size_t object_size)**: Creates a new array with specified element size
- **array_new(type)**: Macro that creates array using `sizeof(type)` for convenience
- **array_free(Array *arr)**: Deallocates array memory

## Element Access Functions
- **array_get(Array *arr, unsigned int i)**: Returns pointer to element at index i
- **array_get_val(type, arr, i)**: Macro that dereferences element at index i to specified type
- **array_top(Array *arr)**: Returns pointer to last element
- **array_top_val(type, arr)**: Macro that dereferences last element to specified type
- **array_length(Array *arr)**: Returns number of elements currently in array

## Iteration Support
- **array_begin(Array *arr)**: Returns pointer to first element
- **array_end(Array *arr)**: Returns pointer to one-past-last element (standard iterator pattern)
- **array_foreach(var, arr)**: Macro for iterating through elements
  - Takes a pre-declared pointer variable and array
  - Visits elements in order of increasing index
  - Warning: should not be used while adding/removing elements

## Element Modification Functions
- **array_add(Array *arr)**: Adds new element and returns pointer to it
- **array_add_val(type, arr)**: Macro that returns reference to newly added element
- **array_reset(Array *arr, unsigned int num)**: Resizes array to specified number of elements (data undefined)
- **array_clear(arr)**: Macro that resets array to zero elements

## Element Removal Functions
- **array_pop(Array *arr)**: Removes element with highest index
- **array_quick_remove(Array *arr, unsigned int i)**: Fast removal that may swap another element into position i
  - Returns boolean indicating if a swap occurred
- **array_sort(Array *arr, int (*compar)(const void *, const void *))**: Sorts array using qsort-style comparator function

## Key Dependencies
- **stddef.h**: For `size_t` type definition
- **stdbool.h**: For `bool` type used in `array_quick_remove`

## Important Usage Notes
- Elements may move in memory during operations, so storing pointers to elements is unsafe
- Iterator functions (`array_begin`/`array_end`) may become invalid after add/remove operations
- The `array_foreach` macro should not be used while modifying the array


### Related
- [Parent](../)

---
Hash: 7ed68d0c403f96b2d1dd12621903879143c82a06e2161d15b2b322cb265317a0
Generated: 2025-07-07T20:21:19-04:00
