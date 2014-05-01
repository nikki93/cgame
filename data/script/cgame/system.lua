local serpent = require 'serpent'

local systems_mt = {
    __index = function (t, k)
        local v = rawget(t, k)

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
cs = cgame.systems

function cgame.__fire_event(event, args)
    -- store system names before firing event because systems list may change

    local sysnames = {}
    for name, sys in pairs(cgame.systems) do
        sysnames[name] = sys.receive_events == nil or sys.receive_events
    end

    for name, _ in pairs(sysnames) do
        local system = cgame.systems[name]
        if system.enabled == nil or system.enabled then
            local func = system[event]
            if func then func(args) end
        end
    end
end

function cgame.__save_all()
    local data = {}

    for name, system in pairs(cgame.systems) do
        if system.auto_saveload then
            data[name] = system
        elseif system.save_all then
            -- has special load_all() event
            data[name] = system.save_all()
        end
    end

    return serpent.dump(data, { indent = '  ', nocode = true })
end

function cgame.__load_all(str)
    local f, err = loadstring(str)
    if err then error(err) end
    local data = f()

    for name, dump in pairs(data) do
        local system = rawget(cgame.systems, name)
        if system then
            -- system currently exists, must merge
            if system.auto_saveload then
                for k, v in pairs(dump) do
                    if cgame.is_entity_table(system[k]) then
                        cgame.entity_table_merge(system[k], v)
                    elseif --[[ system.auto_saveload_functions
                        or --]] type(v) ~= 'function' then
                            system[k] = v
                    end
                end
            elseif system.load_all then
                system.load_all(dump)
            end
        elseif dump.auto_saveload then
            -- system doesn't exist currently, just dump it in
            rawset(cgame.systems, name, dump)
        end
    end
end


-- generic add/remove, get/set for any system, property -- needs corresponding
-- C functions of the form sys_add()/sys_remove(),
-- sys_get_prop(ent)/sys_set_prop(ent, val)

function cgame.getter(sys, prop) return cgame.systems[sys]['get_' .. prop] end
function cgame.setter(sys, prop)return cgame.systems[sys]['set_' .. prop] end
function cgame.get(sys, prop, ...)
    return cgame.getter(sys, prop)(unpack({...}))
end
function cgame.set(sys, prop, ...) cgame.setter(sys, prop)(unpack({...})) end

function cgame.adder(sys) return cgame.systems[sys]['add'] end
function cgame.remover(sys) return cgame.systems[sys]['remove'] end
function cgame.add(sys, ent, props)
    -- multi-add?
    if type(sys) == 'table' then
        ent = ent or sys.ent or cgame.entity_create()
        sys.ent = nil
        for k, v in pairs(sys) do cgame.add(k, ent, v) end
        return ent
    end

    -- all entities are already in 'entity' system
    if sys ~= 'entity' then
        cgame.adder(sys)(ent)
    end
    if (props) then
        for k, v in pairs(props) do
            cgame.set(sys, k, ent, v)
        end
    end
end
function cgame.remove(sys, ...) cgame.remover(sys)(unpack({...})) end

cs.meta = { receive_events = false }
cs.meta.props = {}

function cg.simple_sys()
    local sys = { auto_saveload = true }
    sys.tbl = cg.entity_table()

    -- add/remove
    function sys.simple_add(ent)
        if not sys.tbl[ent] then
            local entry = { ent = ent }
            sys.tbl[ent] = entry
            if sys.create then sys.create(entry) end
        end
    end
    sys.add = sys.simple_add
    function sys.simple_remove(ent)
        local entry = sys.tbl[ent]
        if entry then
            if sys.destroy then sys.destroy(entry) end
            sys.tbl[ent] = nil
        end
    end
    sys.remove = sys.simple_remove
    function sys.simple_has(ent)
        return sys.tbl[ent] ~= nil
    end
    sys.has = sys.simple_has

    -- update
    function sys.simple_update_all()
        cg.entity_table_remove_destroyed(sys.tbl, sys.remove)
        for _, entry in pairs(sys.tbl) do
            if cs.timing.get_paused() then
                if sys.paused_update then sys.paused_update(entry) end
            else
                if sys.unpaused_update then sys.unpaused_update(entry) end
            end
        end
    end
    sys.update_all = sys.simple_update_all

    return sys
end
