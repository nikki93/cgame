--
-- Entity -> data map
--
-- can't use normal Lua tables because Entity is cdata, which isn't
-- hashed right
--

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
        map[k.id] = { ['k'] = cg.Entity { id = k.id }, ['v'] = v }
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

