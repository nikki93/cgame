require 'test.oscillator'
require 'test.rotator'

GRID_SIZE = 32.0
WIN_WIDTH = 800
WIN_HEIGHT = 600

math.randomseed(os.time())

function symrand()
    return 2 * math.random() - 1
end

-- add camera

local camera = cgame.entity_create()

cgame.transform_add(camera)
cgame.camera_add(camera)
cgame.camera_set_viewport_size(
    cgame.vec2(3 * WIN_WIDTH / GRID_SIZE, 3 * WIN_HEIGHT / GRID_SIZE))
cgame.keyboard_controlled_add(camera)

-- add floor

local D = 20

local floor = cgame.entity_create()
cgame.physics_add_static(floor)
cgame.physics_add_box_shape(floor, -D, -D, D, -(D - 1))

local floor = cgame.entity_create()
cgame.physics_add_static(floor)
cgame.physics_add_box_shape(floor, -D, D - 1, D, D)

local floor = cgame.entity_create()
cgame.physics_add_static(floor)
cgame.physics_add_box_shape(floor, -D, -D, -(D - 1), D)

local floor = cgame.entity_create()
cgame.physics_add_static(floor)
cgame.physics_add_box_shape(floor, D - 1, -D, D, D)

-- add some boxes

function make_box(pos, vel)
    local box = cgame.entity_create()

    cgame.transform_add(box)
    cgame.transform_set_position(box, pos)

    cgame.sprite_add(box)
    cgame.sprite_set_cell(box, cgame.vec2(32.0, 32.0))
    cgame.sprite_set_size(box, cgame.vec2(32.0, 32.0))

    cgame.physics_add_dynamic(box, 5.0)
    cgame.physics_add_box_shape(box, -0.5, -0.5, 0.5, 0.5)
    cgame.physics_set_velocity(box, vel)
end

local ymin = -3.0
local ymax = 8.0

local y = ymin
while y < ymax do
    local xspan = 9.0 * (1.0 - (y - ymin) / (ymax - ymin))
    local x = -xspan + math.random()
    while x < xspan do
        make_box(cgame.vec2(x, y), cgame.vec2(symrand(), symrand()))
        x = x + 1.0 + 3.0 * math.random()
    end
    y = y + 1.2
end

-- add more boxes with 'B'

cgame.add_system('box_gen',
{
    update_all = function (dt)
        if cgame.input_key_down('KC_B') then
            make_box(cgame.vec2(5 * symrand(), 5 * symrand()),
            cgame.vec2(5 * symrand(), 5 * symrand()))
        end
    end
})

-- rotate gravity with the camera for kicks

cgame.add_system('camera_gravity',
{
    update_all = function (dt)
        rot = cgame.transform_get_rotation(camera)
        cgame.physics_set_gravity(cgame.vec2_rot(cgame.vec2(0, -9.8), rot))
    end
})

