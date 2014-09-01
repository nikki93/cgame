local ffi = require 'ffi'

cs.edit = { inspect = false }


--- expose C functions ---------------------------------------------------------

cs.edit.set_enabled = cg.edit_set_enabled
cs.edit.get_enabled = cg.edit_get_enabled

cs.edit.set_editable = cg.edit_set_editable
cs.edit.get_editable = cg.edit_get_editable

cs.edit.set_grid_size = cg.edit_set_grid_size
cs.edit.get_grid_size = cg.edit_get_grid_size

cs.edit.bboxes_update = cg.edit_bboxes_update
cs.edit.bboxes_has = cg.edit_bboxes_has
cs.edit.bboxes_get = cg.edit_bboxes_get
cs.edit.bboxes_get_num = cg.edit_bboxes_get_num
cs.edit.bboxes_get_nth = cg.edit_bboxes_get_nth
cs.edit.bboxes_set_selected = cg.edit_bboxes_set_selected

cs.edit.line_add = cg.edit_line_add


-- called when using cg.add { ... }, just do nothing
function cs.edit.add()
end

function cs.edit.has()
    return true
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


--- play/stop/pause ------------------------------------------------------------

cs.edit.stopped = true

-- load this when stopped
local stop_savepoint = nil
local stop_save_next_frame = false -- whether to save a stop soon
local function stop_save()
    cs.group.set_save_filter('default edit_inspector', true)
    local s = cg.store_open()
    cs.system.save_all(s)
    stop_savepoint = ffi.string(cg.store_write_str(s))
    cg.store_close(s)

    if cs.timing.get_paused() then cs.edit.stopped = true end
end
function cs.edit.stop_save()
    stop_save_next_frame = true
end

function cs.edit.stop()
    if not stop_savepoint then return end

    cs.group.destroy('default edit_inspector')
    local s = cg.store_open_str(stop_savepoint)
    cs.system.load_all(s)
    cg.store_close(s)

    cs.timing.set_paused(true)
    cs.edit.stopped = true
end

function cs.edit.play()
    cs.timing.set_paused(false)
end

function cs.edit.pause_toggle()
    cs.timing.set_paused(not cs.timing.get_paused())
end


--- undo -----------------------------------------------------------------------

cs.edit.history = {}

function cs.edit.undo_save()
    cs.group.set_save_filter('default edit_inspector', true)
    local s = cg.store_open()
    cs.system.save_all(s)

    local str = ffi.string(cg.store_write_str(s))
    table.insert(cs.edit.history, str)
    if cs.edit.stopped then stop_savepoint = str end -- update stop if stopped

    cg.store_close(s)
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
    local s = cg.store_open_str(str)
    cs.system.load_all(s)
    cg.store_close(s)
end


--- normal mode ----------------------------------------------------------------

cs.edit.modes.normal = {
}

function cs.edit.modes.normal.enter()
    cs.edit.hide_mode_text()
end

cs.edit.mode = 'normal' -- start in normal mode


--- edit gui root --------------------------------------------------------------

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


--- edit camera ----------------------------------------------------------------

local camera_default_height = 25
cs.edit.camera = cg.add {
    group = { groups = 'builtin' },
    edit = { editable = false },
    camera = { viewport_height = camera_default_height, current = false },
}
cs.camera.set_edit_camera(cs.edit.camera)

-- drag

local camera_drag_mouse_prev
function cs.edit.camera_drag_start()
    camera_drag_mouse_prev = cs.input.get_mouse_pos_unit()
    cs.edit_camera_drag.enabled = true
end
function cs.edit.camera_drag_end()
    cs.edit_camera_drag.enabled = false
end

cs.edit_camera_drag = { enabled = false }
function cs.edit_camera_drag.update_all()
    if not cs.edit.get_enabled() then
        cs.edit_camera_drag.enabled = false
        return
    end

    local m = cs.input.get_mouse_pos_unit()

    -- find screen mouse motion in world coordinates, move opposite way
    local campos = cs.transform.local_to_world(cs.edit.camera, cg.vec2_zero)
    local mp = cs.camera.unit_to_world(camera_drag_mouse_prev) - campos
    local mc = cs.camera.unit_to_world(m) - campos
    cs.transform.translate(cs.edit.camera, mp - mc)
    camera_drag_mouse_prev = m
end

-- zoom
local camera_zoom_factor = 0
function cs.edit.camera_zoom(f)
    camera_zoom_factor = camera_zoom_factor + f
    local h = math.pow(0.8, camera_zoom_factor) * camera_default_height
    cs.camera.set_viewport_height(cs.edit.camera, h)
end
function cs.edit.camera_zoom_in()
    cs.edit.camera_zoom(1)
end
function cs.edit.camera_zoom_out()
    cs.edit.camera_zoom(-1)
end


--- other files ----------------------------------------------------------------

-- core edit stuff
require 'cgame.edit_select'
require 'cgame.edit_command'
require 'cgame.edit_bottom_gui'
require 'cgame.edit_inspector'

-- system-specific
require 'cgame.edit_entity'
require 'cgame.edit_transform'
require 'cgame.edit_physics'

-- default binds
require 'cgame.edit_binds'


--- main events ----------------------------------------------------------------

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
function cs.edit.scroll(scroll)
    cs.edit.camera_zoom((scroll.y > 0 and 0.9 or -0.9) + 0.1 * scroll.y)
end

function cs.edit.update_all()
    for ent in pairs(cs.edit.select) do
        if cs.entity.destroyed(ent) then cs.edit.select[ent] = nil end
    end

    if cs.gui.event_mouse_down(cs.edit.play_text) == cg.MC_LEFT then
        if cs.edit.stopped then cs.edit.play()
        else cs.edit.stop() end
    end
    if not cs.timing.get_paused() then cs.edit.stopped = false end

    -- if not enabled skip -- also handle gui visibility
    if not cs.edit.get_enabled() then
        cs.gui.set_visible(cs.edit.gui_root, false)
        return
    end
    cs.gui.set_visible(cs.edit.gui_root, true)

    -- forward to mode
    cs.edit.mode_event('update_all')

    -- update grid text
    local g = cs.edit.get_grid_size()
    if g.x <= 0 and g.y <= 0 then
        cs.gui.set_visible(cs.edit.grid_textbox, false)
    else
        cs.gui.set_visible(cs.edit.grid_textbox, true)
        local s
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

    -- update play/stop text
    if cs.edit.stopped then
        cs.gui_text.set_str(cs.edit.play_text, '\x10')
    else
        cs.gui_text.set_str(cs.edit.play_text, '\xcb')
    end
end

function cs.edit.post_update_all()
    cs.edit.mode_event('post_update_all')

    -- update bbox highlight
    for i = 0, cs.edit.bboxes_get_num() - 1 do
        local pair = cs.edit.bboxes_get_nth(i)
        cs.edit.bboxes_set_selected(pair.ent, cs.edit.select[pair.ent] ~= nil)
    end

    -- save stop?
    if stop_save_next_frame then
        stop_save()
        stop_save_next_frame = false
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
