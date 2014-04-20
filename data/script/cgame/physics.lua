local ffi = require 'ffi'

local old_physics_shape_add_poly = cg.physics_shape_add_poly
function cg.physics_shape_add_poly(ent, verts)
    local n = #verts
    return old_physics_shape_add_poly(ent, n, ffi.new('Vec2[?]', n, verts))
end
