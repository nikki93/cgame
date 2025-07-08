# Summary: array.c

## Table of Contents

- [Dynamic Array Implementation in C](#dynamic-array-implementation-in-c)

This file implements a generic dynamic array data structure in C with automatic memory management and various utility functions.

## Core Data Structure

**`struct Array`** - The main array structure containing:
- `char *buf` - Buffer for storing elements (char* enables pointer arithmetic)
- `unsigned int capacity` - Allocated size of the buffer
- `unsigned int length` - Current number of elements
- `size_t object_size` - Size of each individual element

**`MIN_CAPACITY`** - Constant defining minimum array capacity (value: 2)

## Primary Functions

### Memory Management
- **`array_new_(size_t object_size)`** - Constructor that:
  - Allocates memory for Array struct
  - Sets initial capacity to MIN_CAPACITY
  - Allocates buffer with `object_size * capacity` bytes
  - Initializes length to 0
- **`array_free(Array *arr)`** - Destructor that frees both buffer and struct memory

### Element Access
- **`array_get(Array *arr, unsigned int i)`** - Returns pointer to element at index i
- **`array_top(Array *arr)`** - Returns pointer to last element
- **`array_length(Array *arr)`** - Returns current number of elements

### Iterator Support
- **`array_begin(Array *arr)`** - Returns pointer to first element
- **`array_end(Array *arr)`** - Returns pointer past the last element

### Dynamic Operations
- **`array_add(Array *arr)`** - Adds new element with automatic resizing:
  - Increments length
  - Doubles capacity if needed using `realloc`
  - Returns pointer to newly added element location

- **`array_reset(Array *arr, unsigned int num)`** - Resets array to specific size:
  - Frees existing buffer
  - Sets new length and capacity (minimum MIN_CAPACITY)
  - Allocates new buffer

- **`array_pop(Array *arr)`** - Removes last element with automatic downsizing:
  - Decrements length
  - Halves capacity if usage drops below 1/4 and capacity > MIN_CAPACITY

### Advanced Operations
- **`array_quick_remove(Array *arr, unsigned int i)`** - Efficient removal:
  - Copies last element to position i (if not removing last element)
  - Calls `array_pop()` to handle length/capacity
  - Returns boolean indicating if element was swapped

- **`array_sort(Array *arr, int (*compar)(const void *, const void *))`** - Sorts array using `qsort`

## Test Code (when ARRAY_TEST is defined)

### Test Data Structure
**`IntPair`** - Test struct with two integers (a, b)

### Test Functions
- **`dump(Array *arr)`** - Prints array state and contents
- **`int_compare(const void *a, const void *b)`** - Comparison function for integer sorting
- **`test_sort()`** - Demonstrates sorting functionality with integer array
- **`main()`** - Main test driver demonstrating:
  - Array creation and population
  - Element removal with `array_quick_remove`
  - Memory management verification
  - Sorting functionality

## Key Dependencies
- References **`array.h`** header file (likely contains macros like `array_new`, `array_add_val`, `array_foreach`)
- Uses standard library functions: `malloc`, `free`, `realloc`, `memcpy`, `qsort`
- Test code uses `printf` for output

## Memory Management Strategy
The implementation uses a doubling/halving strategy for dynamic resizing:
- **Growth**: Doubles capacity when length exceeds capacity
- **Shrinkage**: Halves capacity when usage drops below 25% of capacity (with minimum threshold)

## Sections

### Dynamic Array Implementation in C
Source: [array.c:1-184](../array.c#L1-L184)

This section implements a generic dynamic array data structure in C with automatic memory management and various utility functions.

## Core Data Structure

**`struct Array`** - The main array structure containing:
- `char *buf` - Buffer for storing elements (using char* for pointer arithmetic)
- `unsigned int capacity` - Allocated size of the buffer
- `unsigned int length` - Current number of elements
- `size_t object_size` - Size of each element in bytes

**`MIN_CAPACITY`** - Macro defining minimum array capacity as 2

## Primary Functions

### Memory Management
- **`array_new_(size_t object_size)`** - Creates new array with specified element size
  - Allocates memory for Array struct
  - Sets initial capacity to MIN_CAPACITY
  - Allocates buffer memory
  - Returns pointer to new Array

- **`array_free(Array *arr)`** - Deallocates array memory
  - Frees buffer memory
  - Frees Array struct memory

### Element Access
- **`array_get(Array *arr, unsigned int i)`** - Returns pointer to element at index i
- **`array_top(Array *arr)`** - Returns pointer to last element
- **`array_begin(Array *arr)`** - Returns pointer to first element
- **`array_end(Array *arr)`** - Returns pointer past last element
- **`array_length(Array *arr)`** - Returns current number of elements

### Dynamic Operations
- **`array_add(Array *arr)`** - Adds space for new element
  - Increments length
  - Doubles capacity if needed (capacity << 1)
  - Reallocates buffer if expansion occurs
  - Returns pointer to new element location

- **`array_pop(Array *arr)`** - Removes last element
  - Decrements length
  - Halves capacity if usage drops below 1/4 and capacity > MIN_CAPACITY
  - Reallocates buffer if shrinking occurs

- **`array_quick_remove(Array *arr, unsigned int i)`** - Removes element at index i
  - Copies last element to position i (doesn't preserve order)
  - Calls array_pop() to handle size reduction
  - Returns true if element was moved, false if removing last element

- **`array_reset(Array *arr, unsigned int num)`** - Resets array to specific size
  - Frees existing buffer
  - Sets length to num
  - Sets capacity to max(num, MIN_CAPACITY)
  - Allocates new buffer

### Utility Functions
- **`array_sort(Array *arr, int (*compar)(const void *, const void *))`** - Sorts array using qsort

## Test Code (when ARRAY_TEST is defined)

### Test Data Structure
- **`IntPair`** typedef - Structure with two integers (a, b)

### Test Functions
- **`dump(Array *arr)`** - Prints array contents for IntPair arrays
- **`int_compare(const void *a, const void *b)`** - Comparison function for integer sorting
- **`test_sort()`** - Demonstrates array sorting with integers
- **`main()`** - Main test function demonstrating:
  - Array creation and element addition
  - Quick removal operations
  - Memory management behavior
  - Sorting functionality

## Key Features
- **Automatic resizing**: Doubles capacity when full, halves when usage drops below 1/4
- **Generic design**: Works with any data type through object_size parameter
- **Efficient removal**: Quick remove swaps with last element for O(1) operation
- **Memory efficiency**: Shrinks capacity when usage is low
- **Standard library integration**: Uses qsort for sorting

## Dependencies
- Standard C libraries: `string.h`, `stdlib.h`
- Header file: `array.h` (referenced but not shown)


### Related
- [Parent](../)

---
Hash: a807a9a806194c576d268a344842b1208a6401e40525285e40c748cce5a79e01
Generated: 2025-07-07T20:21:19-04:00
