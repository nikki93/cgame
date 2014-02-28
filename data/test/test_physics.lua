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

camera = cgame.entity_create()

cgame.transform_add(camera)
cgame.camera_add(camera)
cgame.camera_set_viewport_size(
    camera,
    cgame.vec2(3 * WIN_WIDTH / GRID_SIZE, 3 * WIN_HEIGHT / GRID_SIZE)
)

-- add floor

local D = 16

floor = cgame.entity_create()

cgame.transform_add(floor)
cgame.transform_set_position(floor, cgame.vec2_zero)

cgame.sprite_add(floor)
cgame.sprite_set_cell(floor, cgame.vec2(0.0, 0.0))
cgame.sprite_set_size(floor, cgame.vec2(32.0, 32.0))
cgame.transform_set_scale(floor, cgame.vec2(32, 32))

cgame.physics_add(floor)
cgame.physics_set_type(floor, cgame.PB_KINEMATIC)

print(cgame.physics_add_box_shape(floor, -D, -D, D, -(D - 1)))
print(cgame.physics_add_box_shape(floor, -D, D - 1, D, D))
print(cgame.physics_add_box_shape(floor, -D, -D, -(D - 1), D))
print(cgame.physics_add_box_shape(floor, D - 1, -D, D, D))

cgame.keyboard_controlled_add(floor)
-- oscillator_set(floor, { amp = 8, freq = 0.5 })
-- rotator_set(floor, 0.1 * math.pi)

-- add some boxes

function make_box(pos, vel)
    local box = cgame.entity_create()

    cgame.transform_add(box)
    cgame.transform_set_position(box, pos)

    cgame.sprite_add(box)
    cgame.sprite_set_cell(box, cgame.vec2(32.0, 32.0))
    cgame.sprite_set_size(box, cgame.vec2(32.0, 32.0))

    cgame.physics_add(box)
    cgame.physics_set_type(box, cgame.PB_DYNAMIC)
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

-- add more boxes with 'B'

cgame.system_add
{
    update_all = function ()
        if cgame.input_key_down('KC_B') then
            make_box(cgame.vec2(5 * symrand(), 5 * symrand()),
            cgame.vec2(5 * symrand(), 5 * symrand()))
        end
    end
}

-- keys

cgame.system_add
{
    update_all = function ()
        -- press a number to make it dynamic, hold right shift for static,
        -- left shift for kinematic
        for i = 1, 9 do
            if cgame.input_key_down('KC_' .. i) then
                if cgame.input_key_down(cgame.KC_LEFT_SHIFT) then
                    cgame.physics_set_type(i, cgame.PB_KINEMATIC)
                elseif cgame.input_key_down(cgame.KC_RIGHT_SHIFT) then
                    cgame.physics_set_type(i, cgame.PB_STATIC)
                else
                    cgame.physics_set_type(i, cgame.PB_DYNAMIC)
                end
            end
        end

        -- pause/resume
        if cgame.input_key_down(cgame.KC_U) then
            cgame.timing_set_paused(true)
        elseif cgame.input_key_down(cgame.KC_I) then
            cgame.timing_set_paused(false)
        end
    end
}

cgame.system_add
{
    update_all = function ()
        if cgame.input_mouse_down(cgame.MC_LEFT) then
            p = cgame.input_get_mouse_pos_unit()
            p = cgame.transform_local_to_world(camera, p)

            r = cgame.physics_nearest(p, 1)
            if r.ent == cgame.entity_nil then
                print('nothing!')
            elseif r.ent ~= floor and r.d <= 0 then
                cgame.entity_destroy(r.ent)
            end
        end
    end
}


cgame.console_puts('this is the physics test')

