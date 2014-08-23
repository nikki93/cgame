-- can set unique string names per object, can find by name
-- name property is empty string or nil for no name

cs.name = {}

local entity_name = cg.entity_table() -- entity -> name map
local name_entity = {}                -- name -> entity map

function cs.name.add(ent)
end
function cs.name.has(ent)
    return true
end
function cs.name.remove(ent)
    name = entity_name[ent] 
    if name then
        name_entity[name] = nil
        entity_name[ent] = nil
    end
end

function cs.name.set_name(ent, name)
    cs.name.remove(ent) -- remove old name
    if name == '' or name == nil then return end -- no name
    if entity_name[ent] == name then return end -- already same

    -- someone else has name?
    if name_entity[name] ~= nil then
        -- already checked we don't have this name, must be another
        error("name: different entity already has name '" .. name .. "'")
    end

    name_entity[name] = ent
    entity_name[ent] = name
end
function cs.name.get_name(ent, name)
    return entity_name[ent] or ''
end

function cs.name.find(name)
    return name_entity[name] and name_entity[name] or cg.Entity(cg.entity_nil)
end

function cs.name.update_all()
    cg.entity_table_remove_destroyed(entity_name, cs.name.remove)
end

function cs.name.save_all()
    return entity_name
end

local counter = 0
function cs.name.load_all(d)
    cg.entity_table_remove_destroyed(entity_name, cs.name.remove)
    for ent, rname in pairs(d) do
        local name = rname

        -- make up new name if clashes
        while name_entity[name] ~= nil do
            name, r = string.gsub(rname, '-%d+$', '-' .. counter)
            if r == 0 then name = string.format('%s-%d', rname, counter) end
            counter = counter + 1
        end

        cs.name.set_name(ent, name)
    end
end
