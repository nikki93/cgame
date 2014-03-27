local ffi = require 'ffi'

-- dereference a cdata from a pointer
function cgame.__deref_cdata(ct, p)
    return ffi.cast(ct, p)[0]
end

-- return serialized string for cdata, func must be of form
-- void (typeof(cdata) *, Serializer *)
function cgame.c_serialize(func, cdata)
    local s = cgame.serializer_open_str()
    func(cdata, s)
    local dump = ffi.string(cgame.serializer_get_str(s))
    cgame.serializer_close(s)
    return dump
end

-- return struct deserialized from string 'str', func must be of form
-- void (ctype *, Deserializer *)
function cgame.c_deserialize(ctype, func, str)
    local cdata = ctype { }
    local s = cgame.deserializer_open_str(str)
    func(cdata, s)
    cgame.deserializer_close(s)
    return cdata
end

-- create a save/load function given C type and C save/load functions all
-- as string names
function cgame.c_save_load(ctype, c_save, c_load)
    return function (cdata)
        local cdump = cgame.c_serialize(loadstring('return ' .. c_save)(),
                                        cdata)
        return 'cgame.c_deserialize(' .. ctype .. ', ' .. c_load .. ', '
            .. cgame.safestr(cdump) .. ')'
    end
end


--- Entity ---------------------------------------------------------------------

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


--- Vec2 -----------------------------------------------------------------------

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
        elseif type(a) == 'number' then return cgame.scalar_vec2_div(a, b)
        else return cgame.vec2_div(a, b) end
    end,
    __index =
    {
        __serialize = cgame.c_save_load('cgame.Vec2', 'cgame.vec2_save',
            'cgame.vec2_load')
    },
})


--- Mat3 -----------------------------------------------------------------------

cgame.Mat3 = ffi.metatype('Mat3',
{
    __index =
    {
        __serialize = cgame.c_save_load('cgame.Mat3', 'cgame.mat3_save',
            'cgame.mat3_load')
    },
})

--- BBox -----------------------------------------------------------------------

cgame.BBox = ffi.metatype('BBox', {})
