require 'test.oscillator'
require 'test.rotator'

GRID_SIZE = 32.0
WIN_WIDTH = 800
WIN_HEIGHT = 600

-- add camera

camera = cgame.entity_create()

cgame.transform_add(camera)
cgame.camera_add(camera)
cgame.camera_set_viewport_size(
    camera,
    cgame.vec2(WIN_WIDTH / GRID_SIZE, WIN_HEIGHT / GRID_SIZE)
)

-- add some blocks

math.randomseed(os.time())

function symrand()
    return 2 * math.random() - 1
end

local n_blocks = 20
for i = 0, n_blocks do
    local block = cgame.entity_create()

    local pos = cgame.vec2(i - 8, i - 8)

    cgame.transform_add(block)
    cgame.transform_set_position(block, pos)

    cgame.sprite_add(block)
    if i / 2 == math.floor(i / 2) then
        cgame.sprite_set_cell(block, cgame.vec2( 0.0, 32.0))
    else
        cgame.sprite_set_cell(block, cgame.vec2(32.0, 32.0))
    end
    cgame.sprite_set_size(block, cgame.vec2(32.0, 32.0))

    oscillator_set(block, { amp = 1, freq = 1, phase = 5 * i / n_blocks })
    rotator_set(block, 2 * math.pi)
end

-- add player

player = cgame.add
{
    transform = {
        position = cgame.vec2(0, 0),
        scale = cgame.vec2(2, 2),
        rotation = math.pi / 16,
    },
    sprite = { cell = cgame.vec2(0, 32), size = cgame.vec2(32, 32) },
}

rchild = cgame.add
{
    transform = {
        parent = player,
        position = cgame.vec2(1, 0),
        scale = cgame.vec2(0.5, 0.5),
    },
    sprite = { cell = cgame.vec2(32, 32), size = cgame.vec2(32, 32) },
}

lchild = cgame.add
{
    transform = {
        parent = player,
        position = cgame.vec2(-1, 0),
        scale = cgame.vec2(0.5, 0.5),
    },
    sprite = { cell = cgame.vec2(32, 32), size = cgame.vec2(32, 32) },
}

-- who gets keyboard?

--cgame.keyboard_controlled_add(camera)
cgame.keyboard_controlled_add(player)

-- entity destruction

cgame.system_add
{
    name = 'destroyer',

    update_all = function ()
        for i = 1, 9 do
            if cgame.input_key_down('KC_' .. i) then
                cgame.entity_destroy(i)
            end
        end

        if cgame.input_mouse_down(cgame.MC_LEFT) then
            p = cgame.input_get_mouse_pos_unit()
            p = cgame.transform_local_to_world(camera, p)
            print(p.x .. ', ' .. p.y)
        end
    end
}
