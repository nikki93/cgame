--
-- another silly test system
--

local tbl = cgame.entity_table()

function rotator_set(ent, speed)
    if not speed then speed = 2 * math.pi end
    tbl[ent] = speed
end

cgame.system_add
{
    name = 'rotator',

    clear = function ()
        tbl = cgame.entity_table()
    end,

    update_all = function ()
        for ent, _ in pairs(tbl) do
            if cgame.entity_destroyed(ent) then tbl[ent] = nil end
        end

        for ent, speed in pairs(tbl) do
            cgame.transform_rotate(ent, speed * cgame.timing_dt)
        end
    end,

    save_all = function ()
        return tbl
    end,

    load_all = function (d)
        cgame.entity_table_merge(tbl, d)
    end,
}

