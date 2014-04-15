local ffi = require 'ffi'

cs.edit = {}

cs.edit.set_enabled = cg.edit_set_enabled
cs.edit.get_enabled = cg.edit_get_enabled

cs.edit.set_grid_size = cg.edit_set_grid_size
cs.edit.get_grid_size = cg.edit_get_grid_size

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
    if cs.gui.captured_event() then return end
    cs.edit._mode_exec_bind(false, cs.input.keycode_to_string(key))
end
function cs.edit.mode_key_up(key)
    if cs.gui.captured_event() then return end
    cs.edit._mode_exec_bind(true, cs.input.keycode_to_string(key))
end
function cs.edit.mode_mouse_down(mouse)
    if cs.gui.captured_event() then return end
    cs.edit._mode_exec_bind(false, cs.input.mousecode_to_string(mouse))
end
function cs.edit.mode_mouse_up(mouse)
    if cs.gui.captured_event() then return end
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
    cs.group.set_save_filter('default edit_inspector', true)
    local s = cs.serializer.open_str()
    cs.system.save_all(s)
    table.insert(cs.edit.history, ffi.string(cs.serializer.get_str(s)))
    cs.serializer.close(s)
end

function cs.edit.undo()
    if #cs.edit.history <= 1 then
        print('nothing to undo')
        return
    end

    -- TODO: make 'edit' entity group and destroy all except that?
    cs.group.destroy('default edit_inspector')

    table.remove(cs.edit.history)
    local str = cs.edit.history[#cs.edit.history]
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
    -- anything under mouse?
    local ents = _get_entities_under_mouse()
    if #ents == 0 then
        cs.edit.select = cg.entity_table()
        cs.edit.undo_save()
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

    cs.edit.undo_save()
end

function cs.edit.select_click_multi()
    -- anything under mouse?
    local ents = _get_entities_under_mouse()
    if #ents == 0 then
        cs.edit.undo_save()
        return
    end

    -- if something isn't selected, select it
    for i = 1, #ents do
        if not cs.edit.select[ents[i]] then
            cs.edit.select[ents[i]] = true
            cs.edit.undo_save()
            return
        end
    end

    -- otherwise deselect the first
    cs.edit.select[ents[1]] = nil

    cs.edit.undo_save()
end

function cs.edit.select_clear()
    cs.edit.select = cg.entity_table()
end


--- entity management ----------------------------------------------------------

function cs.edit.destroy_rec()
    for ent, _ in pairs(cs.edit.select) do
        cs.transform.destroy_rec(ent)
    end

    cs.edit.undo_save()
end

function cs.edit.destroy()
    for ent, _ in pairs(cs.edit.select) do
        cs.entity.destroy(ent)
    end

    cs.edit.undo_save()
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

function cs.edit.modes.normal.enter()
    cs.edit.hide_mode_text()
end

--- grab mode ------------------------------------------------------------------

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
    for ent, _ in pairs(cs.edit.select) do
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
    for ent, _ in pairs(cs.edit.select) do
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


--- rotate mode ----------------------------------------------------------------

local rotate_old_posrot, rotate_mouse_start, rotate_pivot

function cs.edit.rotate_start()
    cs.edit.set_mode('rotate')
end
function cs.edit.rotate_end()
    cs.edit.set_mode('normal')
    cs.edit.undo_save()
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
    cs.edit.undo_save()
end

cs.edit.boxsel_box = cg.add {
    group = { groups = 'builtin' },
    edit = { editable = false },
    transform = { position = cg.vec2(-20, -20) },
    gui_rect = { size = cg.vec2(10, 10), hfit = false, vfit = false },
    gui = { captures_events = false, color = cg.color(0.8, 0.5, 0.1, 0.3) },
}

cs.edit.modes.boxsel = {}

function cs.edit.modes.boxsel.enter()
    cs.edit.set_mode_text('boxsel')

    boxsel_has_begun = false
end

function cs.edit.modes.boxsel.exit()
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


--- command mode ---------------------------------------------------------------

cs.edit.modes.command = {}

local command_end_callback, command_completion_func, command_completions
local command_completions_index, command_always_complete

local function command_update_completions()
    local s = cs.gui_text.get_str(cs.edit.command_text)
    command_completions = command_completion_func(s)
    cs.gui_text.set_str(cs.edit.command_completions_text,
                        table.concat(command_completions, ' | '))
end

-- returns a completion function that uses substring search
function cs.edit.command_completion_substr(t)
    return function(s)
        local comps = {}
        for k, _ in pairs(t) do
            if string.find(k, s) then table.insert(comps, k) end
        end
        return comps
    end
end

local function run_string(s)
    local r, e = loadstring(s)
    if r then r() else error(e) end
end

function cs.edit.command_start(prompt, callback, completion_func,
                               always_complete)
    cs.edit.set_mode('command')

    -- default is eval script
    prompt = prompt or 'lua: '
    command_end_callback = callback or run_string
    command_completion_func = completion_func or function () return {} end
    command_always_complete = always_complete and true or false

    cs.gui_text.set_str(cs.edit.command_text_colon, prompt)
    command_update_completions()
end
function cs.edit.command_end()
    if command_always_complete then
        if #command_completions == 0 then return end -- no completions
        cs.edit.command_complete()
    end

    cs.edit.set_mode('normal')

    local s = cs.gui_text.get_str(cs.edit.command_text)
    if command_end_callback then command_end_callback(s)
    else print('no command callback for \'' .. s .. '\'') end
end
function cs.edit.command_cancel()
    cs.edit.set_mode('normal')
end

-- actually pick a completion
function cs.edit.command_complete()
    if #command_completions > 0 then
        local comp = command_completions[1]
        cs.gui_text.set_str(cs.edit.command_text, comp)
        cs.gui_textedit.set_cursor(cs.edit.command_text, #comp)
    end
end

function cs.edit.modes.command.enter()
    cs.edit.set_mode_text('command')

    cs.gui.set_visible(cs.edit.command_bar, true)

    cs.gui_text.set_str(cs.edit.command_text, '')
    cs.gui_text.set_str(cs.edit.command_completions_text, '')
end
function cs.edit.modes.command.exit()
    cs.gui.set_visible(cs.edit.command_bar, false)

    cs.gui.set_focus(cs.edit.command_text, false)
    command_completions = {}
end

function cs.edit.modes.command.update_all()
    -- done?
    if cs.gui.event_key_down(cs.edit.command_text) == cg.KC_ENTER then
        cs.edit.command_end()
        return
    elseif cs.gui.event_focus_exit(cs.edit.command_text) then
        cs.edit.command_cancel()
        return
    end

    if cs.gui.event_changed(cs.edit.command_text) then
        command_update_completions()
    end
    if cs.gui.event_key_down(cs.edit.command_text) == cg.KC_TAB then
        cs.edit.command_complete()
    end

    cs.gui.set_focus(cs.edit.command_text, true)
end


--- command prompts ------------------------------------------------------------

local function command_gridx(x)
    local function gridy(y)
        cs.edit.set_grid_size(cg.vec2(tonumber(x) or 0, tonumber(y) or 0))
    end
    cs.edit.command_start('grid y: ', gridy)
end
function cs.edit.command_grid()
    cs.edit.command_start('grid x: ', command_gridx)
end

-- inspects system on selected entities, or creates entity if none selected
function cs.edit.command_inspect()
    local add = cg.entity_table_empty(cs.edit.select)

    local function system(s)
        if add then
            local e = cg.entity_create()
            cs.edit_inspector.add(e, s)
            cs.edit.select[e] = true
        elseif not cg.entity_table_empty(cs.edit.select) then
            for ent, _ in pairs(cs.edit.select) do
                cs.edit_inspector.add(ent, s)
            end
        end
        cs.edit.undo_save()
    end

    -- complete to systems that have properties listed
    local comp = cs.edit.command_completion_substr(cs.props)

    cs.edit.command_start(add and 'new entity: ' or 'edit system: ',
                          system, comp, true)
end


--- gui ------------------------------------------------------------------------

cs.edit.gui_root = cg.add {
    group = { groups = 'builtin' },
    edit = { editable = false },
    gui_rect = { hfill = true, vfill = true },
    gui = {
        visible = false,
        captures_events = false,
        color = cg.color(0, 0, 0, 0), -- invisible
        halign = cg.GA_MIN, valign = cg.GA_MIN,
        padding = cg.vec2_zero,
    }
}

require 'cgame.edit_bottom_gui'
require 'cgame.edit_inspector'


--- bindings -------------------------------------------------------------------

-- normal mode
cs.edit.modes.normal['S-;'] = cs.edit.command_start
cs.edit.modes.normal['u'] = cs.edit.undo
cs.edit.modes.normal['a'] = cs.edit.select_clear
cs.edit.modes.normal['<mouse_1>'] = cs.edit.select_click_single
cs.edit.modes.normal['S-<mouse_1>'] = cs.edit.select_click_multi
cs.edit.modes.normal['C-<mouse_1>'] = cs.edit.select_click_multi
cs.edit.modes.normal['x'] = cs.edit.destroy
cs.edit.modes.normal['S-x'] = cs.edit.destroy_rec
cs.edit.modes.normal['S-d'] = cs.edit.duplicate
cs.edit.modes.normal['g'] = cs.edit.grab_start
cs.edit.modes.normal['r'] = cs.edit.rotate_start
cs.edit.modes.normal['b'] = cs.edit.boxsel_start

cs.edit.modes.normal[','] = cs.edit.command_inspect
cs.edit.modes.normal['S-g'] = cs.edit.command_grid

-- grab mode
cs.edit.modes.grab['<enter>'] = cs.edit.grab_end
cs.edit.modes.grab['<escape>'] = cs.edit.grab_cancel
cs.edit.modes.grab['<mouse_1>'] = cs.edit.grab_end
cs.edit.modes.grab['<mouse_2>'] = cs.edit.grab_cancel
cs.edit.modes.grab['g'] = cs.edit.grab_snap_on
cs.edit.modes.grab['<left>'] = cs.edit.grab_move_left
cs.edit.modes.grab['<right>'] = cs.edit.grab_move_right
cs.edit.modes.grab['<up>'] = cs.edit.grab_move_up
cs.edit.modes.grab['<down>'] = cs.edit.grab_move_down
cs.edit.modes.grab['S-<left>'] = function () cs.edit.grab_move_left(5) end
cs.edit.modes.grab['S-<right>'] = function () cs.edit.grab_move_right(5) end
cs.edit.modes.grab['S-<up>'] = function () cs.edit.grab_move_up(5) end
cs.edit.modes.grab['S-<down>'] = function () cs.edit.grab_move_down(5) end

-- rotate mode
cs.edit.modes.rotate['<enter>'] = cs.edit.rotate_end
cs.edit.modes.rotate['<escape>'] = cs.edit.rotate_cancel
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
        cs.gui.set_visible(cs.edit.gui_root, false)
        return
    end
        cs.gui.set_visible(cs.edit.gui_root, true)

    cs.edit.mode_event('update_all')

    -- update grid text
    local g = cs.edit.get_grid_size()
    if g.x <= 0 and g.y <= 0 then
        cs.gui.set_visible(cs.edit.grid_textbox, false)
    else
        cs.gui.set_visible(cs.edit.grid_textbox, true)
        if g.x == g.y then s = string.format('grid %.4g', g.x)
        else s = string.format('grid %.4g %.4g', g.x, g.y) end
        cs.gui_text.set_str(cs.edit.grid_text, s)
    end

    -- update select text
    local nselect = 0
    for _ in pairs(cs.edit.select) do nselect = nselect + 1 end
    if nselect > 0 then
        cs.gui.set_visible(cs.edit.select_textbox, true)
        cs.gui_text.set_str(cs.edit.select_text, 'select ' .. nselect)
    else
        cs.gui.set_visible(cs.edit.select_textbox, false)
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
