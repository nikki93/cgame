--- inherit ffi ---------------------------------------------------------------

local ffi = require 'ffi'
local cgame = setmetatable({}, { __index = ffi.C })


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

