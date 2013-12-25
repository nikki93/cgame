--- ffi -----------------------------------------------------------------------

local ffi = require 'ffi'
local file = assert(io.open('cgame_ffi.h'))
ffi.cdef(file:read('*all'))
local C = ffi.C
local cgame = setmetatable({}, { __index = C })


--- lua utils -----------------------------------------------------------------

cgame.Vec2 = ffi.typeof('Vec2')


--- lua systems ---------------------------------------------------------------

local systems = {}

function cgame.__fire_event(event, args)
    for _, handlers in pairs(systems) do
        func = handlers[event]
        if func then func(args) end
    end
end

function cgame.add_system(name, handlers)
    systems[name] = handlers
end

-------------------------------------------------------------------------------

print('lua: loaded cgame')
return cgame

