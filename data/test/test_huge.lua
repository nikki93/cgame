require 'test.oscillator'
require 'test.rotator'

GRID_SIZE = 32.0
WIN_WIDTH = 800
WIN_HEIGHT = 600

-- add camera

local camera = cgame.entity_create()

cgame.transform_add(camera)
cgame.camera_add(camera)
cgame.camera_set_viewport_size(
    cgame.vec2(WIN_WIDTH / GRID_SIZE, WIN_HEIGHT / GRID_SIZE))

-- add some blocks

math.randomseed(os.time())

function symrand()
    return 2 * math.random() - 1
end

local n_blocks = cgame_args[2] or 30000
print('creating ' .. n_blocks .. ' blocks')
for i = 0, n_blocks do
    local block = cgame.entity_create()

    local y = 8 * symrand()
    while math.abs(y) < 1.5 do
        y = 8 * symrand()
    end
    local pos = cgame.vec2(8 * symrand(), y)

    cgame.transform_add(block)
    cgame.transform_set_position(block, pos)

    cgame.sprite_add(block)
    if symrand() < 0 then
        cgame.sprite_set_cell(block, cgame.vec2( 0.0, 32.0))
    else
        cgame.sprite_set_cell(block, cgame.vec2(32.0, 32.0))
    end
    cgame.sprite_set_size(block, cgame.vec2(32.0, 32.0))

    oscillator_set(block, { amp = 3 * math.random(), freq = math.random() })
    rotator_set(block, math.random() * math.pi)
end

-- add player

local player = cgame.entity_create()

cgame.transform_add(player)
cgame.transform_set_position(player, cgame.vec2(0.0, 0.0))

cgame.sprite_add(player)
cgame.sprite_set_cell(player, cgame.vec2( 0.0, 32.0))
cgame.sprite_set_size(player, cgame.vec2(32.0, 32.0))

cgame.transform_set_scale(player, cgame.vec2(2, 2))

-- who gets keyboard?

cgame.keyboard_controlled_add(camera)
--cgame.keyboard_controlled_add(player)

