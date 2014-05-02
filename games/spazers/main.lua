--- game settings ---

cs.game.set_bg_color(cg.color_black)

level_dir = './games/spazers/levels/'
cs.edit.set_default_file(level_dir)
prefab_dir = './games/spazers/prefabs/'
cs.edit.set_default_prefab_file(prefab_dir)

cs.sprite.set_atlas('./games/spazers/atlas.png')

cs.physics.set_gravity(cg.vec2_zero)


--- level stuff ---

function load_level(name)
    cs.group.destroy('default')
    local d = cs.deserializer.open_file(level_dir .. name)
    cs.system.load_all(d)
    cs.deserializer.close(d)
end


--- player_control ---

cs.player_control = cg.simple_sys()

cg.simple_prop(cs.player_control, 'forward_speed', 3)

cg.simple_prop(cs.player_control, 'camera', cg.Entity(cg.entity_nil))

function cs.player_control.mouse_down(mouse)
    if cs.timing.get_paused() then return end

    for _, player in pairs(cs.player_control.tbl) do
        local p = cs.transform.local_to_world(player.ent, cg.vec2(0, 1))
        local r = cs.transform.get_rotation(player.ent)
        local b = cs.prefab.load(prefab_dir .. 'bullet.pfb')
        cs.transform.set_position(b, p)
        cs.transform.set_rotation(b, r)
    end
end

function cs.player_control.unpaused_update(player)
    for _, col in ipairs(cs.physics.get_collisions(player.ent)) do
        if cs.enemy_control.has(col.b) then
            print('you lost!')
            load_level('lose.lvl')
        end
    end

    local p = cs.transform.get_position(player.ent)

    -- follow mouse horizontally
    local m = cs.camera.unit_to_world(cs.input.get_mouse_pos_unit())
    p.x = m.x

    -- move forward
    p.y = p.y + cs.timing.dt * player.forward_speed

    -- make camera follow
    if player.camera ~= cg.entity_nil then
        local camp = cs.transform.get_position(player.camera)
        camp.y = p.y + 8
        cs.transform.set_position(player.camera, camp)
    end

    cs.transform.set_position(player.ent, p)
end


--- bullet_motion ---

cs.bullet_motion = cg.simple_sys()

cg.simple_prop(cs.bullet_motion, 'speed', 20)
cg.simple_prop(cs.bullet_motion, 'lifetime', 3)
cg.simple_prop(cs.bullet_motion, 'time', 0)
cg.simple_prop(cs.bullet_motion, 'player_bullet', true)

function cs.bullet_motion.unpaused_update(bullet)
    -- lifetime
    bullet.time = bullet.time + cs.timing.dt
    if bullet.time >= bullet.lifetime then
        cs.entity.destroy(bullet.ent)
        return
    end

    -- move
    local r = cs.transform.get_rotation(bullet.ent)
    local v = cg.vec2_rot(cg.vec2(0, bullet.speed), r)
    cs.transform.translate(bullet.ent, cs.timing.dt * v)
end


--- enemy_control

cs.enemy_control = cg.simple_sys()

cg.simple_prop(cs.enemy_control, 'health', 3)

function cs.enemy_control.unpaused_update(enemy)
    for _, col in ipairs(cs.physics.get_collisions(enemy.ent)) do
        if cs.bullet_motion.has(col.b)
        and cs.bullet_motion.get_player_bullet(col.b) then
            cs.entity.destroy(col.b)
            enemy.health = enemy.health - 1
        end
    end

    if enemy.health <= 0 then
        print('enemey destroyed!')
        cs.entity.destroy(enemy.ent)
    end
end





--- buttons

cs.play_again_button = cg.simple_sys()
function cs.play_again_button.unpaused_update(button)
    if cs.gui.event_mouse_down(button.ent) == cg.MC_LEFT then
        load_level('test.lvl')
    end
end
