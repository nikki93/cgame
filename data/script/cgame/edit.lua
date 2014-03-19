local ffi = require 'ffi'

cs.edit = {}

cs.edit.set_enabled = cg.edit_set_enabled
cs.edit.get_enabled = cg.edit_get_enabled

function cs.edit._get_entities_under_mouse()
    local ents = {}

    local m = cs.camera.unit_to_world(cs.input.get_mouse_pos_unit())
    for i = 0, cs.edit.bboxes_get_num() - 1 do
        local pair = cs.edit.bboxes_get_nth(i)

        -- transform m to local space
        local t = cg.mat3_inverse(cs.transform.get_world_matrix(pair.ent))
        if cg.bbox_contains(pair.bbox, cg.mat3_transform(t, m)) then
            table.insert(ents, pair.ent)
        end
    end

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


--- status text ----------------------------------------------------------------

cs.edit.status_text = cs.text.add(cg.vec2(0, cs.game.get_window_size().y - 12),
                                  'edit')
cs.text.set_color(cs.edit.status_text, cg.color_red)
cs.text.set_visible(cs.edit.status_text, false)


--- undo -----------------------------------------------------------------------

cs.edit.history = {}

function cs.edit.undo_save()
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

    cs.system.clear()
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
    local ents = cs.edit._get_entities_under_mouse()
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
    local ents = cs.edit._get_entities_under_mouse()
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


--- destroy --------------------------------------------------------------------

function cs.edit.destroy()
    cs.edit.undo_save()

    for ent, _ in pairs(cs.edit.select) do
        cs.entity.destroy(ent)
    end
    cs.edit.select = cg.entity_table()
end


--- normal mode ----------------------------------------------------------------

cs.edit.modes.normal = {
}


--- grab mode ------------------------------------------------------------------

local grab_old_pos, grab_mouse_start

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

cs.edit.modes.grab = {}

function cs.edit.modes.grab.enter()
    grab_mouse_start = cs.input.get_mouse_pos_unit()

    -- store old positions
    grab_old_pos = cg.entity_table()
    for ent, _ in pairs(cs.edit.select) do
        grab_old_pos[ent] = cs.transform.get_position(ent)
    end
end

function cs.edit.modes.grab.post_update_all()
    local ms = cs.camera.unit_to_world(grab_mouse_start)
    local mc = cs.camera.unit_to_world(cs.input.get_mouse_pos_unit())

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
            local d = cg.mat3_transform(m, mc) - cg.mat3_transform(m, ms)
            cs.transform.set_position(ent, grab_old_pos[ent] + d)
        end
    end
end


--- rotate mode ----------------------------------------------------------------

local rotate_old_posrot, rotate_mouse_start

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
        rotate_mouse_start = cs.input.get_mouse_pos_unit()

        -- store old positions, rotations
        rotate_old_posrot = cg.entity_table()
        for ent, _ in pairs(cs.edit.select) do
            rotate_old_posrot[ent] = {
                pos = cs.transform.get_position(ent),
                rot = cs.transform.get_rotation(ent),
            }
        end
end

function cs.edit.modes.rotate.post_update_all()
    local ms = cs.camera.unit_to_world(rotate_mouse_start)
    local mc = cs.camera.unit_to_world(cs.input.get_mouse_pos_unit())

    for ent, _ in pairs(cs.edit.select) do
        local wpos = cs.transform.get_world_position(ent)
        local rot = cg.vec2_atan2(ms - wpos) - cg.vec2_atan2(mc - wpos)
        cs.transform.set_rotation(ent, rotate_old_posrot[ent].rot + rot)
    end
end


--- bindings -------------------------------------------------------------------

-- normal mode
cs.edit.modes.normal['u'] = cs.edit.undo
cs.edit.modes.normal['<mouse_1>'] = cs.edit.select_click_single
cs.edit.modes.normal['C-<mouse_1>'] = cs.edit.select_click_multi
cs.edit.modes.normal['d'] = cs.edit.destroy
cs.edit.modes.normal['g'] = cs.edit.grab_start
cs.edit.modes.normal['r'] = cs.edit.rotate_start

-- grab mode
cs.edit.modes.grab['<mouse_1>'] = cs.edit.grab_end
cs.edit.modes.grab['<mouse_2>'] = cs.edit.grab_cancel

-- rotate mode
cs.edit.modes.rotate['<mouse_1>'] = cs.edit.rotate_end
cs.edit.modes.rotate['<mouse_2>'] = cs.edit.rotate_cancel


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

function cs.edit.post_update_all()
    if not cs.edit.get_enabled() then
        cs.text.set_visible(cs.edit.status_text, false)
        return
    end
    cs.text.set_visible(cs.edit.status_text, true)

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
    cs.edit.select = d.sel
end
