--
-- another silly test system
--

local serpent = require 'serpent'

local tbl = {}   -- tbl[ent] contains data for ent -- right now just speed

function rotator_set(ent, speed)
    if not speed then speed = 2 * math.pi end
    tbl[ent] = speed
end

cgame.system_add
{
    name = 'rotator',

    clear = function ()
        tbl = {}
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
        return serpent.dump(tbl)
    end,

    load_all = function (str)
        tbl = loadstring(str)()
    end,
}

