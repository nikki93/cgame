--- load some modules ---------------------------------------------------------

serpent = require 'serpent'

--- inherit ffi ---------------------------------------------------------------

local ffi = require 'ffi'
local cgame = setmetatable({}, { __index = ffi.C })


--- lua utils/wrappers --------------------------------------------------------

-- return serialized string for cdata, func must be of form
-- void (typeof(cdata) *, Serializer *)
function cgame.c_serialize(func, cdata)
    s = cgame.serializer_open_str()
    func(cdata, s)
    dump = ffi.string(cgame.serializer_get_str(s))
    cgame.serializer_close(s)
    return dump
end

-- return struct deserialized from string 'str', func must be of form
-- void (ctype *, Deserializer *)
function cgame.c_deserialize(ctype, func, str)
    cdata = ctype { }
    s = cgame.deserializer_open_str(str)
    func(cdata, s)
    cgame.deserializer_close(s)
    return cdata
end

-- create a save/load function given C type and C save/load functions all
-- as string names
function cgame.c_save_load(ctype, c_save, c_load)
    return function (cdata)
        cdump = cgame.c_serialize(loadstring('return ' .. c_save)(), cdata)
        cdumpe = ("%q"):format(cdump):gsub("\010", "n"):gsub("\026", "\\026")
        return 'cgame.c_deserialize(' .. ctype .. ', ' .. c_load .. ', '
                .. cdumpe .. ')'
    end
end

cgame.Vec2 = ffi.metatype('Vec2',
{
    __add = function (u, v) return cgame.vec2_add(u, v) end,
    __index =
    {
        __serialize = cgame.c_save_load('cgame.Vec2', 'cgame.vec2_save',
            'cgame.vec2_load')
    },
})

cgame.Mat3 = ffi.metatype('Mat3',
{
    __index =
    {
        __serialize = cgame.c_save_load('cgame.Mat3', 'cgame.mat3_save',
            'cgame.mat3_load')
    },
})


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

