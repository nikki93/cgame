--- inherit ffi ---------------------------------------------------------------

local ffi = require 'ffi'
local cgame = setmetatable({}, { __index = ffi.C })


--- lua utils/wrappers --------------------------------------------------------

cgame.Vec2 = ffi.metatype('Vec2',
{
    __add = function (u, v) return cgame.vec2_add(u, v) end,
})

cgame.Mat3 = ffi.typeof('Mat3')


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

v = cgame.vec2(2, 3) + cgame.vec2(5, 6)
print(v.x .. ', ' .. v.y)

print('lua: loaded cgame')
return cgame

