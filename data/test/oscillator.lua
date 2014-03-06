--
-- a silly test system
--
--     oscillator_set(ent, { amp = ..., freq = ... }) to set
--     for entity ent
--
--     oscillator_reset_all() to reset time
--

local tbl = cgame.entity_table()

function oscillator_set(ent, osc)
    -- default parameters
    if not osc.phase then osc.phase = 0 end
    if not osc.amp then osc.amp = 1 end
    if not osc.freq then osc.freq = 1 end

    osc.initx = cgame.transform_get_position(ent).x
    osc.t = 0

    tbl[ent] = osc
end

function oscillator_reset_all()
    time = 0
end

cgame.system_add
{
    name = 'oscillator',

    clear = function ()
        tbl = cgame.entity_table()
    end,

    update_all = function ()
        for ent, _ in pairs(tbl) do
            if cgame.entity_destroyed(ent) then tbl[ent] = nil end
        end

        for ent, osc in pairs(tbl) do
            pos = cgame.transform_get_position(ent)
            pos.x = osc.initx
                + osc.amp * math.sin(2 * math.pi
                                         * (osc.phase + osc.freq * osc.t))
            cgame.transform_set_position(ent, pos)
            osc.t = osc.t + cgame.timing_dt
        end
    end,

    save_all = function ()
        return tbl
    end,
    load_all = function (d)
        cgame.entity_table_merge(tbl, d)
    end,
}

