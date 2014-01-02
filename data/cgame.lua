--- load some modules ---------------------------------------------------------

serpent = require 'serpent'

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
    for _, system in pairs(systems) do
        func = system[event]
        if func then func(args) end
    end
end

function cgame.__save_all()
    -- make table of all system dumps
    local tbl = {}
    for name, system in pairs(systems) do
        if system.save_all then
            tbl[name] = system.save_all()
        end
    end
    return serpent.dump(tbl)
end

function cgame.__load_all(str)
    -- load table and tell systems
    local tbl = loadstring(str)()
    for name, dump in pairs(tbl) do
        local system = systems[name]
        if system.load_all then
            systems[name].load_all(dump)
        end
    end
end

function cgame.add_system(name, handlers)
    systems[name] = handlers
end


-------------------------------------------------------------------------------

print('lua: loaded cgame')
return cgame

