local ffi = require 'ffi'

local file = assert(io.open('cgame_ffi.h'))
ffi.cdef(file:read('*all'))
local C = ffi.C

-- add some manual wrappers/extensions to make it prettier

local cgame = setmetatable(
{
    Vec2 = ffi.typeof('Vec2')
}, { __index = C })

return cgame

