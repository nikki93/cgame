require 'test.oscillator'
require 'test.rotator'

-- add camera: 32 pixels is one unit

camera = cs.entity.create()

cs.transform.add(camera)
cs.camera.add(camera)
cs.camera.set_viewport_height(camera, 18)

-- add some blocks

local n_blocks = 20
for i = 0, n_blocks do
    local block = cg.add
    {
        transform = { position = cg.vec2(i - 8, i - 8) },
        sprite = {
            texcell = cg.vec2(i / 2 == math.floor(i / 2) and 0 or 32, 32),
            texsize = cg.vec2(32, 32)
        },
    }

    cs.oscillator.add(block, { amp = 1, freq = 1, phase = 5 * i / n_blocks })
    cs.rotator.add(block, 2 * math.pi)
end

-- add player

player = cg.add
{
    transform = {
        position = cg.vec2(0, 0),
        scale = cg.vec2(2, 2),
        rotation = math.pi / 16,
    },
    sprite = { texcell = cg.vec2(0, 32), texsize = cg.vec2(32, 32) },
}

rchild = cg.add
{
    transform = {
        parent = player,
        position = cg.vec2(1, 0),
        scale = cg.vec2(0.5, 0.5),
    },
    sprite = { texcell = cg.vec2(32, 32), texsize = cg.vec2(32, 32) },
}

lchild = cg.add
{
    transform = {
        parent = player,
        position = cg.vec2(-1, 0),
        scale = cg.vec2(0.5, 0.5),
    },
    sprite = { texcell = cg.vec2(32, 32), texsize = cg.vec2(32, 32) },
}

-- who gets keyboard?

--cs.keyboard.controlled_add(camera)
cs.keyboard.controlled_add(player)

-- entity destruction
