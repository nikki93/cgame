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

    -- move filtering
    local p = cs.transform.get_position(obj.ent)
    local function filter(e)
        if cs.oneway.has(e) then
            local ep = cs.transform.get_position(e)
            if p.y < ep.y + 0.99 then return false end
        end
        return true
    end

    -- move! (first y then x)
    local d = obj.v * cs.timing.dt
    local cols = cs.bump.slide(obj.ent, cs.transform.get_position(obj.ent) + d,
                               filter)

    -- collisions
    for _, col in ipairs(cols) do
        if col.normal.y ~= 0 then obj.v.y = 0 end
        if col.normal.y > 0 then obj.grounded = true end
    end
end


--- oneway ------------------------------------------------------------------

cs.oneway = cg.simple_sys()

function cs.oneway.create(obj)
    cs.bump.add(obj.ent)
end


--- main --------------------------------------------------------------------

-- load level if specified
if cg.args[2] then
    local s = cg.store_open_file(cg.args[2])
    cs.system.load_all(s)
    cg.store_close(s)
end
