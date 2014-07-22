local ffi = require 'ffi'

cg.Collision = ffi.metatype('Collision', {})

local old_physics_shape_add_box = cg.physics_shape_add_box
function cg.physics_shape_add_box(ent, b, r)
    r = r or 0
    return old_physics_shape_add_box(ent, b, r)
end

local old_physics_shape_add_poly = cg.physics_shape_add_poly
function cg.physics_shape_add_poly(ent, verts, r)
    r = r or 0
    local n = #verts
    return old_physics_shape_add_poly(ent, n, ffi.new('Vec2[?]', n, verts), r)
end

local old_physics_convex_hull = cg.physics_convex_hull
function cg.physics_convex_hull(verts)
    local n = #verts
    local c_arr = ffi.new('Vec2[?]', n, verts)
    n = old_physics_convex_hull(n, c_arr)

    local lua_arr = {}
    for i = 0, n - 1 do
        table.insert(lua_arr, cg.Vec2(c_arr[i]))
    end
    return lua_arr
end

local old_physics_get_collisions = cg.physics_get_collisions
function cg.physics_get_collisions(ent)
    local n = cg.physics_get_num_collisions(ent)
    local c_arr = old_physics_get_collisions(ent)
    local lua_arr = {}
    for i = 0, n - 1 do
        table.insert(lua_arr, cg.Collision(c_arr[i]))
    end
    return lua_arr
end
