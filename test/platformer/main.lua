--- player ------------------------------------------------------------------

cs.player = cg.simple_sys()

cg.simple_prop(cs.player, 'speed', 20)
cg.simple_prop(cs.player, 'gravity', 85)
cg.simple_prop(cs.player, 'jump_speed', 25)

function cs.player.create(obj)
    cs.bump.add(obj.ent)
end

function cs.player.key_down(k)
    for e, obj in pairs(cs.player.tbl) do
        if k == cg.KC_UP and obj.grounded then
            obj.v.y = obj.v.y + obj.jump_speed
        end
    end
end

function cs.player.move(ent, d)
    local p = cs.transform.get_position(ent)
    local function filter(e)
        if cs.oneway.has(e) then
            local ep = cs.transform.get_position(e)
            if p.y < ep.y + 1 then return false end
        end
        return true
    end

    return cs.bump.slide(ent, d, filter)
end

function cs.player.unpaused_update(obj)
    obj.grounded = false
    if not obj.v then obj.v = cg.vec2(0, 0) end

    -- respawn
    if cs.transform.get_position(obj.ent).y < -11 then
        local start = cs.name.find('start')
        cs.transform.set_position(obj.ent, start ~= cg.entity_nil
                                      and cs.transform.get_position(start)
                                      or cg.vec2_zero)
        obj.v = cg.vec2(0, 0)
    end

    -- left/right
    obj.v.x = 0
    if cs.input.key_down( cg.KC_LEFT) then obj.v.x = obj.v.x - obj.speed end
    if cs.input.key_down(cg.KC_RIGHT) then obj.v.x = obj.v.x + obj.speed end

    -- gravity
    obj.v.y = obj.v.y - obj.gravity * cs.timing.dt

    -- move!
    local cols = cs.player.move(obj.ent, obj.v * cs.timing.dt)

    -- collisions
    for _, col in ipairs(cols) do
        if col.normal.y ~= 0 then obj.v.y = 0 end
        if col.normal.y > 0 then obj.grounded = true end
    end
end


--- mover -------------------------------------------------------------------

cs.mover = cg.simple_sys()

cg.simple_prop(cs.mover, 'velocity', cg.vec2(2, 0))

function cs.mover.create(obj)
    cs.bump.add(obj.ent)
end

function cs.mover.unpaused_update(obj)
    local old_pos = cs.transform.get_position(obj.ent)

    -- move
    local d = obj.velocity * cs.timing.dt
    local cols = cs.bump.slide(obj.ent, d)

    -- bounce?
    for _, col in ipairs(cols) do
        if col.normal.x ~= 0 then
            obj.velocity = cg.vec2_neg(obj.velocity)
            break
        end
    end

    -- move player?
    local reald = cs.transform.get_position(obj.ent) - old_pos
    for _, col in ipairs(cs.bump.sweep(obj.ent, cg.vec2(0, 0.01))) do
        if cs.player.has(col.other) then
            cs.player.move(col.other, reald)
        end
    end
end


--- oneway ------------------------------------------------------------------

cs.oneway = cg.simple_sys()

function cs.oneway.create(obj)
    cs.bump.add(obj.ent)
end


--- main --------------------------------------------------------------------

-- directories
game_dir = './test/platformer'
cs.edit.set_default_file(game_dir .. '/')
cs.edit.set_default_prefab_file(game_dir .. '/')

-- start at level 1 or command line option
local level_path = cg.args[2] or (game_dir .. '/1.lvl')
local s = cg.store_open_file(level_path)
cs.system.load_all(s)
cg.store_close(s)
