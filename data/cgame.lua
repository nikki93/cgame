--- load some modules ---------------------------------------------------------

local serpent = require 'serpent'

--- inherit ffi ---------------------------------------------------------------

local ffi = require 'ffi'
local cgame = setmetatable({}, { __index = ffi.C })


--- lua utils/wrappers --------------------------------------------------------

-- dereference a cdata from a pointer
function cgame.__deref_cdata(ct, p)
    return ffi.cast(ct, p)[0]
end

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
    __add = cgame.vec2_add,
    __sub = cgame.vec2_sub,
    __div = cgame.vec2_div,
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


-- generic add/remove, get/set for any system, property -- needs corresponding
-- C functions of the form sys_add()/sys_remove(),
-- sys_get_prop(ent)/sys_set_prop(ent, val)

function cgame.getter(sys, prop) return cgame[sys .. '_get_' .. prop] end
function cgame.setter(sys, prop) return cgame[sys .. '_set_' .. prop] end
function cgame.get(sys, prop, ent) return cgame.getter(sys, prop)(ent) end
function cgame.set(sys, prop, ent, val) cgame.setter(sys, prop)(ent, val) end

function cgame.adder(sys) return cgame[sys .. '_add'] end
function cgame.remover(sys) return cgame[sys .. '_remove'] end
function cgame.add(sys, ent, props)
    cgame.adder(sys)(ent)
    if (props) then
        for k, v in pairs(props) do
            cgame.set(sys, k, ent, v)
        end
    end
end
function cgame.remove(sys, ent) cgame.remover(sys)(ent) end


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

local sys_counter = 0
function cgame.add_system(t)
    if not t.name then
        t.name = 'sys_' .. sys_counter
        sys_counter = sys_counter + 1
    end

    systems[t.name] = t
end

function cgame.remove_system(name)
    systems[name] = nil
end


-------------------------------------------------------------------------------

print('lua: loaded cgame')
return cgame

