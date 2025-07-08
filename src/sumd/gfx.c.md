# Summary: gfx.c

## Table of Contents

- [OpenGL Shader Compilation and Program Creation](#opengl-shader-compilation-and-program-creation)

This section implements OpenGL shader compilation and program creation functionality for a graphics system.

## Functions Defined

### `_compile_shader(GLuint shader, const char *filename)`
A static helper function that compiles a shader from a source file:
- **File Reading Process:**
  - Opens the specified shader file in binary mode
  - Determines file size using `fseek()` and `ftell()`
  - Allocates memory buffer for file contents
  - Reads entire file into memory and null-terminates it
  - Closes the file and frees the buffer after use
- **Shader Compilation:**
  - Sets shader source code using `glShaderSource()`
  - Compiles the shader with `glCompileShader()`
  - Retrieves compilation status using `glGetShaderiv()` with `GL_COMPILE_STATUS`
  - Logs compilation results and any error messages via `console_printf()`
  - Returns the compilation status as a `GLint`

### `gfx_create_program(const char *vert_path, const char *geom_path, const char *frag_path)`
Creates a complete OpenGL shader program from optional shader files:
- **Shader Compilation Phase:**
  - Uses a macro `compile(shader, type)` to conditionally compile each shader type
  - Creates vertex shader (`GL_VERTEX_SHADER`) if `vert_path` is provided
  - Creates geometry shader (`GL_GEOMETRY_SHADER`) if `geom_path` is provided
  - Creates fragment shader (`GL_FRAGMENT_SHADER`) if `frag_path` is provided
  - Returns 0 if any shader compilation fails
- **Program Linking Phase:**
  - Creates a new OpenGL program object with `glCreateProgram()`
  - Attaches all successfully compiled shaders to the program
  - Links the program using `glLinkProgram()`
  - Cleans up by deleting individual shader objects (OpenGL automatically detaches them)
  - Returns the program handle

## Dependencies
- **Headers:** `gfx.h`, `stdlib.h`, `stdio.h`, `console.h`
- **External Functions:** `console_printf()` for logging output
- **OpenGL Functions:** `glCreateShader()`, `glShaderSource()`, `glCompileShader()`, `glGetShaderiv()`, `glGetShaderInfoLog()`, `glCreateProgram()`, `glAttachShader()`, `glLinkProgram()`, `glDeleteShader()`

## Key Features
- Flexible shader program creation supporting any combination of vertex, geometry, and fragment shaders
- Comprehensive error logging and status reporting
- Automatic memory management and cleanup
- File-based shader source loading

## Sections

### OpenGL Shader Compilation and Program Creation
Source: [gfx.c:1-67](../gfx.c#L1-L67)

This section implements OpenGL shader compilation and program creation functionality with two main functions:

## Functions Defined

• **`_compile_shader(GLuint shader, const char *filename)`** - Static helper function that compiles a shader from file
  - Reads shader source code from file specified by `filename`
  - File reading process:
    - Opens file in binary mode (`"rb"`)
    - Determines file size using `fseek`/`ftell`/`rewind`
    - Allocates memory buffer of `file_size + 1` characters
    - Reads entire file contents into buffer
    - Null-terminates the string
    - Closes file and frees buffer after use
  - Shader compilation process:
    - Logs compilation attempt via `console_printf`
    - Calls `glShaderSource` to set shader source code
    - Calls `glCompileShader` to compile the shader
    - Retrieves compilation status using `glGetShaderiv` with `GL_COMPILE_STATUS`
    - Logs success/failure status
    - Retrieves and logs compilation info/errors using `glGetShaderInfoLog`
  - Returns compilation status (`GLint`)

• **`gfx_create_program(const char *vert_path, const char *geom_path, const char *frag_path)`** - Creates complete OpenGL shader program
  - Parameters allow optional vertex, geometry, and fragment shader file paths
  - Uses macro `compile(shader, type)` to conditionally compile each shader type:
    - Only compiles shader if corresponding path is provided (non-null)
    - Creates shader object with `glCreateShader`
    - Calls `_compile_shader` helper function
    - Returns 0 if any compilation fails
  - Shader types handled:
    - Vertex shader: `GL_VERTEX_SHADER`
    - Geometry shader: `GL_GEOMETRY_SHADER` 
    - Fragment shader: `GL_FRAGMENT_SHADER`
  - Program linking process:
    - Creates program object with `glCreateProgram`
    - Attaches all successfully compiled shaders using `glAttachShader`
    - Links program with `glLinkProgram`
    - Cleans up by deleting individual shader objects with `glDeleteShader`
  - Returns the created program ID (`GLuint`)

## Dependencies

• **External headers**: `gfx.h`, `stdlib.h`, `stdio.h`, `console.h`
• **OpenGL functions**: `glShaderSource`, `glCompileShader`, `glGetShaderiv`, `glGetShaderInfoLog`, `glCreateShader`, `glCreateProgram`, `glAttachShader`, `glLinkProgram`, `glDeleteShader`
• **Console logging**: `console_printf` function for status reporting
• **OpenGL constants**: `GL_COMPILE_STATUS`, `GL_VERTEX_SHADER`, `GL_GEOMETRY_SHADER`, `GL_FRAGMENT_SHADER`

The code provides a complete pipeline for loading shader files from disk, compiling them, and creating a linked OpenGL program with proper error reporting and resource cleanup.


### Related
- [Parent](../)

---
Hash: 0ae0c81e280bd25162f9ddbb2adf0318e0609c16975f3ac1aa7de2e0dfa56a5c
Generated: 2025-07-07T20:21:19-04:00
