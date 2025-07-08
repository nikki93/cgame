# Summary: gfx.h

## Table of Contents

- [Graphics Utility Header - Shader Program Management and Vertex Attribute Binding](#graphics-utility-header---shader-program-management-and-vertex-attribute-binding)

This is a header file that provides OpenGL graphics utilities and macros for shader program management and vertex attribute binding.

## Dependencies
- Includes `GL/glew.h` for OpenGL Extension Wrangler Library support

## Function Declarations

### `gfx_create_program`
- **Purpose**: Compiles and links an OpenGL shader program from shader file paths
- **Parameters**:
  - `const char *vert_path`: Path to vertex shader file (can be NULL)
  - `const char *geom_path`: Path to geometry shader file (can be NULL)  
  - `const char *frag_path`: Path to fragment shader file (can be NULL)
- **Returns**: `GLuint` program ID
- **Note**: Does not call `glUseProgram()` after creation

### `gfx_free_program`
- **Purpose**: Frees/deletes an OpenGL shader program
- **Parameters**: `GLuint program` - the program ID to free

## Utility Macros

### `poffsetof`
- **Purpose**: Gets the pointer offset of a field within a struct type
- **Parameters**:
  - `type`: The struct type
  - `field`: The field name within the struct
- **Returns**: `void *` pointer to the field offset
- **Implementation**: Uses pointer arithmetic with a null pointer cast to calculate offset

### `gfx_bind_vertex_attrib`
- **Purpose**: Binds vertex attribute data to a specific field in a struct type for OpenGL rendering
- **Parameters**:
  - `program`: OpenGL program ID
  - `gl_type`: OpenGL data type (e.g., `GL_FLOAT`)
  - `components`: Number of components in the attribute
  - `param_name`: String name of the parameter as it appears in the shader program
  - `type`: The struct type containing the vertex data
  - `field`: The specific field within the struct to bind
- **Implementation Steps**:
  1. Gets the attribute location using `glGetAttribLocation()`
  2. Sets up vertex attribute pointer using `glVertexAttribPointer()` with:
     - Attribute location
     - Number of components
     - Data type
     - Normalization disabled (`GL_FALSE`)
     - Struct size as stride
     - Field offset as pointer offset
  3. Enables the vertex attribute array using `glEnableVertexAttribArray()`
- **Usage Examples**: Referenced in `sprite.c` and `text.c` files

## Header Guard
- Uses standard header guard pattern with `GFX_H` macro to prevent multiple inclusions

## Sections

### Graphics Utility Header - Shader Program Management and Vertex Attribute Binding
Source: [gfx.h:1-38](../gfx.h#L1-L38)

This header file defines core graphics utilities for OpenGL shader program management and vertex attribute binding. It serves as a foundational graphics interface with dependencies on OpenGL/GLEW.

## Dependencies
- **OpenGL Extension Wrangler Library (GLEW)**: `#include <GL/glew.h>`

## Defined Entities

### Functions
- **`gfx_create_program`**: Creates and links an OpenGL shader program
  - Parameters: `vert_path` (vertex shader file path), `geom_path` (geometry shader file path), `frag_path` (fragment shader file path)
  - Any of the shader paths can be NULL
  - Returns: `GLuint` program ID
  - Note: Does NOT automatically call `glUseProgram()` on the created program

- **`gfx_free_program`**: Cleans up/destroys an OpenGL shader program
  - Parameter: `program` (GLuint program ID to free)

### Macros
- **`poffsetof(type, field)`**: Calculates pointer offset of a struct field
  - Purpose: Gets the memory offset of a specific field within a struct type
  - Implementation: Uses pointer arithmetic with null pointer casting
  - Returns: `void*` pointer representing the offset

- **`gfx_bind_vertex_attrib(program, gl_type, components, param_name, type, field)`**: Binds vertex attribute data to shader parameters
  - **Parameters**:
    - `program`: OpenGL program ID
    - `gl_type`: OpenGL data type (e.g., `GL_FLOAT`)
    - `components`: Number of components in the attribute
    - `param_name`: String name of the shader parameter
    - `type`: C struct type containing the vertex data
    - `field`: Specific field within the struct to bind
  - **Internal logic**:
    1. Retrieves attribute location using `glGetAttribLocation()`
    2. Configures vertex attribute pointer with `glVertexAttribPointer()`
    3. Enables the vertex attribute array with `glEnableVertexAttribArray()`
  - **Usage examples**: Referenced in `sprite.c` and `text.c` files

## Usage Context
This header provides a simplified interface for common OpenGL operations, particularly shader program lifecycle management and vertex buffer attribute binding. The macro-based approach for vertex attribute binding reduces boilerplate code when setting up vertex array objects with structured vertex data.


### Related
- [Parent](../)

---
Hash: f979f793409817d5517a83890594d52cce2019b7e67515b019c3365cf61ae9ba
Generated: 2025-07-07T20:21:19-04:00
