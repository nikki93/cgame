--
-- a silly test system
--
--     oscillator.set(ent, { amp = ..., freq = ... }) to set
--     for entity ent
--
--     oscillator.reset() to reset time
--

local tbl = {} -- stores data per oscillator entity
local time = 0

cgame.add_system('oscillator',
{
    update_all = function (dt)
        time = time + dt

        for ent, osc in pairs(tbl) do
            pos = cgame.transform_get_position(ent)
            pos.x = osc.initx
                + osc.amp * math.sin(2 * math.pi * osc.freq * time)
            cgame.transform_set_position(ent, pos)
        end
    end,
})

return
{
    set = function (ent, osc)
        -- default parameters
        if not osc.amp then osc.amp = 1 end
        if not osc.freq then osc.freq = 1 end

        osc.initx = cgame.transform_get_position(ent).x

        tbl[ent] = osc
    end,

    reset = function ()
        time = 0
    end,
}

