--- grab -----------------------------------------------------------------------

local grab_old_pos, grab_mouse_start
local grab_disp -- 'extra' displacement on top of mouse motion
local grab_snap -- whether snapping to grid

function cs.edit.grab_start()
    cs.edit.set_mode('grab')
end
function cs.edit.grab_end()
    cs.edit.set_mode('normal')
    cs.edit.undo_save()
end
function cs.edit.grab_cancel()
    for ent in pairs(cs.edit.select) do
        cs.transform.set_position(ent, grab_old_pos[ent])
    end
    cs.edit.set_mode('normal')
end

function cs.edit.grab_snap_on() grab_snap = true end
function cs.edit.grab_snap_off() grab_snap = false end

-- move all selected grid size times mult in a direction
function cs.edit.grab_move_left(mult)
    local g = cs.edit.get_grid_size()
    grab_disp.x = grab_disp.x - (mult or 1) * (g.x > 0 and g.x or 1)
end
function cs.edit.grab_move_right(mult)
    local g = cs.edit.get_grid_size()
    grab_disp.x = grab_disp.x + (mult or 1) * (g.x > 0 and g.x or 1)
end
function cs.edit.grab_move_up(mult)
    local g = cs.edit.get_grid_size()
    grab_disp.y = grab_disp.y + (mult or 1) * (g.y > 0 and g.y or 1)
end
function cs.edit.grab_move_down(mult)
    local g = cs.edit.get_grid_size()
    grab_disp.y = grab_disp.y - (mult or 1) * (g.y > 0 and g.y or 1)
end

cs.edit.modes.grab = {}

function cs.edit.modes.grab.enter()
    grab_mouse_start = cs.input.get_mouse_pos_unit()
    grab_disp = cg.Vec2(cg.vec2_zero)
    grab_snap = false

    -- store old positions
    grab_old_pos = cg.entity_table()
    for ent in pairs(cs.edit.select) do
        grab_old_pos[ent] = cs.transform.get_position(ent)
    end
end

function cs.edit.modes.grab.update_all()
    local ms = cs.camera.unit_to_world(grab_mouse_start)
    local mc = cs.camera.unit_to_world(cs.input.get_mouse_pos_unit())

    -- snap mc to grid if needed
    if grab_snap then
        local g = cs.edit.get_grid_size()
        if g.x > 0 then
            mc.x = g.x * math.floor(0.5 + (mc.x - ms.x) / g.x) + ms.x
        end
        if g.y > 0 then
            mc.y = g.y * math.floor(0.5 + (mc.y - ms.y) / g.y) + ms.y
        end
    end

    -- move selected objects
    for ent in pairs(cs.edit.select) do
        -- move only if no ancestor is being moved (avoid double-move)
        local anc = cs.transform.get_parent(ent)
        while anc ~= cg.entity_nil and not cs.edit.select[anc] do
            anc = cs.transform.get_parent(anc)
        end
        if anc == cg.entity_nil then
            -- find translation in parent space
            local parent = cs.transform.get_parent(ent)
            local m = cg.mat3_inverse(cs.transform.get_world_matrix(parent))
            local d = cg.mat3_transform(m, mc)
                - cg.mat3_transform(m, ms)
            d = d + cg.mat3_transform(m, grab_disp)
                - cg.mat3_transform(m, cg.vec2_zero)
            cs.transform.set_position(ent, grab_old_pos[ent] + d)
        end
    end

    -- update
    local snap_text = grab_snap and 'snap ' or ''
    local d = mc - ms + grab_disp
    local mode_text = string.format('grab %s%.4f, %.4f', snap_text, d.x, d.y)
    cs.edit.set_mode_text(mode_text)
end


--- rotate ---------------------------------------------------------------------

local rotate_old_posrot, rotate_mouse_start, rotate_pivot

function cs.edit.rotate_start()
    cs.edit.set_mode('rotate')
end
function cs.edit.rotate_end()
    cs.edit.set_mode('normal')
    cs.edit.undo_save()
end
function cs.edit.rotate_cancel()
    for ent in pairs(cs.edit.select) do
        cs.transform.set_position(ent, rotate_old_posrot[ent].pos)
        cs.transform.set_rotation(ent, rotate_old_posrot[ent].rot)
    end
    cs.edit.set_mode('normal')
end

cs.edit.modes.rotate = {}

function cs.edit.modes.rotate.enter()
    cs.edit.set_mode_text('rotate')

    rotate_mouse_start = cs.input.get_mouse_pos_unit()

    -- store old positions, rotations
    rotate_old_posrot = cg.entity_table()
    for ent in pairs(cs.edit.select) do
        rotate_old_posrot[ent] = {
            pos = cs.transform.get_position(ent),
            rot = cs.transform.get_rotation(ent),
        }
    end

    -- compute pivot (currently just the median)
    local n = 0
    rotate_pivot = cg.vec2_zero
    for ent in pairs(cs.edit.select) do
        rotate_pivot = rotate_pivot + cs.transform.get_world_position(ent)
        n = n + 1
    end
    rotate_pivot = rotate_pivot / n
end

function cs.edit.modes.rotate.update_all()
    local ms = cs.camera.unit_to_world(rotate_mouse_start)
    local mc = cs.camera.unit_to_world(cs.input.get_mouse_pos_unit())
    local ang = cg.vec2_atan2(mc - rotate_pivot)
        - cg.vec2_atan2(ms - rotate_pivot)

    for ent in pairs(cs.edit.select) do
        -- set new rotation
        cs.transform.set_rotation(ent, rotate_old_posrot[ent].rot + ang)

        -- compute new position
        local parent = cs.transform.get_parent(ent)
        local m = cs.transform.get_world_matrix(parent)
        local wpos = cg.mat3_transform(m, rotate_old_posrot[ent].pos)
        local d = wpos - rotate_pivot
        d = cg.vec2_rot(d, ang)
        wpos = rotate_pivot + d

        -- set new position
        local im = cg.mat3_inverse(m)
        cs.transform.set_position(ent, cg.mat3_transform(im, wpos))
    end
end
