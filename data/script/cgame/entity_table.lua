--
-- Entity -> data map
--
-- can't use normal Lua tables because Entity is cdata, which isn't
-- hashed right
--

local function bind_defaults(t, v)
    if type(v) == 'table' then
        local defaults = rawget(t, 'defaults')
        if defaults then
            setmetatable(v, { __index = defaults })
        end
    end
end

local entity_table_mt = {
    __newindex = function (t, k, v)
        local map = rawget(t, 'map')

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
        bind_defaults(t, v)
        map[k.id] = { ['k'] = cg.Entity(k), ['v'] = v }
    end,

    __index = function (t, k)
        local map = rawget(t, 'map')

        -- no map => empty
        if not map then return nil end

        -- find slot, return value in it
        local slot = map[k.id]
        if not slot then return nil end
        return slot.v
    end,

    __serialize_f = function (t)
        local map = rawget(t, 'map') or {}

        -- don't save filtered-out entities
        local filtered = {}
        for k, slot in pairs(map) do
            if cg.entity_get_save_filter(slot.k) then
                filtered[k] = slot
            end
        end
        return 'cg.__entity_table_load', filtered
    end,

    -- allows iteration using pairs(...)
    __pairs = function (t)
        local map = rawget(t, 'map')

        return function (_, k)
            -- no map => empty
            if not map then return nil, nil end

            -- get next in map
            local id, slot = next(map, k and k.id or nil)
            if not id then return nil, nil end -- end
            return slot.k, slot.v
        end, nil, nil
    end,
}

function cg.is_entity_table(t)
    return type(t) == 'table' and getmetatable(t) == entity_table_mt
end

function cg.entity_table()
    return setmetatable({}, entity_table_mt)
end

function cg.entity_table_empty(t)
    for _ in pairs(t) do return false end
    return true
end

function cg.__entity_table_load(t)
    local e = cg.entity_table()
    for _, slot in pairs(t) do
        e[slot.k] = slot.v
    end
    return e
end

function cg.entity_table_merge(t, d)
    for _, slot in pairs(rawget(d, 'map') or {}) do
        bind_defaults(t, slot.v)
        t[slot.k] = slot.v
    end
end

function cg.entity_table_remove_destroyed(t, f)
    for e in pairs(t) do
        if cs.entity.destroyed(e) then f(e) end
    end
end

-- use to easily define properties with default values stored per-entity in a 
-- cg.entity_table
-- sys is the system, tbl is the table properties are stored in, name is the
-- name of the property and default is the default value if unset
function cg.simple_prop(sys, name, default, tbl, set, get)
    if tbl == nil then tbl = sys.tbl end

    -- update defaults
    if default ~= nil then
        local defaults = rawget(tbl, 'defaults')
        if not defaults then
            defaults = {}
            rawset(tbl, 'defaults', defaults)
            for _, v in pairs(tbl) do
                bind_defaults(tbl, v)
            end
        end
        defaults[name] = default
    end

    -- setter
    if set ~= false then
        sys['set_' .. name] = function (ent, val)
            local t = tbl[ent]
            if t then t[name] = val end
        end
    end

    -- getter
    if get ~= false then
        sys['get_' .. name] = function (ent)
            local t = tbl[ent]
            if t then return t[name] end
        end
    end
end

function cg.simple_props(sys, props, tbl)
    for name, default in pairs(props) do
        cg.simple_prop(sys, name, default, tbl)
    end
end
