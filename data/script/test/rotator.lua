--
-- another silly test system
--

cs.rotator = { auto_saveload = true }

cs.rotator.tbl = cg.entity_table()

-- properties

cg.simple_props(cs.rotator, cs.rotator.tbl, {
    speed = 2 * math.pi
})


-- add/remove

function cs.rotator.add(ent, speed)
    if cs.rotator.tbl[ent] then return end
    cs.rotator.tbl[ent] = { speed = speed or 2 * math.pi }
end
function cs.rotator.remove(ent)
    cs.rotator.tbl[ent] = nil
end


-- update

function cs.rotator.update_all()
    cg.entity_table_remove_destroyed(cs.rotator.tbl, cs.rotator.remove)

    for ent, rotator in pairs(cs.rotator.tbl) do
        cs.transform.rotate(ent, rotator.speed * cs.timing.dt)
    end
end
