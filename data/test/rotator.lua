--
-- another silly test system
--

cs.rotator = {}

cs.rotator.tbl = cg.entity_table()

function cs.rotator.add(ent, speed)
    cs.rotator.tbl[ent] = speed or 2 * math.pi
end

function cs.rotator.clear()
    cs.rotator.tbl = cg.entity_table()
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

function cs.rotator.save_all()
    return cs.rotator.tbl
end

function cs.rotator.load_all(d)
    -- merge the loaded data in
    cg.entity_table_merge(cs.rotator.tbl, d)
end
