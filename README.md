NOTE: after building you must run the 'cgame' executable with working directory
as git root (containing the 'data' directory) -- so if you do a CMake build in
'build' then run it as './build/cgame' from the repository root

test/keyboard_controlled.{c,h} shows an example of implementing a system in C,
data/test/oscillator.lua and data/test/rotator.lua show examples of
implementing systems in Lua. test/test.c shows how to set up entities in C,
and data/test/test.lua shows the same in Lua.

Building
---

CMake should make it easy to set up. GLFW, GLEW, stb_image are dependencies and
are included in ext/. LuaJIT is an external dependency needed for now but I'll
get it into the repository soon making it self-contained except for OpenGL and
the standard library.

Running
---

After building just run the 'cgame' executable, making sure your working
directory is the root of the project (cgame looks for data in './data').

