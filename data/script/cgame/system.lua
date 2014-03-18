local serpent = require 'serpent'

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
cs = cgame.systems

function cgame.__fire_event(event, args)
    for _, system in pairs(cgame.systems) do
        func = system[event]
        if func then func(args) end
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

    return serpent.dump(data, { indent = '  ' })
end

function cgame.__load_all(str)
    local data = loadstring(str)()

    for name, dump in pairs(data) do
        local system = rawget(cgame.systems, name)
        if system then
            -- system currently exists, must merge
            if system.auto_saveload then
                for k, v in pairs(dump) do
                    if type(system[k]) == 'table'
                    and getmetatable(system[k]) == entity_table_mt then
                        cgame.entity_table_merge(system[k], v)
                    else
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
function cgame.remove(sys, ...) cgame.remover(sys)(unpack({...})) end

