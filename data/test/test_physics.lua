require 'test.oscillator'
require 'test.rotator'

GRID_SIZE = 32.0
WIN_WIDTH = 800
WIN_HEIGHT = 600

math.randomseed(os.time())

function symrand()
    return 2 * math.random() - 1
end

cgame.physics_set_simulation_frequency(300)

-- add camera

local camera = cgame.entity_create()

cgame.transform_add(camera)
cgame.camera_add(camera)
cgame.camera_set_viewport_size(
    cgame.vec2(3 * WIN_WIDTH / GRID_SIZE, 3 * WIN_HEIGHT / GRID_SIZE))
cgame.keyboard_controlled_add(camera)

-- add floor

local D = 16

local floor = cgame.entity_create()

cgame.transform_add(floor)
cgame.transform_set_position(floor, cgame.vec2_zero)

cgame.sprite_add(floor)
cgame.sprite_set_cell(floor, cgame.vec2(0.0, 0.0))
cgame.sprite_set_size(floor, cgame.vec2(32.0, 32.0))
cgame.transform_set_scale(floor, cgame.vec2(32, 32))

cgame.physics_add(floor)
cgame.physics_set_static(floor, true)

print(cgame.physics_add_box_shape(floor, -D, -D, D, -(D - 1)))
print(cgame.physics_add_box_shape(floor, -D, D - 1, D, D))
print(cgame.physics_add_box_shape(floor, -D, -D, -(D - 1), D))
print(cgame.physics_add_box_shape(floor, D - 1, -D, D, D))

-- add some boxes

function make_box(pos, vel)
    local box = cgame.entity_create()

    cgame.transform_add(box)
    cgame.transform_set_position(box, pos)

    cgame.sprite_add(box)
    cgame.sprite_set_cell(box, cgame.vec2(32.0, 32.0))
    cgame.sprite_set_size(box, cgame.vec2(32.0, 32.0))

    cgame.physics_add(box)
    cgame.physics_set_static(box, false)
    cgame.physics_set_mass(box, 5.0)
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

-- add more boxes with 'B', apply force with 'F'

cgame.add_system('box_gen',
{
    update_all = function (dt)
        if cgame.input_key_down('KC_B') then
            make_box(cgame.vec2(5 * symrand(), 5 * symrand()),
            cgame.vec2(5 * symrand(), 5 * symrand()))
        end
    end
})

cgame.add_system('box_force',
{
    update_all = function (dt)
        if cgame.input_key_down('KC_F') then
            cgame.physics_apply_force(10, cgame.vec2(0, 20))
        else
            cgame.physics_reset_forces(10)
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

-- entity destruction

cgame.add_system('destroyer',
{
    update_all = function (dt)
        for i = 1, 9 do
            if (cgame.input_key_down('KC_' .. i)) then
                cgame.physics_set_static(i,
                    cgame.input_key_down('KC_LEFT_SHIFT'))
            end
        end
    end
})

