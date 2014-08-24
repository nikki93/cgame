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
usr_dir = './usr'
prefab_dir = data_dir .. '/prefabs'
sound_dir = data_dir .. '/sounds'

cs.sprite.set_atlas(data_dir .. '/atlas-1.png')

cs.edit.set_default_file(data_dir .. '/')
cs.edit.set_default_prefab_file(prefab_dir .. '/')


-----------------------------------------------------------------------------

function save_game(name, groups, lvl)
    local dir = lvl and data_dir or usr_dir
    cs.group.set_save_filter(groups, true)
    local s = cg.store_open()
    cs.system.save_all(s)

    local str
    if name then
        cg.store_write_file(s, dir .. '/' .. name)
    else
        str = cg.string(cg.store_write_str(s))
    end
    cg.store_close(s)
    return str
end

function load_game(name, lvl, str)
    if str then
        local s = cg.store_open_str(name)
        cs.system.load_all(s)
        cg.store_close(s)
        return true
    end

    local dir = lvl and data_dir or usr_dir
    if not file_exists(dir .. '/' .. name) then return false end
    local s = cg.store_open_file(dir .. '/' .. name)
    cs.system.load_all(s)
    cg.store_close(s)
    return true
end

cs.edit.modes.normal['s'] = function ()
    cs.edit.command_save(function () g_save_world = true end)

    cs.group.set_save_filter('portals', true)
    local s = cg.store_open()
    cs.system.save_all(s)
    cg.store_write_file(s, data_dir .. '/portals.lvl')
    cg.store_close(s)
    print('saved portals!')
end


-----------------------------------------------------------------------------

function play_sound(file)
    cg.add {
        sound = {
            path = sound_dir .. '/' .. file,
            playing = true
        }
    }
end


-----------------------------------------------------------------------------

cs.main = {}

cs.main.intermediates = {}

function cs.main.reload()
    cs.group.destroy('portals warp default')
    local s = cg.store_open_file(usr_dir .. '/curr.sav')
    cs.system.load_all(s)
    cg.store_close(s)
end

function cs.main.die(...)
    local player = cs.name.find('player')
    if player then cs.player_control.die(player, unpack({...})) end
    play_sound('death.wav')

end

function cs.main.warp(world)
    print('warping to ' .. world)
    -- save and clear current state
    cs.main.intermediates[cs.main.world] = save_game(nil, 'default')
    cs.group.destroy('default')

    -- set, load new world
    cs.main.world = world
    if cs.main.intermediates[world] then
        load_game(cs.main.intermediates[world], false, true)
    else
        load_game(world .. '.lvl', true)
    end
    print('now in ' .. cs.main.world .. ', saving')

    -- save progress
    g_save_world = true
    save_game('curr.sav', 'portals warp default')

    -- play sound
    play_sound('portal.wav')
end

function cs.main.key_down(k)
    if k == cg.KC_ESCAPE then
        main_menu()
    end
end

g_world_colors = {
    ['hell-1'] = cg.color_opaque(198 / 255.0, 77 / 255.0, 77 / 255.0),
    ['hell-2'] = cg.color_opaque(198 / 255.0, 77 / 255.0, 77 / 255.0),
    earth = cg.color_opaque(231 / 255.0, 163 / 255.0, 93 / 255.0),
}

function cs.main.update_all()
    cs.game.set_bg_color(g_world_colors[cs.main.world]
                             and g_world_colors[cs.main.world]
                             or cg.color_white)
end

g_save_world = false
function cs.main.save_all()
    t = {}
    if g_save_world then
        t.saved_world = cs.main.world
        t.intermediates = cs.main.intermediates
    end
    g_save_world = false
    return t
end
function cs.main.load_all(t)
    if t.saved_world then cs.main.world = t.saved_world end
    if t.intermediates then cs.main.intermediates = t.intermediates end
end


-----------------------------------------------------------------------------

cs.player_dead = cg.simple_sys()

cg.simple_prop(cs.player_dead, 'time', 0.7)
cg.simple_prop(cs.player_dead, 'falling', false)

function cs.player_dead.set_falling(ent, falling)
    local obj = cs.player_dead.tbl[ent]
    assert(obj, 'must be in player_dead')

    obj.falling = falling
    if falling then
        cs.transform.set_scale(obj.ent, cg.vec2(1.5, 1.5))
    end
end

function cs.player_dead.create(obj)
    cs.transform.set_rotation(obj.ent, math.random(0, 3) * 0.5 * math.pi)
end

function cs.player_dead.unpaused_update(obj)
    obj.time = obj.time - cs.timing.dt

    if obj.time <= 0 then
        cs.main.reload()
    end

    if obj.falling then
        local s = cs.transform.get_scale(obj.ent)
        cs.transform.set_scale(obj.ent, math.pow(0.3, cs.timing.dt) * s)
    end
end


-----------------------------------------------------------------------------

cs.player_control = cg.simple_sys()

g_player_speed = 7

function cs.player_control.die(ent, falling)
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
        player_dead = { falling = falling }
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

    local crate_cols = cs.bump.sweep(obj.ent, 0.01 * d,
                                     function (e) return cs.crate.has(e) end)
    for _, col in ipairs(crate_cols) do
        cs.crate.move(col.other, -col.normal)
    end

    if pressed then 
        cs.bump.slide(obj.ent, g_player_speed * d * cs.timing.dt,
            function (e)
                return not cs.portal.has(e) and not cs.dangerous.has(e)
                    and not cs.pit.has(e) and not cs.switch.has(e)
                    and not cs.message_trigger.has(e)
            end)
    end

    for _, col in pairs(cs.bump.sweep(obj.ent)) do
        if cs.dangerous.has(col.other) then
            cs.main.die()
            return
        end
        if cs.pit.has(col.other) then
            local op = cs.transform.get_position(col.other)
            local p = cs.transform.get_position(obj.ent)
            if cg.vec2_len(op - p) < 0.48 then
                cs.transform.set_position(obj.ent, op)
                cs.main.die(true)
                return
            end
        end
    end

    if rot then cs.transform.set_rotation(obj.ent, rot) end
end


-----------------------------------------------------------------------------

cs.wall = cg.simple_sys()

function cs.wall.create(obj)
    cs.bump.add(obj.ent)
end

function cs.wall.unpaused_update(obj)
    if not obj.randomized then
        if math.random() > 0.5 then
            cs.transform.rotate(obj.ent, math.pi)
        end
        obj.randomized = true
    end
end


-----------------------------------------------------------------------------

cs.follower = cg.simple_sys()

cg.simple_prop(cs.follower, 'speed', 4)

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
            local function filter (e)
                return not cs.player_control.has(e)
                    and not cs.pit.has(e) and not cs.switch.has(e)
                    and not cs.follower.has(e)
                    and not cs.bullet.has(e)
            end
            local cols = cs.bump.slide(obj.ent, obj.speed * d * cs.timing.dt,
                                       filter)
        end
    end
end

-----------------------------------------------------------------------------

cs.quad_shooter = cg.simple_sys()

cg.simple_prop(cs.quad_shooter, 'next', 1)
cg.simple_prop(cs.quad_shooter, 'period', 1)
cg.simple_prop(cs.quad_shooter, 'dir', cg.vec2(1, 0))

function cs.quad_shooter.unpaused_update(obj)
    obj.next = obj.next - cs.timing.dt

    if not obj.shooting and obj.next <= 0.28 then
        cs.animation.start(obj.ent, 'shoot')
        obj.shooting = true
    end

    if obj.next <= 0 then
        obj.shooting = false
        obj.next = obj.period

        local player = cs.name.find('player')
        if player ~= cg.entity_nil then
            local pp = cs.transform.get_position(player)
            local p = cs.transform.get_position(obj.ent)
            local d = cg.vec2_len(pp - p)
            if d < 30 then
                cg.add {
                    prefab = prefab_dir .. '/hell-bullet-1.pfb',
                    transform = { position = p + 0.7 * obj.dir },
                    bullet = {
                        creator = cg.Entity(obj.ent),
                        velocity = 8 * obj.dir,
                    }
                }

                if d < 15 then
                    local g = (15 - d) / 15
                    cg.add {
                        sound = {
                            path = sound_dir .. '/quad.wav',
                            playing = true,
                            gain = g * g,
                        }
                    }
                end
            end
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
    local function filter (e)
        return e ~= obj.creator and not cs.bullet.has(e)
            and not cs.player_control.has(e)
            and not cs.pit.has(e)
            and not cs.switch.has(e)
    end
    local cols = cs.bump.slide(obj.ent, obj.velocity * cs.timing.dt,
                               filter)
    for _, col in ipairs(cols) do
        hit = true
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
cg.simple_prop(cs.portal, 'world_2', 'earth')

function cs.portal.unpaused_update(obj)
    local cols = cs.bump.sweep(obj.ent)
    if obj.in_warp == nil then obj.in_warp = false end


    if obj.world_1 == cs.main.world or obj.world_2 == cs.main.world then
        local next_in_warp = false
        for _, col in ipairs(cols) do
            if cs.name.get_name(col.other) == 'player' then
                next_in_warp = true
                if not obj.in_warp then
                    local ow = (obj.world_1 == cs.main.world)
                        and obj.world_2 or obj.world_1
                    obj.in_warp = true  -- do it here to save it for the warp-save
                    cs.main.warp(ow)
                end
            end
        end
        obj.in_warp = next_in_warp
    end
end


-----------------------------------------------------------------------------

cs.crate = cg.simple_sys()

cg.simple_prop(cs.crate, 'stuck', false)

function cs.crate.create(obj)
    cs.bump.add(obj.ent)
end

function cs.crate.move(ent, dir)
    local obj = cs.crate.tbl[ent]
    assert(obj, 'must be in crate')
    if obj.stuck then return end

    local sound
    local function filter (e)
        return not cs.pit.has(e) and not cs.switch.has(e)
            and not cs.message_trigger.has(e)
    end
    local cols = cs.bump.sweep(obj.ent, dir, filter)
    if #cols == 0 then
        cs.bump.set_position(ent, cs.transform.get_position(ent) + dir)
        sound = 'crate.wav'
    end

    local cols = cs.bump.sweep(obj.ent, cg.vec2_zero, cs.pit.has)
    for _, col in pairs(cols) do
        cs.entity.destroy(col.other)
    end
    if #cols > 0 then
        obj.stuck = true
        cs.bump.remove(obj.ent)
        sound = 'crate-pit.wav'

        local ot = cs.sprite.get_texcell(obj.ent)
        cs.sprite.set_texcell(obj.ent, ot + cg.vec2(32, 0))
        cs.sprite.set_depth(obj.ent, 6)
    end

    if sound then play_sound(sound) end
end


-----------------------------------------------------------------------------

cs.dangerous = cg.simple_sys()


-----------------------------------------------------------------------------

cs.pit = cg.simple_sys()

function cs.pit.create(obj)
    cs.bump.add(obj.ent)
end


-----------------------------------------------------------------------------

cs.switcher = cg.simple_sys()     -- can switch switches

cs.switch = cg.simple_sys()

cg.simple_prop(cs.switch, 'on', false)

function cs.switch.create(obj)
    cs.bump.add(obj.ent)
end

function cs.switch.unpaused_update(obj)
    cs.sprite.set_depth(obj.ent, 5)
    if not obj.prev_on then obj.prev_on = false end
    local sound

    local switcher_cols = cs.bump.sweep(obj.ent, cg.vec2_zero, cs.switcher.has)
    obj.on = #switcher_cols > 0

    if obj.on then
        cs.sprite.set_texcell(obj.ent, cg.vec2(160, 128))
        if not obj.prev_on then sound = 'switch-on.wav' end
    else
        cs.sprite.set_texcell(obj.ent, cg.vec2(144, 128))
        if obj.prev_on then sound = 'switch-off.wav' end
    end
    obj.prev_on = obj.on

    local player = cs.name.find('player')
    if player ~= cg.entity_nil and sound then
        local pp = cs.transform.get_position(player)
        local d = cg.vec2_len(pp - cs.transform.get_position(obj.ent))
        if d < 15 then
            local g = (15 - d) / 15
            cg.add {
                sound = {
                    path = sound_dir .. '/' .. sound,
                    playing = true,
                    gain = g * g,
                }
            }
        end
    end
end


-----------------------------------------------------------------------------

cs.door = cg.simple_sys()

cg.simple_prop(cs.door, 'open', false)
cg.simple_prop(cs.door, 'switch', cg.Entity(cg.entity_nil))

door_hor_bbox = cg.bbox(cg.vec2(-0.5, -0.1875), cg.vec2(0.5, 0.1875))
door_vert_bbox = cg.bbox(cg.vec2(-0.1875, -0.5), cg.vec2(0.1875, 0.5))

function cs.door.unpaused_update(obj)
    local v = cg.vec2_rot(cg.vec2(0, 1), cs.transform.get_rotation(obj.ent))
    cs.bump.set_bbox(obj.ent, v.y > 0.5 and door_vert_bbox or door_hor_bbox)

    if obj.switch ~= cg.entity_nil then
        obj.open = cs.switch.get_on(obj.switch)
    end

    local curr_anim = cs.animation.get_curr_anim(obj.ent)
    if obj.open then
        cs.bump.remove(obj.ent)
        if curr_anim ~= 'opening' and curr_anim ~= 'opened' then
            cs.animation.start(obj.ent, 'opening')
        end
    else
        if not cs.bump.has(obj.ent) then
            cs.bump.add(obj.ent)

            -- blocked?
            local function filter (e)
                return not cs.pit.has(e)
            end
            local cols = cs.bump.sweep(obj.ent, cg.vec2_zero, filter)
            if #cols > 0 then cs.bump.remove(obj.ent) end
        end
        if cs.bump.has(obj.ent) and curr_anim ~= 'closing' and curr_anim ~= 'closed' then
            cs.animation.start(obj.ent, 'closing')
        end
    end
end


-----------------------------------------------------------------------------

g_messages = {
    default = "this is a message...",
    wasd = "W, A, S, D: move",
    portal = "This is weird... Where am I?\nAnd what is this thing?",
    hell = "Oh my...",
    earth = "This place looks familiar...",
    ['hell-2'] = "What in hell??",
}

cs.message = cg.simple_sys()

cg.simple_prop(cs.message, 'time', 10)
cg.simple_prop(cs.message, 'message_name', 'default')

function cs.message.set_message_name(ent, name)
    local obj = cs.message.tbl[ent]
    assert(obj, 'must be in message')

    local message = g_messages[name] or g_messages.default
    cs.gui_text.set_str(ent, message)
end

function cs.message.unpaused_update(obj)
    obj.time = obj.time - cs.timing.dt

    if obj.time < 1 then
        local c = cs.gui.get_color(obj.ent)
        c.a = obj.time
        cs.gui.set_color(obj.ent, c)
    end

    if obj.time <= 0 then
        cs.entity.destroy(obj.ent)
    end
end

function show_message(name, time)
    for e in pairs(cs.message.tbl) do
        cs.entity.destroy(e)
    end

    cg.add {
        prefab = prefab_dir .. '/message.pfb',
        message = {
            message_name = name,
            time = time,
        }
    }
end


-----------------------------------------------------------------------------

cs.message_trigger = cg.simple_sys()

cg.simple_prop(cs.message_trigger, 'time', 10)
cg.simple_prop(cs.message_trigger, 'message_name', 'default')

function cs.message_trigger.unpaused_update(obj)
    local pcols = cs.bump.sweep(obj.ent, cg.vec2_zero, cs.player_control.has)
    if #pcols > 0 then
        show_message(obj.message_name, obj.time)
        cs.entity.destroy(obj.ent)
    end
end


-----------------------------------------------------------------------------

cs.main_menu = {}

function cs.main_menu.update_all()
    local new_game = cs.name.find('new-game')
    if new_game ~= cg.entity_nil
    and cs.gui.event_mouse_down(new_game) == cg.MC_LEFT then
        cs.group.destroy('menu')
        cs.timing.set_paused(false)
        save_game('curr.sav', 'portals warp default')
    end

    local continue = cs.name.find('continue')
    if continue ~= cg.entity_nil
    and cs.gui.event_mouse_down(continue) == cg.MC_LEFT then
        cs.group.destroy('menu')
        cs.timing.set_paused(false)
        cs.main.reload()
    end

    local quit = cs.name.find('quit')
    if quit ~= cg.entity_nil
    and cs.gui.event_mouse_down(quit) == cg.MC_LEFT then
        cs.game.quit()
    end
end

function main_menu()
    cs.group.destroy('portals warp default')

    cs.main.world = 'white'
    load_game('portals.lvl', true)
    load_game('start.lvl', true)

    cs.timing.set_paused(true)
    load_game('menu.lvl', true)
end

if cg.args[2] == 'start' then
    main_menu()
else
    cs.group.destroy('portals warp default')

    cs.main.world = 'white'
    load_game('portals.lvl', true)

    cg.add {
        camera = { viewport_height = 18.75 },
        camera_follow = {},
        group = { groups = 'warp' },
    }
end
