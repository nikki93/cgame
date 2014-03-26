local old_entity_create = cg.entity_create
function cg.entity_create()
    local e = old_entity_create()
    cs.group.add(e, 'default')
    return e
end
