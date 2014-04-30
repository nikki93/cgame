require 'test.oscillator'
require 'test.rotator'

-- add camera: 32 pixels is one unit

camera = cs.entity.create()
cs.transform.add(camera)
cs.camera.add(camera)
cs.camera.set_viewport_height(camera, 18)

-- add some blocks

math.randomseed(os.time())

function symrand()
    return 2 * math.random() - 1
end

local n_blocks = cg.args[2] or 30000
print('creating ' .. n_blocks .. ' blocks')
for i = 0, n_blocks do
    local block = cs.entity.create()

    local y = 8 * symrand()
    while math.abs(y) < 1.5 do
        y = 8 * symrand()
    end
    local pos = cg.vec2(8 * symrand(), y)

    cs.transform.add(block)
    cs.transform.set_position(block, pos)

    cs.sprite.add(block)
    if symrand() < 0 then
        cs.sprite.set_texcell(block, cg.vec2( 0.0, 32.0))
        cs.edit.select[block] = true
    else
        cs.sprite.set_texcell(block, cg.vec2(32.0, 32.0))
    end
    cs.sprite.set_texsize(block, cg.vec2(32.0, 32.0))

    cs.oscillator.add(block, { amp = 3 * math.random(), freq = math.random() })
    cs.rotator.add(block, math.random() * math.pi)
end

-- add player

player = cs.entity.create()

cs.transform.add(player)
cs.transform.set_position(player, cg.vec2(0.0, 0.0))

cs.sprite.add(player)
cs.sprite.set_texcell(player, cg.vec2( 0.0, 32.0))
cs.sprite.set_texsize(player, cg.vec2(32.0, 32.0))

cs.transform.set_scale(player, cg.vec2(2, 2))

-- who gets keyboard?

cs.keyboard.controlled_add(camera)
--cs.keyboard.controlled_add(player)

