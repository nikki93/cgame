local oscillator = require 'test.oscillator'

-- add some blocks

math.randomseed(os.time())
local n_blocks = math.random(50)
for i = 0, n_blocks do
    local block = cgame.entity_new()

    local pos = cgame.vec2(math.random(25) - 12, math.random(9) - 4)

    cgame.transform_add(block)
    cgame.transform_set_position(block, pos)

    cgame.sprite_add(block)
    cgame.sprite_set_cell(block, cgame.vec2(32.0, 32.0))
    cgame.sprite_set_size(block, cgame.vec2(32.0, 32.0))

    if pos.y < 0 then
        oscillator.set(block, { freq = math.random() })
    end
end

-- add player

local player = cgame.entity_new()

cgame.transform_add(player)
cgame.transform_set_position(player, cgame.vec2(0.0, 0.0))

cgame.sprite_add(player)
cgame.sprite_set_cell(player, cgame.vec2( 0.0, 32.0))
cgame.sprite_set_size(player, cgame.vec2(32.0, 32.0))

cgame.keyboard_controlled_add(player)

