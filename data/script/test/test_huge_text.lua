ffi = require 'ffi'
require 'test.rotator'

math.randomseed(os.time())

N = cg.args[2] or 500

box = cg.bbox_bound(cg.vec2_zero, cg.vec2(1, -0.8) * cs.game.get_window_size())

d = cg.vec2(box.max.x - box.min.x, box.max.y - box.min.y) / math.sqrt(N)
pos = cg.vec2(box.min.x, box.min.y)
n = 0
while pos.y <= box.max.y do
    pos.x = box.min.x
    while pos.x <= box.max.x do
        cg.add {
            transform = { position = pos },
            gui_text = { str = 'hello, world' },
            gui = { color = cg.color_blue },
            --rotator = { speed = math.random() * math.pi / 2 },
        }
        n = n + 1

        pos.x = pos.x + d.x
    end
    pos.y = pos.y + d.y
end
print(n .. ' text entities')

ffi = require 'ffi'
ffi.cdef('struct P { struct Q { int x; int y; } q; };')
P = ffi.typeof('struct P')

a = ffi.new('struct P', { { 2, 3 } })
b = P(a)
print('before:')
print(a.q.x)
print(b.q.x)
a.q.x = 3
print('after:')
print(a.q.x)
print(b.q.x)
