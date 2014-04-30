WIN_WIDTH = 800
WIN_HEIGHT = 600

math.randomseed(os.time())

function symrand()
    return 2 * math.random() - 1
end

cs.physics.set_simulation_frequency(300)

-- add camera

camera = cg.add {
    camera = { viewport_height = 54 },
}

-- add floor

local D = 16

floor = cg.add {
    transform = { position = cg.vec2_zero, scale = cg.vec2(32, 32) },
    sprite = { texcell = cg.vec2(0, 0), texsize = cg.vec2(32, 32) },
    physics = { type = cs.PB.KINEMATIC },
    keyboard_controlled = {},
}

cs.physics.shape_add_box(floor, cg.bbox(cg.vec2(-D, -D), cg.vec2(D, -(D - 1))))
cs.physics.shape_add_box(floor, cg.bbox(cg.vec2(-D, D - 1), cg.vec2(D, D)))
cs.physics.shape_add_box(floor, cg.bbox(cg.vec2(-D, -D), cg.vec2(-(D - 1), D)))
cs.physics.shape_add_box(floor, cg.bbox(cg.vec2(D - 1, -D), cg.vec2(D, D)))

-- oscillator_set(floor, { amp = 8, freq = 0.5 })
-- rotator_set(floor, 0.1 * math.pi)

-- add some boxes

boxes = {}
function make_box(pos, vel)
    local box = cg.add {
        transform = { position = pos },
        sprite = { texcell = cg.vec2(32, 32), texsize = cg.vec2(32, 32) },
        physics = { type = cg.PB_DYNAMIC, mass = 5, velocity = vel },
    }
    cs.physics.shape_add_box(box,
                             cg.bbox(cg.vec2(-0.5, -0.5), cg.vec2(0.5, 0.5)))
    cs.group.add_groups(box, 'box')

    boxes[box] = true
end

function clear_boxes()
    for e, _ in pairs(boxes) do cs.entity.destroy(e) end
    boxes = {}
end

local ymin = -3.0
local ymax = 8.0

local y = ymin
while y < ymax do
    local xspan = 9.0 * (1.0 - (y - ymin) / (ymax - ymin))
    local x = -xspan + math.random()
    while x < xspan do
        make_box(cg.vec2(x, y), cg.vec2(symrand(), symrand()))
        x = x + 1.0 + 3.0 * math.random()
    end
    y = y + 1.2
end

-- add more boxes with space

cs.box_add = {}
function cs.box_add.update_all()
    if cs.input.key_down(cg.KC_SPACE) then
        make_box(cg.vec2(5 * symrand(), 5 * symrand()),
                 cg.vec2(5 * symrand(), 5 * symrand()))
    end
end

print('this is the physics test')
