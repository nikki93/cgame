--
-- a silly test system
--
--     oscillator_set(ent, { amp = ..., freq = ... }) to set
--     for entity ent
--
--     oscillator_reset_all() to reset time
--

cs.oscillator = { auto_saveload = true }

cs.oscillator.tbl = cg.entity_table()

function cs.oscillator.add(ent, osc)
    -- default parameters
    if not osc.phase then osc.phase = 0 end
    if not osc.amp then osc.amp = 1 end
    if not osc.freq then osc.freq = 1 end

    -- initial state
    osc.initx = cg.transform_get_position(ent).x
    osc.t = 0

    cs.oscillator.tbl[ent] = osc
end

function cs.oscillator.update_all()
    for ent, _ in pairs(cs.oscillator.tbl) do
        if cs.entity.destroyed(ent) then cs.oscillator.tbl[ent] = nil end
    end

    for ent, osc in pairs(cs.oscillator.tbl) do
        local pos = cs.transform.get_position(ent)
        pos.x = osc.initx
            + osc.amp * math.sin(2 * math.pi
                                     * (osc.phase + osc.freq * osc.t))
        cs.transform.set_position(ent, pos)
        osc.t = osc.t + cs.timing.dt
    end
end
