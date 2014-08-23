--- utils -------------------------------------------------------------------

function cg.square_random(d)
    return cg.vec2(math.random() * d, math.random() * d)
end

function file_exists(s)
    local f = io.open(s, 'r')
    if f ~= nil then io.close(f) return true else return false end
end


--- cgame config ------------------------------------------------------------

data_dir = './ld30'

cs.sprite.set_atlas(data_dir .. '/atlas-1.png')

cs.edit.set_default_file(data_dir .. '/')
cs.edit.set_default_prefab_file(data_dir .. '/')


-----------------------------------------------------------------------------

function save_game(name, groups)
    cs.group.set_save_filter(groups, true)
    local s = cg.store_open()
    cs.system.save_all(s)
    cg.store_write_file(s, data_dir .. '/' .. name)
    cg.store_close(s)
end

function load_game(name)
    if not file_exists(data_dir .. '/' .. name) then return false end
    local s = cg.store_open_file(data_dir .. '/' .. name)
    cs.system.load_all(s)
    cg.store_close(s)
    return true
end

cs.edit.modes.normal['s'] = function ()
    cs.edit.command_save()

    save_game('portals.lvl', 'portals')
    print('saved portals!')
end


-----------------------------------------------------------------------------

cs.main = {}

cs.main.world = 'hell'

function cs.main.reload()
    cs.group.destroy('world')
    local s = cg.store_open_file(data_dir .. '/hell.lvl')
    cs.system.load_all(s)
    cg.store_close(s)
end

function cs.main.die()
    local player = cs.name.find('player')
    if player then cs.player_control.die(player) end
end

function cs.main.warp(world)
    -- save and clear current state
    save_game(cs.main.world .. '.sav', 'default')
    cs.groups.destroy('default')

    -- set, load new world
    cs.main.world = world
    if not load_game(world .. '.sav') then load_game(world .. '.lvl') end
end


-----------------------------------------------------------------------------

cs.player_dead = cg.simple_sys()

cg.simple_prop(cs.player_dead, 'time', 0.7)

function cs.player_dead.create(obj)
    cs.transform.set_rotation(obj.ent, math.random(0, 3) * 0.5 * math.pi)
end

function cs.player_dead.unpaused_update(obj)
    obj.time = obj.time - cs.timing.dt

    if obj.time <= 0 then
        cs.main.reload()
    end
end


-----------------------------------------------------------------------------

cs.player_control = cg.simple_sys()

g_player_speed = 7

function cs.player_control.die(ent)
    local obj = cs.player_control.tbl[ent]
    assert(obj)
    local p = cs.transform.get_position(obj.ent)
    cg.entity_destroy(ent)
    
    cg.add {
        transform = {
            position = p,
            scale = cg.vec2(2.2, 2.2),
        },
        sprite = {
            texcell = cg.vec2(64, 16),
            texsize = cg.vec2(32, 32),
        },
        player_dead = {}
    }
end

function cs.player_control.unpaused_update(obj)
    local d = cg.vec2(0, 0)
    local rot, pressed

    if cs.input.key_down(cg.KC_W) then
        rot = - 0.5 * math.pi
        pressed = true
        d = d + cg.vec2( 0,  1)
    end
    if cs.input.key_down(cg.KC_S) then
        rot = 0.5 * math.pi
        pressed = true
        d = d + cg.vec2( 0, -1)
    end
    if cs.input.key_down(cg.KC_D) then
        rot = math.pi
        pressed = true
        d = d + cg.vec2( 1,  0)
    end
    if cs.input.key_down(cg.KC_A) then
        rot = 0
        pressed = true
        d = d + cg.vec2(-1,  0)
    end
    d = cg.vec2_normalize(d)

    if pressed then 
        cs.bump.slide(obj.ent, g_player_speed * d * cs.timing.dt,
                      function (e) return not cs.portal.has(e) end)
    end

    if rot then cs.transform.set_rotation(obj.ent, rot) end
end


for e in pairs(cs.edit.select) do
    cs.sprite.set_depth(e, 10)
end



-----------------------------------------------------------------------------

cs.wall = cg.simple_sys()

function cs.wall.create(obj)
    cs.bump.add(obj.ent)
end




-----------------------------------------------------------------------------

cs.follower = cg.simple_sys()

g_follower_speed = 4

function cs.follower.create(obj)
    cs.bump.add(obj.ent)
end

function cs.follower.unpaused_update(obj)
    if not obj.t then obj.t = 0 end

    local player = cs.name.find('player')
    if player ~= cg.entity_nil then
        local pp = cs.transform.get_position(player)
        local d = pp - cs.transform.get_position(obj.ent)
        local len = cg.vec2_len(d)
        if len < 20 then
            d = d / len
            local cols = cs.bump.slide(obj.ent, g_follower_speed * d * cs.timing.dt)
            for _, col in pairs(cols) do
                if cs.player_control.has(col.other) then
                    cs.main.die()
                end
            end
        end
    end
end


-----------------------------------------------------------------------------

cs.quad_shooter = cg.simple_sys()

cg.simple_prop(cs.quad_shooter, 'next', 1)
cg.simple_prop(cs.quad_shooter, 'period', 1)


function cs.quad_shooter.unpaused_update(obj)
    obj.next = obj.next - cs.timing.dt

    if not obj.shooting and obj.next <= 0.28 then
        cs.animation.start(obj.ent, 'shoot')
        obj.shooting = true
    end

    if obj.next <= 0 then
        obj.shooting = false
        obj.next = obj.period

        local dirs = { cg.vec2(1, 0), cg.vec2(-1, 0),
                       cg.vec2(0, 1), cg.vec2(0, -1) }
        local p = cs.transform.get_position(obj.ent)
        local r = cs.transform.get_rotation(obj.ent)

        for _, dir in ipairs(dirs) do
            dir = cg.vec2_rot(dir, r)
            cg.add {
                prefab = data_dir .. '/hell-bullet-1.pfb',
                transform = { position = p + dir },
                bullet = {
                    creator = cg.Entity(obj.ent),
                    velocity = 8 * dir,
                }
            }
        end
    end
end


-----------------------------------------------------------------------------

cs.bullet = cg.simple_sys()

cg.simple_prop(cs.bullet, 'creator', cg.Entity(cg.entity_nil))
cg.simple_prop(cs.bullet, 'velocity', cg.vec2(7, 0))
cg.simple_prop(cs.bullet, 'time', 10)

function cs.bullet.unpaused_update(obj)
    obj.time = obj.time - cs.timing.dt
    if obj.time <= 0 then
        cs.entity.destroy(obj.ent)
        return
    end

    cs.transform.set_rotation(obj.ent, cg.vec2_atan2(obj.velocity))

    local hit = false
    local cols = cs.bump.slide(obj.ent, obj.velocity * cs.timing.dt,
                               function (e) return e ~= obj.creator end)
    for _, col in ipairs(cols) do
        hit = true
        if cs.player_control.has(col.other) then
            cs.main.die()
        end
    end
    if hit then cs.entity.destroy(obj.ent) end
end


-----------------------------------------------------------------------------

cs.camera_follow = cg.simple_sys()

function cs.camera_follow.update_all()
    local cam = cs.camera.get_current_camera()
    if cam ~= cg.entity_nil and cam ~= cs.edit.camera
    and not cs.camera_follow.has(cam) then
        cs.camera_follow.add(cam)
        cs.group.set_groups(cam, 'warp')
    end

    cs.camera_follow.simple_update_all()
end

function cs.camera_follow.unpaused_update(obj)
    local player = cs.name.find('player')
    if player ~= cg.entity_nil then
        local ppw = cs.transform.local_to_world(player, cg.vec2_zero)
        local d = ppw - cs.transform.get_position(obj.ent)
        cs.transform.translate(obj.ent, 4 * d * cs.timing.dt)
    end
end


-----------------------------------------------------------------------------

cs.portal = cg.simple_sys()

cg.simple_prop(cs.portal, 'world_1', 'hell')
cg.simple_prop(cs.portal, 'world_2', 'water')

function cs.portal.create(obj)
end

function cs.portal.unpaused_update(obj)
    local cols = cs.bump.sweep(obj.ent)
    if obj.in_warp == nil then obj.in_warp = false end

    local next_in_warp = false
    for _, col in ipairs(cols) do
        if cs.name.get_name(col.other) == 'player' then
            next_in_warp = true
            if not obj.in_warp then
                local ow = (obj.world_1 == cs.main.world)
                    and obj.world_2 or obj.world_1
                cs.main.warp(ow)
            end
        end
    end
    obj.in_warp = next_in_warp
end
