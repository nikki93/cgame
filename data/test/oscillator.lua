--
-- a silly test system
--
--     oscillator_set(ent, { amp = ..., freq = ... }) to set
--     for entity ent
--
--     oscillator_reset_all() to reset time
--

local serpent = require 'serpent'

local tbl = {} -- stores data per oscillator entity
local time = 0

function oscillator_set(ent, osc)
    -- default parameters
    if not osc.phase then osc.phase = 0 end
    if not osc.amp then osc.amp = 1 end
    if not osc.freq then osc.freq = 1 end

    osc.initx = cgame.transform_get_position(ent).x

    tbl[ent] = osc
end

function oscillator_reset_all()
    time = 0
end

cgame.add_system
{
    name = 'oscillator',

    clear = function ()
        tbl = {}
    end,

    update_all = function ()
        for ent, _ in pairs(tbl) do
            if cgame.entity_destroyed(ent) then tbl[ent] = nil end
        end

        time = time + cgame.timing_dt

        for ent, osc in pairs(tbl) do
            pos = cgame.transform_get_position(ent)
            pos.x = osc.initx
                + osc.amp * math.sin(2 * math.pi
                                         * (osc.phase + osc.freq * time))
            cgame.transform_set_position(ent, pos)
        end
    end,

    save_all = function ()
        local dump = { tbl = tbl, time = time }
        return serpent.dump(dump)
    end,

    load_all = function (str)
        local dump = loadstring(str)()
        tbl = dump.tbl
        time = dump.time
    end,
}

