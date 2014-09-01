cs.group = {}

local group_entities = {}                 -- group name --> entity_table
local entity_groups = cg.entity_table()   -- entity --> set of group names

-- iterate over a group collection, which can be a string of
-- space-separated group names or a table with group names as keys
local function _groups(groups)
    if type(groups) == 'string' then
        return string.gmatch(groups, '%S+')
    end
    return pairs(groups)
end

function cs.group.add(ent, groups)
    -- if no groups parameter, nothing to do
    if not groups then return end

    for group in _groups(groups) do
        -- connect both ways
        if not group_entities[group] then
            group_entities[group] = cg.entity_table()
        end
        group_entities[group][ent] = true
        if not entity_groups[ent] then
            entity_groups[ent] = {}
        end
        entity_groups[ent][group] = true
    end
end

cs.group.add_groups = cs.group.add

function cs.group.remove(ent, groups)
    if type(groups) == 'nil' then
        -- no groups given, remove from all
        if entity_groups[ent] then
            groups = entity_groups[ent]
        else
            return -- no groups to remove from
        end
    end

    for group in _groups(groups) do
        -- disconnect both ways
        if group_entities[group] then
            group_entities[group][ent] = nil
        end
        if entity_groups[ent] then
            entity_groups[ent][group] = nil
        end
    end
end

function cs.group.has(ent)
    return true
end

function cs.group.set_groups(ent, groups)
    cs.group.remove(ent)
    cs.group.add(ent, groups)
end

function cs.group.get_groups(ent)
    local groups = {}
    if entity_groups[ent] then
        for group in pairs(entity_groups[ent]) do
            table.insert(groups, group)
        end
    end
    return table.concat(groups, ' ')
end

function cs.group.get_entities(groups)
    local ents = {}
    for group in _groups(groups) do
        if group_entities[group] then
            cg.entity_table_merge(ents, group_entities[group])
        end
    end
    return ents
end

function cs.group.destroy(groups)
    for group in _groups(groups) do
        if group_entities[group] then
            for ent in pairs(group_entities[group]) do
                cs.entity.destroy(ent)
            end
        end
        group_entities[group] = nil
    end
end

function cs.group.set_save_filter(groups, val)
    for group in _groups(groups) do
        if group_entities[group] then
            for ent in pairs(group_entities[group]) do
                cs.entity.set_save_filter(ent, val)
            end
        end
    end
end

function cs.group.update_all()
    for ent in pairs(entity_groups) do
        if cs.entity.destroyed(ent) then cs.group.remove(ent) end
    end
end

function cs.group.save_all()
    return entity_groups
end

function cs.group.load_all(d)
    for ent, groups in pairs(d) do
        cs.group.set_groups(ent, groups)
    end
end
