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


cgame.Entity = ffi.metatype('Entity',
{
    __eq = function (a, b)
        return type(a) == 'cdata' and type(b) == 'cdata'
            and ffi.istype('Entity', a) and ffi.istype('Entity', b)
            and cgame.entity_eq(a, b)
    end,
    __index =
    {
        __serialize = cgame.c_save_load('cgame.Entity', 'cgame.entity_save',
            'cgame.entity_load')
    },
})

cgame.Vec2 = ffi.metatype('Vec2',
{
    __add = cgame.vec2_add,
    __sub = cgame.vec2_sub,
    __mul = function (a, b)
        if type(a) == 'number' then return cgame.vec2_scalar_mul(b, a)
        elseif type(b) == 'number' then return cgame.vec2_scalar_mul(a, b)
        else return cgame.vec2_mul(a, b) end
    end,
    __div = function (a, b)
        if type(b) == 'number' then return cgame.vec2_scalar_div(a, b)
        else return cgame.vec2_div(a, b) end
    end,
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
    -- multi-add?
    if type(sys) == 'table' then
        ent = ent or cgame.entity_create()
        for k, v in pairs(sys) do cgame.add(k, ent, v) end
        return ent
    end

    cgame.adder(sys)(ent)
    if (props) then
        for k, v in pairs(props) do
            cgame.set(sys, k, ent, v)
        end
    end
end
function cgame.remove(sys, ent) cgame.remover(sys)(ent) end


---- entity_table (for Entity-keyed Lua tables) -------------------------------

local entity_table_mt = {
    __newindex = function (t, k, v)
        map = rawget(t, 'map')

        -- remove
        if v == nil then
            if map == nil then return end
            map[k.id] = nil
            return
        end

        -- add
        if not map then
            map = {}
            rawset(t, 'map', map)
        end
        map[k.id] = { ['k'] = k, ['v'] = v }
    end,

    __index = function (t, k)
        map = rawget(t, 'map')

        -- no map => empty
        if not map then return nil end

        -- find slot, return value in it
        slot = map[k.id]
        if not slot then return nil end
        return slot.v
    end,

    __serialize_f = function (t)
        return 'cgame.__entity_table_load', (rawget(t, 'map') or {})
    end,

    -- allows iteration using pairs(...)
    __pairs = function (t)
        map = rawget(t, 'map')

        return function (_, k)
            -- no map => empty
            if not map then return nil, nil end

            -- get next in map
            id, slot = next(map, k and k.id or nil)
            if not id then return nil, nil end -- end
            return slot.k, slot.v
        end, nil, nil
    end,
}

function cgame.entity_table()
    return setmetatable({}, entity_table_mt)
end

function cgame.__entity_table_load(t)
    e = cgame.entity_table()
    for _, slot in pairs(t) do
        e[slot.k] = slot.v
    end
    return e
end

function cgame.entity_table_merge(t, d)
    for _, slot in pairs(rawget(d, 'map')) do
        t[slot.k] = slot.v
    end
end


--- lua systems ---------------------------------------------------------------

local systems_mt = {
    __index = function (t, k)
        v = rawget(t, k)

        if v == nil then
            local mt = {
                __index = function (_, k2)
                    return cgame[k .. '_' .. k2]
                end,
            }
            return setmetatable({}, mt)
        end
        return v
    end,
}
cgame.systems = setmetatable({}, systems_mt)

function cgame.__fire_event(event, args)
    for _, system in pairs(cgame.systems) do
        func = system[event]
        if func then func(args) end
    end
end

function cgame.__save_all()
    local data = {}

    for name, system in pairs(cgame.systems) do
        if system.save_all then
            -- has special load_all() event
            data[name] = system.save_all()
        else
            -- no special load_all() event, just dump it
            data[name] = system
        end
    end

    return serpent.dump(data)
end

function cgame.__load_all(str)
    local data = loadstring(str)()

    for name, dump in pairs(data) do
        local system = rawget(cgame.systems, name)
        if system then
            if system.load_all then
                system.load_all(dump)
            else
                -- doesn't have a special load_all() event, just
                -- load it back, but merge entity_tables
                for k, v in pairs(dump) do
                    if type(system[k]) == 'table'
                    and getmetatable(system[k]) == entity_table_mt then
                        cgame.entity_table_merge(system[k], v)
                    else
                        system[k] = v
                    end
                end
            end
        end
    end
end


-------------------------------------------------------------------------------

print('lua: loaded cgame')
return cgame

