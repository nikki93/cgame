--
-- another silly test system
--

cs.rotator = { auto_saveload = true }

cs.rotator.tbl = cg.entity_table()

function cs.rotator.add(ent, speed)
    cs.rotator.tbl[ent] = speed or 2 * math.pi
end

function cs.rotator.set_speed(ent, speed)
    cs.rotator.tbl[ent] = speed or 2 * math.pi
end
function cs.rotator.get_speed(ent)
    return cs.rotator.tbl[ent].speed
end

function cs.rotator.update_all()
    for ent, _ in pairs(cs.rotator.tbl) do
        if cs.entity.destroyed(ent) then
            cs.rotator.tbl[ent] = nil
        end
    end

    for ent, speed in pairs(cs.rotator.tbl) do
        cs.transform.rotate(ent, speed * cs.timing.dt)
    end
end
