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

function cs.edit.mode_switch(mode)
    cs.edit.mode_event('exit')
    cs.edit.mode = mode
    cs.edit.mode_event('enter')
end


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
    if cs.edit.select[ent] ~= nil then
        cs.edit.select[ent] = nil
    else
        cs.edit.select[ent] = true
    end
end

function cs.edit.select_click()
    cs.edit.undo_save()

    -- anything under mouse?
    local ents = cs.edit._get_entities_under_mouse()
    if #ents == 0 then
        cs.edit.select = cg.entity_table()
        return
    end

    -- if something's selected, select the next thing
    ents[#ents + 1] = ents[1]    -- duplicate first at end to wrap-around
    local sel = 0
    for i = 1, #ents - 1 do
        sel = i
        if cs.edit.select[ents[i]] ~= nil then
            break
        end
    end
    cs.edit.select = cg.entity_table()
    cs.edit.select[ents[sel + 1]] = true
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

local grab_old_pos = cg.entity_table()
local grab_mouse_start = cg.vec2_zero

function cs.edit.grab_start()
    cs.edit.undo_save()

    cs.edit.mode_switch('grab')
end
function cs.edit.grab_end()
    cs.edit.mode_switch('normal')
end
function cs.edit.grab_cancel()
    for ent, _ in pairs(cs.edit.select) do
        cs.transform.set_position(ent, old_pos[ent])
    end
    cs.edit.mode_switch('normal')
end

cs.edit.modes.grab = {
    enter = function ()
        grab_mouse_start = cs.input.get_mouse_pos_unit()

        -- store old positions
        old_pos = cg.entity_table()
        for ent, _ in pairs(cs.edit.select) do
            old_pos[ent] = cs.transform.get_position(ent)
        end
    end,

    post_update_all = function ()
        local ms = cs.camera.unit_to_world(grab_mouse_start)
        local mc = cs.camera.unit_to_world(cs.input.get_mouse_pos_unit())
        
        for ent, _ in pairs(cs.edit.select) do
            -- find translation in parent space
            local parent = cs.transform.get_parent(ent)
            local m = cg.mat3_inverse(cs.transform.get_world_matrix(parent))
            local d = cg.mat3_transform(m, mc) - cg.mat3_transform(m, ms)
            cs.transform.set_position(ent, old_pos[ent] + d)
        end
    end,
}


--- bindings -------------------------------------------------------------------

-- normal mode
cs.edit.modes.normal[cg.KC_U] = cs.edit.undo
cs.edit.modes.normal[cg.MC_LEFT] = cs.edit.select_click
cs.edit.modes.normal[cg.KC_D] = cs.edit.destroy
cs.edit.modes.normal[cg.KC_G] = cs.edit.grab_start

-- grab mode
cs.edit.modes.grab[cg.MC_LEFT] = cs.edit.grab_end
cs.edit.modes.grab[cg.KC_ENTER] = cs.edit.grab_end
cs.edit.modes.grab[cg.MC_RIGHT] = cs.edit.grab_cancel
cs.edit.modes.grab[cg.KC_ESCAPE] = cs.edit.grab_cancel


--- main events ----------------------------------------------------------------

cs.edit.mode = 'normal'

function cs.edit.mouse_down(mouse)
    if not cs.edit.get_enabled() then return end

    cs.edit.mode_event(mouse)
end

function cs.edit.key_down(key)
    if not cs.edit.get_enabled() then return end

    cs.edit.mode_event(key)
end

function cs.edit.post_update_all()
    if not cs.edit.get_enabled() then return end

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
