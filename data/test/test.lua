require 'test.oscillator'
require 'test.rotator'

GRID_SIZE = 32.0
WIN_WIDTH = 800
WIN_HEIGHT = 600

-- add camera

local camera = cgame.entity_new()

cgame.transform_add(camera)
cgame.camera_add(camera)
cgame.camera_set_viewport_size(
    cgame.vec2(WIN_WIDTH / GRID_SIZE, WIN_HEIGHT / GRID_SIZE))

cgame.keyboard_controlled_add(camera)

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
        oscillator_set(block, { freq = math.random() })
    end
    if pos.x < 0 then
        rotator_set(block, math.pi)
    end
end

-- add player

local player = cgame.entity_new()

cgame.transform_add(player)
cgame.transform_set_position(player, cgame.vec2(0.0, 0.0))

cgame.sprite_add(player)
cgame.sprite_set_cell(player, cgame.vec2( 0.0, 32.0))
cgame.sprite_set_size(player, cgame.vec2(32.0, 32.0))

-- cgame.keyboard_controlled_add(player)

-- rotator_set(player, -math.pi)
cgame.transform_set_scale(player, cgame.vec2(2, 2))

