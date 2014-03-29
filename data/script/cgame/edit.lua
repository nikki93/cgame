local ffi = require 'ffi'

cs.edit = {}

cs.edit.set_enabled = cg.edit_set_enabled
cs.edit.get_enabled = cg.edit_get_enabled

cs.edit.set_editable = cg.edit_set_editable
cs.edit.get_editable = cg.edit_get_editable

-- called when using cg.add { ... }, just do nothing
function cs.edit.add()
end

local function _get_entities_under_mouse()
    local ents = {}

    local m = cs.camera.unit_to_world(cs.input.get_mouse_pos_unit())
    for i = 0, cs.edit.bboxes_get_num() - 1 do
        local pair = cs.edit.bboxes_get_nth(i)

        -- transform m to local space
        local t = cg.mat3_inverse(cs.transform.get_world_matrix(pair.ent))
        if cg.bbox_contains(pair.bbox, cg.mat3_transform(t, m)) then
            table.insert(ents, cg.Entity(pair.ent))
        end
    end

    -- sort by distance to mouse
    local distcomp = function (e1, e2)
        local p1 = cs.transform.get_world_position(e1)
        local p2 = cs.transform.get_world_position(e2)
        return cg.vec2_dist(p1, m) < cg.vec2_dist(p2, m)
    end
    table.sort(ents, distcomp)

    return ents
end


--- mode management-------------------------------------------------------------

cs.edit.modes = { ['none'] = {}, ['all'] = {} }
cs.edit.mode = 'none'

function cs.edit.mode_event(evt)
    -- convert key/mouse enum to number
    if type(evt) == 'cdata' then evt = tonumber(evt) end

    -- forward to all, then to current mode
    local e = cs.edit.modes.all[evt]
    if e then e() end
    local e = cs.edit.modes[cs.edit.mode][evt]
    if e then e() end
end

-- codestr is 'a', 'b', 'c', '<tab>' etc. as returned by input.*_to_string(...)
function cs.edit._mode_exec_bind(up, codestr)
    -- modifier prefixes
    local mods = {
        [cg.KC_LEFT_CONTROL] = 'C-',
        [cg.KC_RIGHT_CONTROL] = 'C-',
        [cg.KC_LEFT_ALT] = 'M-',
        [cg.KC_RIGHT_ALT] = 'M-',
        [cg.KC_LEFT_SHIFT] = 'S-',
        [cg.KC_RIGHT_SHIFT] = 'S-',
    }
    for mod, prefix in pairs(mods) do
        if cs.input.key_down(mod) then
            codestr = prefix .. codestr
        end
    end

    -- up prefix
    codestr = up and ('^' .. codestr) or codestr

    -- execute!
    cs.edit.mode_event(codestr)
end

function cs.edit.mode_key_down(key)
    cs.edit._mode_exec_bind(false, cs.input.keycode_to_string(key))
end
function cs.edit.mode_key_up(key)
    cs.edit._mode_exec_bind(true, cs.input.keycode_to_string(key))
end
function cs.edit.mode_mouse_down(mouse)
    cs.edit._mode_exec_bind(false, cs.input.mousecode_to_string(mouse))
end
function cs.edit.mode_mouse_up(mouse)
    cs.edit._mode_exec_bind(true, cs.input.mousecode_to_string(mouse))
end

function cs.edit.set_mode(mode)
    cs.edit.mode_event('exit')
    cs.edit.mode = mode
    cs.edit.mode_event('enter')
end


--- undo -----------------------------------------------------------------------

cs.edit.history = {}

function cs.edit.undo_save()
    cs.group.set_save_filter('default', true)
    local s = cs.serializer.open_str()
    cs.system.save_all(s)
    table.insert(cs.edit.history, ffi.string(cs.serializer.get_str(s)))
    cs.serializer.close(s)
end

function cs.edit.undo()
    if #cs.edit.history == 0 then
        print('nothing to undo')
        return
    end

    -- TODO: make 'edit' entity group and destroy all except that?
    cs.group.destroy('default')

    local str = table.remove(cs.edit.history)
    local d = cs.deserializer.open_str(str)
    cs.system.load_all(d)
    cs.deserializer.close(d)
end


--- bbox -----------------------------------------------------------------------

cs.edit.bboxes_clear = cg.edit_bboxes_clear
cs.edit.bboxes_update = cg.edit_bboxes_update
cs.edit.bboxes_get_num = cg.edit_bboxes_get_num
cs.edit.bboxes_get_nth = cg.edit_bboxes_get_nth
cs.edit.bboxes_set_selected = cg.edit_bboxes_set_selected


--- select ---------------------------------------------------------------------

cs.edit.select = cg.entity_table()

-- get some selected entity, or nil if none selected
function cs.edit.select_get_first()
    for ent, _ in pairs(cs.edit.select) do
        return ent
    end
    return nil
end

function cs.edit.select_toggle(ent)
    if cs.edit.select[ent] then
        cs.edit.select[ent] = nil
    else
        cs.edit.select[ent] = true
    end
end

function cs.edit.select_click_single()
    cs.edit.undo_save()

    -- anything under mouse?
    local ents = _get_entities_under_mouse()
    if #ents == 0 then
        cs.edit.select = cg.entity_table()
        return
    end

    -- if something's already selected, select the next thing
    ents[#ents + 1] = ents[1]    -- duplicate first at end to wrap-around
    local sel = 0
    for i = 1, #ents - 1 do
        sel = i
        if cs.edit.select[ents[i]] then
            break
        end
    end
    cs.edit.select = cg.entity_table()
    cs.edit.select[ents[sel + 1]] = true
end

function cs.edit.select_click_multi()
    cs.edit.undo_save()

    -- anything under mouse?
    local ents = _get_entities_under_mouse()
    if #ents == 0 then
        return
    end

    -- if something isn't selected, select it
    for i = 1, #ents do
        if not cs.edit.select[ents[i]] then
            cs.edit.select[ents[i]] = true
            return
        end
    end

    -- otherwise deselect the first
    cs.edit.select[ents[1]] = nil
end

function cs.edit.select_clear()
    cs.edit.select = cg.entity_table()
end


--- entity management ----------------------------------------------------------

function cs.edit.destroy()
    cs.edit.undo_save()

    for ent, _ in pairs(cs.edit.select) do
        cs.entity.destroy(ent)
    end
    cs.edit.select_clear()
end

function cs.edit.duplicate()
    -- save just current selection to a string
    for ent, _ in pairs(cs.edit.select) do
        if cs.transform.has(ent) then
            cs.transform.set_save_filter_rec(ent, true)
        else
            cs.entity.set_save_filter(ent, true)
        end
    end
    local s = cs.serializer.open_str()
    cs.system.save_all(s)
    local str = ffi.string(cs.serializer.get_str(s))
    cs.serializer.close(s)

    -- clear selection
    cs.edit.select_clear()

    -- load from the string -- they were selected on save and so will be
    -- selected when loaded
    local d = cs.deserializer.open_str(str)
    cs.system.load_all(d)
    cs.deserializer.close(d)
end


--- normal mode ----------------------------------------------------------------

cs.edit.modes.normal = {
}


--- grab mode ------------------------------------------------------------------

local grab_old_pos, grab_mouse_prev

function cs.edit.grab_start()
    cs.edit.undo_save()
    cs.edit.set_mode('grab')
end
function cs.edit.grab_end()
    cs.edit.set_mode('normal')
end
function cs.edit.grab_cancel()
    for ent, _ in pairs(cs.edit.select) do
        cs.transform.set_position(ent, grab_old_pos[ent])
    end
    cs.edit.set_mode('normal')
end

-- move all selected by d in world space
function cs.edit.grab_move(d)
    for ent, _ in pairs(cs.edit.select) do
        -- move only if no ancestor is being moved (avoid double-move)
        local anc = cs.transform.get_parent(ent)
        while anc ~= cg.entity_nil and not cs.edit.select[anc] do
            anc = cs.transform.get_parent(anc)
        end
        if anc == cg.entity_nil then
            -- find translation in parent space
            local parent = cs.transform.get_parent(ent)
            local m = cg.mat3_inverse(cs.transform.get_world_matrix(parent))
            cs.transform.translate(ent, cg.mat3_transform(m, d)
                                       - cg.mat3_transform(m, cg.vec2_zero))
        end
    end
end

-- move all selected grid size times mult in a direction
function cs.edit.grab_move_left(mult)
    cs.edit.grab_move(cg.vec2(-1, 0) * (mult or 1))
end
function cs.edit.grab_move_right(mult)
    cs.edit.grab_move(cg.vec2(1, 0) * (mult or 1))
end
function cs.edit.grab_move_up(mult)
    cs.edit.grab_move(cg.vec2(0, 1) * (mult or 1))
end
function cs.edit.grab_move_down(mult)
    cs.edit.grab_move(cg.vec2(0, -1) * (mult or 1))
end

cs.edit.modes.grab = {}

function cs.edit.modes.grab.enter()
    cs.edit.set_mode_text('grab')

    grab_mouse_prev = cs.input.get_mouse_pos_unit()

    -- store old positions
    grab_old_pos = cg.entity_table()
    for ent, _ in pairs(cs.edit.select) do
        grab_old_pos[ent] = cs.transform.get_position(ent)
    end
end
function cs.edit.modes.grab.exit()
    cs.edit.hide_mode_text()
end

function cs.edit.modes.grab.update_all()
    local mp = cs.camera.unit_to_world(grab_mouse_prev)
    local mc = cs.camera.unit_to_world(cs.input.get_mouse_pos_unit())
    cs.edit.grab_move(mc - mp)
    grab_mouse_prev = cs.input.get_mouse_pos_unit()
end


--- rotate mode ----------------------------------------------------------------

local rotate_old_posrot, rotate_mouse_start, rotate_pivot

function cs.edit.rotate_start()
    cs.edit.undo_save()
    cs.edit.set_mode('rotate')
end
function cs.edit.rotate_end()
    cs.edit.set_mode('normal')
end
function cs.edit.rotate_cancel()
    for ent, _ in pairs(cs.edit.select) do
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
    for ent, _ in pairs(cs.edit.select) do
        rotate_old_posrot[ent] = {
            pos = cs.transform.get_position(ent),
            rot = cs.transform.get_rotation(ent),
        }
    end

    -- compute pivot (currently just the median)
    local n = 0
    rotate_pivot = cg.vec2_zero
    for ent, _ in pairs(cs.edit.select) do
        rotate_pivot = rotate_pivot + cs.transform.get_world_position(ent)
        n = n + 1
    end
    rotate_pivot = rotate_pivot / n
end
function cs.edit.modes.rotate.exit()
    cs.edit.hide_mode_text()
end

function cs.edit.modes.rotate.update_all()
    local ms = cs.camera.unit_to_world(rotate_mouse_start)
    local mc = cs.camera.unit_to_world(cs.input.get_mouse_pos_unit())
    local ang = cg.vec2_atan2(ms - rotate_pivot)
        - cg.vec2_atan2(mc - rotate_pivot)

    for ent, _ in pairs(cs.edit.select) do
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


--- boxsel mode ----------------------------------------------------------------

local boxsel_has_begun, boxsel_init_mouse_pos

local function _boxsel_select()
end

function cs.edit.boxsel_start()
    cs.edit.undo_save()
    cs.edit.set_mode('boxsel')
end
function cs.edit.boxsel_begin()
    boxsel_has_begun = true
    boxsel_init_mouse_pos = cs.input.get_mouse_pos_pixels()
end
function cs.edit.boxsel_end()
    cs.edit.select = cg.entity_table()
    cs.edit.boxsel_end_add()
end
function cs.edit.boxsel_end_add()
    if boxsel_has_begun then
        b = cs.camera.pixels_to_world(boxsel_init_mouse_pos)
        e = cs.camera.pixels_to_world(cs.input.get_mouse_pos_pixels())
        bb = cg.BBox(cg.bbox_bound(b, e))

        for i = 0, cs.edit.bboxes_get_num() - 1 do
            local ent = cg.Entity(cs.edit.bboxes_get_nth(i).ent)
            if cg.bbox_contains(bb, cs.transform.get_world_position(ent)) then
                cs.edit.select[ent] = true
            end
        end
    end

    cs.edit.set_mode('normal')
end

cs.edit.boxsel_box = cg.add {
    group = { groups = 'builtin' },
    edit = { editable = false },
    transform = { position = cg.vec2(-20, -20) },
    gui_rect = { size = cg.vec2(10, 10), hfit = false, vfit = false },
    gui = { color = cg.color(0.8, 0.5, 0.1, 0.3) },
}

cs.edit.modes.boxsel = {}

function cs.edit.modes.boxsel.enter()
    cs.edit.set_mode_text('boxsel')

    boxsel_has_begun = false
end

function cs.edit.modes.boxsel.exit()
    cs.edit.hide_mode_text()

    cs.transform.set_position(cs.edit.boxsel_box, cg.vec2(-20, -20))
    cs.gui_rect.set_size(cs.edit.boxsel_box, cg.vec2(10, 10))
end

function cs.edit.modes.boxsel.update_all()
    if not boxsel_has_begun then return end

    m = cs.input.get_mouse_pos_pixels()
    b = cg.BBox(cg.bbox_bound(m, boxsel_init_mouse_pos))
    cs.transform.set_position(cs.edit.boxsel_box, cg.vec2(b.min.x, b.max.y))
    cs.gui_rect.set_size(cs.edit.boxsel_box, b.max - b.min)
end


--- bottom gui -----------------------------------------------------------------

require 'cgame.edit_bottom_gui'


--- bindings -------------------------------------------------------------------

-- normal mode
cs.edit.modes.normal['u'] = cs.edit.undo
cs.edit.modes.normal['a'] = cs.edit.select_clear
cs.edit.modes.normal['<mouse_1>'] = cs.edit.select_click_single
cs.edit.modes.normal['S-<mouse_1>'] = cs.edit.select_click_multi
cs.edit.modes.normal['C-<mouse_1>'] = cs.edit.select_click_multi
cs.edit.modes.normal['x'] = cs.edit.destroy
cs.edit.modes.normal['S-d'] = cs.edit.duplicate
cs.edit.modes.normal['g'] = cs.edit.grab_start
cs.edit.modes.normal['r'] = cs.edit.rotate_start
cs.edit.modes.normal['b'] = cs.edit.boxsel_start

-- grab mode
cs.edit.modes.grab['<enter>'] = cs.edit.grab_end
cs.edit.modes.grab['<mouse_1>'] = cs.edit.grab_end
cs.edit.modes.grab['<mouse_2>'] = cs.edit.grab_cancel
cs.edit.modes.grab['<left>'] = cs.edit.grab_move_left
cs.edit.modes.grab['<right>'] = cs.edit.grab_move_right
cs.edit.modes.grab['<up>'] = cs.edit.grab_move_up
cs.edit.modes.grab['<down>'] = cs.edit.grab_move_down
cs.edit.modes.grab['S-<left>'] = function () cs.edit.grab_move_left(10) end
cs.edit.modes.grab['S-<right>'] = function () cs.edit.grab_move_right(10) end
cs.edit.modes.grab['S-<up>'] = function () cs.edit.grab_move_up(10) end
cs.edit.modes.grab['S-<down>'] = function () cs.edit.grab_move_down(10) end

-- rotate mode
cs.edit.modes.rotate['<enter>'] = cs.edit.rotate_end
cs.edit.modes.rotate['<mouse_1>'] = cs.edit.rotate_end
cs.edit.modes.rotate['<mouse_2>'] = cs.edit.rotate_cancel

-- boxsel mode
cs.edit.modes.boxsel['<mouse_1>'] = cs.edit.boxsel_begin
cs.edit.modes.boxsel['C-<mouse_1>'] = cs.edit.boxsel_begin
cs.edit.modes.boxsel['^<mouse_1>'] = cs.edit.boxsel_end
cs.edit.modes.boxsel['^C-<mouse_1>'] = cs.edit.boxsel_end_add


--- main events ----------------------------------------------------------------

cs.edit.mode = 'normal'

function cs.edit.key_up(key)
    if not cs.edit.get_enabled() then return end
    cs.edit.mode_key_up(key)
end
function cs.edit.key_down(key)
    if not cs.edit.get_enabled() then return end
    cs.edit.mode_key_down(key)
end
function cs.edit.mouse_down(mouse)
    if not cs.edit.get_enabled() then return end
    cs.edit.mode_mouse_down(mouse)
end
function cs.edit.mouse_up(mouse)
    if not cs.edit.get_enabled() then return end
    cs.edit.mode_mouse_up(mouse)
end

function cs.edit.update_all()
    for ent, _ in pairs(cs.edit.select) do
        if cs.entity.destroyed(ent) then cs.edit.select[ent] = nil end
    end

    if not cs.edit.get_enabled() then
        cs.gui.set_visible(cs.edit.bottom_rect, false)
        return
    end
        cs.gui.set_visible(cs.edit.bottom_rect, true)

    cs.edit.mode_event('update_all')

    -- update select text
    local nselect = 0
    for _ in pairs(cs.edit.select) do nselect = nselect + 1 end
    if nselect > 0 then
        cs.gui.set_visible(cs.edit.select_text, true)
        cs.gui_text.set_str(cs.edit.select_text, nselect .. ' selected')
    else
        cs.gui.set_visible(cs.edit.select_text, false)
        cs.gui_text.set_str(cs.edit.select_text, '')
    end
end

function cs.edit.post_update_all()
    cs.edit.mode_event('post_update_all')

    -- update bbox highlight
    for i = 0, cs.edit.bboxes_get_num() - 1 do
        local pair = cs.edit.bboxes_get_nth(i)
        cs.edit.bboxes_set_selected(pair.ent, cs.edit.select[pair.ent] ~= nil)
    end
end

function cs.edit.save_all()
    return {
        sel = cs.edit.select,
    }
end
function cs.edit.load_all(d)
    cg.entity_table_merge(cs.edit.select, d.sel)
end
