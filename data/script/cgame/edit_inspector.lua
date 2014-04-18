local ffi = require 'ffi'
local refct = require 'reflect'

--- property -------------------------------------------------------------------

local property_types = {}

local function property_create_container(inspector, prop)
    prop.container = cg.add {
        transform = { parent = inspector.window_body },
        gui = {
            padding = cg.vec2_zero,
            color = cg.color_clear,
            valign = cg.GA_TABLE,
            halign = cg.GA_MIN,
        },
        gui_rect = { hfill = true },
    }
end

local function property_create_label(inspector, prop)
    prop.label = cg.add {
        transform = { parent = prop.container },
        gui = {
            color = cg.color_white,
            valign = cg.GA_MID,
            halign = cg.GA_TABLE,
        },
        gui_text = { str = prop.name }
    }
end

-- returns textbox, text
local function property_create_textbox(args)
    local textbox = cg.add {
        transform = { parent = args.prop.container },
        gui = {
            color = cg.color(0.2, 0.2, 0.4, 1),
            valign = cg.GA_MAX,
            halign = cg.GA_TABLE,
        },
        gui_textbox = {},
    }

    local text = cs.gui_textbox.get_text(textbox)
    if args.editable == nil or args.editable or args.numerical then
        cs.gui_textedit.add(text)
        cs.gui_textbox.set_click_focus(textbox, true)
    end
    if args.numerical then cs.gui_textedit.set_numerical(text, true) end

    return textbox, text
end

property_types['boolean'] = {
    add = function (inspector, prop)
        property_create_container(inspector, prop)
        property_create_label(inspector, prop)

        prop.checkbox = cg.add {
            transform = { parent = prop.container },
            gui = {
                color = cg.color(0.2, 0.2, 0.4, 1),
                valign = cg.GA_MAX,
                halign = cg.GA_TABLE,
            },
            gui_checkbox = {},
        }
    end,

    post_update = function (inspector, prop)
        if cs.gui.event_changed(prop.checkbox) then
            cg.set(inspector.sys, prop.name, inspector.ent,
                   cs.gui_checkbox.get_checked(prop.checkbox))
            cs.edit.undo_save()
        else
            local b = cg.get(inspector.sys, prop.name, inspector.ent)
            cs.gui_checkbox.set_checked(prop.checkbox, b)
        end
    end,
}

property_types['string'] = {
    add = function (inspector, prop)
        property_create_container(inspector, prop)
        property_create_label(inspector, prop)

        prop.textbox, prop.textedit
            = property_create_textbox { prop = prop }
    end,

    post_update = function (inspector, prop)
        if cs.gui.event_focus_exit(prop.textedit) then
            cs.edit.undo_save()
        end

        if cs.gui.event_changed(prop.textedit) then
            cg.set(inspector.sys, prop.name, inspector.ent,
                   cs.gui_text.get_str(prop.textedit))
        elseif not cs.gui.get_focus(prop.textedit) then
            local s = cg.get(inspector.sys, prop.name, inspector.ent)
            cs.gui_text.set_str(prop.textedit, s)
        end
    end,
}

property_types['Scalar'] = {
    add = function (inspector, prop)
        property_create_container(inspector, prop)
        property_create_label(inspector, prop)

        prop.textbox, prop.textedit
            = property_create_textbox { prop = prop, numerical = true }
    end,

    post_update = function (inspector, prop)
        if cs.gui.event_focus_exit(prop.textedit) then
            cs.edit.undo_save()
        end

        if cs.gui.event_changed(prop.textedit) then
            cg.set(inspector.sys, prop.name, inspector.ent,
                   cs.gui_textedit.get_num(prop.textedit))
        elseif not cs.gui.get_focus(prop.textedit) then
            local s = cg.get(inspector.sys, prop.name, inspector.ent)
            cs.gui_text.set_str(prop.textedit, string.format('%.4f', s))
        end
    end,
}

-- find values for a given enum type -- memoized
local enum_values_map = {}
local function enum_values(typename)
    if enum_values_map[typename] then return enum_values_map[typename] end
    enum_values_map[typename] = {}
    for v in refct.typeof(typename):values() do
        enum_values_map[typename][v.name] = true
    end
    return enum_values_map[typename]
end
local function enum_tostring(typename, val)
    -- no nice inverse mapping exists...
    for name in pairs(enum_values(typename)) do
        if ffi.new(typename, name) == val then return name end
    end
    return nil
end

property_types['enum'] = {
    add = function (inspector, prop)
        property_create_container(inspector, prop)
        property_create_label(inspector, prop)

        local r = cg.get(inspector.sys, prop.name, inspector.ent)
        prop.enumtype = refct.typeof(r).name

        prop.textbox, prop.text
            = property_create_textbox { prop = prop, editable = false }
    end,

    post_update = function (inspector, prop)
        if cs.gui.event_mouse_down(prop.textbox) == cg.MC_LEFT then
            local function setter(s)
                cg.set(inspector.sys, prop.name, inspector.ent, s)
                cs.edit.undo_save()
            end
            local values = enum_values(prop.enumtype)
            local comp = cs.edit.command_completion_substr(values)
            cs.edit.command_start('set ' .. prop.name .. ': ', setter,
                                  comp, true)
        end

        local s = cg.get(inspector.sys, prop.name, inspector.ent)
        cs.gui_text.set_str(prop.text,
                            enum_tostring(prop.enumtype, s))
    end,
}

property_types['Vec2'] = {
    add = function (inspector, prop)
        property_create_container(inspector, prop)
        property_create_label(inspector, prop)

        prop.x_textbox, prop.x_textedit
            = property_create_textbox { prop = prop, numerical = true }
        prop.y_textbox, prop.y_textedit
            = property_create_textbox { prop = prop, numerical = true }
    end,

    post_update = function (inspector, prop)
        local v = cg.get(inspector.sys, prop.name, inspector.ent)
        local changed = false

        if cs.gui.event_focus_exit(prop.x_textedit) then
            cs.edit.undo_save()
        end
        if cs.gui.event_changed(prop.x_textedit) then
            v.x = cs.gui_textedit.get_num(prop.x_textedit)
            changed = true
        elseif not cs.gui.get_focus(prop.x_textedit) then
            cs.gui_text.set_str(prop.x_textedit, string.format('%.4f', v.x))
        end

        if cs.gui.event_focus_exit(prop.y_textedit) then
            cs.edit.undo_save()
        end
        if cs.gui.event_changed(prop.y_textedit) then
            v.y = cs.gui_textedit.get_num(prop.y_textedit)
            changed = true
        elseif not cs.gui.get_focus(prop.y_textedit) then
            cs.gui_text.set_str(prop.y_textedit, string.format('%.4f', v.y))
        end

        if changed then
            cg.set(inspector.sys, prop.name, inspector.ent, v)
        end
    end,
}

property_types['Color'] = {
    add = function (inspector, prop)
        property_create_container(inspector, prop)
        property_create_label(inspector, prop)

        prop.r_textbox, prop.r_textedit
            = property_create_textbox { prop = prop, numerical = true }
        prop.g_textbox, prop.g_textedit
            = property_create_textbox { prop = prop, numerical = true }
        prop.b_textbox, prop.b_textedit
            = property_create_textbox { prop = prop, numerical = true }
        prop.a_textbox, prop.a_textedit
            = property_create_textbox { prop = prop, numerical = true }
    end,

    post_update = function (inspector, prop)
        local v = cg.get(inspector.sys, prop.name, inspector.ent)
        local changed = false

        if cs.gui.event_focus_exit(prop.r_textedit) then
            cs.edit.undo_save()
        end
        if cs.gui.event_changed(prop.r_textedit) then
            v.r = cs.gui_textedit.get_num(prop.r_textedit)
            changed = true
        elseif not cs.gui.get_focus(prop.r_textedit) then
            cs.gui_text.set_str(prop.r_textedit, string.format('%.4f', v.r))
        end

        if cs.gui.event_focus_exit(prop.g_textedit) then
            cs.edit.undo_save()
        end
        if cs.gui.event_changed(prop.g_textedit) then
            v.g = cs.gui_textedit.get_num(prop.g_textedit)
            changed = true
        elseif not cs.gui.get_focus(prop.g_textedit) then
            cs.gui_text.set_str(prop.g_textedit, string.format('%.4f', v.g))
        end

        if cs.gui.event_focus_exit(prop.b_textedit) then
            cs.edit.undo_save()
        end
        if cs.gui.event_changed(prop.b_textedit) then
            v.b = cs.gui_textedit.get_num(prop.b_textedit)
            changed = true
        elseif not cs.gui.get_focus(prop.b_textedit) then
            cs.gui_text.set_str(prop.b_textedit, string.format('%.4f', v.b))
        end

        if cs.gui.event_focus_exit(prop.a_textedit) then
            cs.edit.undo_save()
        end
        if cs.gui.event_changed(prop.a_textedit) then
            v.a = cs.gui_textedit.get_num(prop.a_textedit)
            changed = true
        elseif not cs.gui.get_focus(prop.a_textedit) then
            cs.gui_text.set_str(prop.a_textedit, string.format('%.4f', v.a))
        end

        if changed then
            cg.set(inspector.sys, prop.name, inspector.ent, v)
        end
    end,
}

property_types['Entity'] = {
    add = function (inspector, prop)
        property_create_container(inspector, prop)
        property_create_label(inspector, prop)

        prop.textbox, prop.text
            = property_create_textbox { prop = prop, editable = false }

        prop.pick = cg.add {
            transform = { parent = prop.container },
            gui = {
                color = cg.color(0.35, 0.15, 0.30, 1),
                valign = cg.GA_MAX,
                halign = cg.GA_TABLE,
            },
            gui_textbox = {},
        }
        cs.gui_text.set_str(cs.gui_textbox.get_text(prop.pick), 'set')
    end,

    post_update = function (inspector, prop)
        -- pick?
        if cs.gui.event_mouse_down(prop.pick) == cg.MC_LEFT then
            local sel = cs.edit.select_get_first()
            cg.set(inspector.sys, prop.name, inspector.ent,
                   sel and sel or cg.entity_nil)
        end

        -- display
        local e = cg.get(inspector.sys, prop.name, inspector.ent)
        if e == cg.entity_nil then
            cs.gui_text.set_str(prop.text, '(nil)')
        else
            cs.gui_text.set_str(prop.text, string.format('[%d]', e.id))

            -- draw line between entities
            if cs.transform.has(inspector.ent)
            and cs.edit.get_editable(e) and cs.transform.has(e) then
                local a = cs.transform.local_to_world(inspector.ent, cg.vec2_zero)
                local b = cs.transform.local_to_world(e,
                                                      cg.vec2_zero)
                cs.edit.line_add(a, b)
            end
        end

        -- select?
        if cs.gui.event_mouse_down(prop.textbox) == cg.MC_LEFT
        and e ~= cg.entity_nil then
            cs.edit.select_clear()
            cs.edit.select[e] = true
        end
    end,
}


--- inspector ------------------------------------------------------------------

cs.edit_inspector = {}

cs.edit_inspector.custom = {} -- custom inspectors -- eg. for physics

local inspectors = cg.entity_table() -- Entity (sys --> inspector) map

cs.edit_inspector.gui_root = cg.add {
    transform = { parent = cs.edit.gui_root },
    group = { groups = 'builtin edit_inspector' },
    edit = { editable = false },
    gui_rect = { },
    gui = {
        captures_events = false,
        color = cg.color(0, 0, 0, 0), -- invisible
        halign = cg.GA_MAX, valign = cg.GA_MAX,
        padding = cg.vec2_zero,
    }
}

local function custom_event(inspector, evt)
    local custom = cs.edit_inspector.custom[inspector.sys]
    if custom then
        local f = cs.edit_inspector.custom[inspector.sys][evt]
        if f then f(inspector) end
    end
end

local function property_type(inspector, name)
    local r = cg.get(inspector.sys, name, inspector.ent)

    local typ = type(r)
    if typ == 'cdata' then
        local refctt = refct.typeof(r)
        if refctt.what == 'enum' then typ = 'enum'
        else typ = refctt.name end
    end
    if typ == 'number' then typ = 'Scalar' end

    return typ

    -- local c = cg.getter(inspector.sys, name)
    -- if type(c) == 'cdata' then
    --     local r = refct.typeof(c).return_type
    -- else
    --     typ = type(c(inspector.ent)) -- can't reflect on Lua, just call
    -- end
end

local function add_property(inspector, name)
    if inspector.props[name] then return end

    -- figure out the type
    local typ = property_type(inspector, name)
    print('property: ', inspector.sys, name, typ)
    if not typ or not property_types[typ] then return end -- type not supported

    inspector.props[name] = {
        typ = typ,
        name = name,
    }
    property_types[typ].add(inspector, inspector.props[name])
end

local function make_inspector(ent, sys)
    local inspector = {}

    inspector.ent = cg.Entity(ent)
    inspector.sys = sys

    inspector.window = cg.add {
        transform = { parent = cs.edit_inspector.gui_root },
        gui_window = {},
        gui = {
            valign = cg.GA_TABLE,
            halign = cg.GA_MAX,
        }
    }
    inspector.window_body = cs.gui_window.get_body(inspector.window)

    inspector.props = {}
    if cs.meta.props[inspector.sys] then
        for _, p in ipairs(cs.meta.props[inspector.sys]) do
            add_property(inspector, p.name)
        end
    elseif rawget(cs, inspector.sys) then
        for f, _ in pairs(cs[inspector.sys]) do
            if string.sub(f, 1, 4) == 'set_' then
                add_property(inspector, string.sub(f, 5, string.len(f)))
            end
        end
    end

    custom_event(inspector, 'add')
    return inspector
end

function cs.edit_inspector.add(ent, sys)
    local adder = cs[sys].add
    if not adder then error("system '" .. sys .. "' has no 'add(...)' function") end

    if not inspectors[ent] then
        inspectors[ent] = {}
    end

    if inspectors[ent][sys] then return end
    adder(ent)
    inspectors[ent][sys] = make_inspector(ent, sys)
end

function cs.edit_inspector.remove(ent, sys)
    if not inspectors[ent] then return end

    if not sys then
        for sys in pairs(inspectors[ent]) do
            cs.edit_inspector.remove(ent, sys)
        end
        inspectors[ent] = nil
        return
    end

    custom_event(inspectors[ent][sys], 'remove')
    cs.gui_window.remove(inspectors[ent][sys].window)
    inspectors[ent][sys] = nil
end

-- return set of all valid inspector systems
function cs.edit_inspector.get_systems()
    local sys = {}
    -- system must either have property metadata or an 'add(...)' function
    for k, _ in pairs(cs.meta.props) do sys[k] = true end
    for k, _ in pairs(cs) do
        if cs[k].add then sys[k] = true end
    end
    return sys
end

local function set_group_rec(ent)
    cs.edit.set_editable(ent, false)
    cs.group.set_groups(ent, 'builtin edit_inspector')

    if cs.transform.has(ent) then
        local children = cs.transform.get_children(ent)
        for i = 0, cs.transform.get_num_children(ent) - 1 do
            set_group_rec(children[i])
        end
    end
end

local function remove_destroyed()
    for ent, insps in pairs(inspectors) do
        if cs.entity.destroyed(ent) then
            cs.edit_inspector.remove(ent)
        else
            for _, inspector in pairs(insps) do
                if cs.entity.destroyed(inspector.window) then
                    cs.edit_inspector.remove(inspector.ent, inspector.sys)
                    return
                end
            end
        end
    end
end

local function update_inspector(inspector)
    cs.transform.set_parent(inspector.window, cs.edit_inspector.gui_root)

    cs.gui_window.set_highlight(inspector.window,
                                cs.edit.select[inspector.ent])
    local title = inspector.sys
    cs.gui_window.set_title(inspector.window, title)

    -- make everything uneditable/unsaveable etc.
    set_group_rec(inspector.window)

    for _, prop in pairs(inspector.props) do
        local f = property_types[prop.typ].update
        if f then f(inspector, prop) end
    end
    custom_event(inspector, 'update')
end

function cs.edit_inspector.update_all()
    remove_destroyed()
    if not cs.edit.get_enabled() then return end

    for _, insps in pairs(inspectors) do
        for _, inspector in pairs(insps) do
            update_inspector(inspector)
        end
    end
end

local function post_update_inspector(inspector)
    -- draw line from inspector to target entity
    if cs.transform.has(inspector.ent) then
        local a = cs.transform.local_to_world(inspector.window,
                                              cg.vec2(0, -16))
        local b = cs.transform.local_to_world(inspector.ent,
                                              cg.vec2_zero)
        cs.edit.line_add(a, b)
    end

    for _, prop in pairs(inspector.props) do
        property_types[prop.typ].post_update(inspector, prop)
    end
    custom_event(inspector, 'post_update')
end

function cs.edit_inspector.post_update_all()
    remove_destroyed()
    if not cs.edit.get_enabled() then return end

    for _, insps in pairs(inspectors) do
        for _, inspector in pairs(insps) do
            post_update_inspector(inspector)
        end
    end
end

function cs.edit_inspector.save_all()
    local data = {}

    if cs.entity.get_save_filter(cs.edit_inspector.gui_root) then
        data.gui_root = cs.edit_inspector.gui_root
    end

    data.tbl = cg.entity_table()
    for _, insps in pairs(inspectors) do
        for _, inspector in pairs(insps) do
            data.tbl[inspector.window] = inspector
        end
    end

    return data
end
function cs.edit_inspector.load_all(data)
    if data.gui_root then
        cs.edit_inspector.gui_root = data.gui_root
    end

    for win, inspector in pairs(data.tbl) do
        if not inspectors[inspector.ent] then
            inspectors[inspector.ent] = {}
        end
        inspectors[inspector.ent][inspector.sys] = inspector
    end
end


--- C system properties --------------------------------------------------------

cs.meta.props['transform'] = {
    { name = 'parent' },
    { name = 'position' },
    { name = 'rotation' },
    { name = 'scale' },
}

cs.meta.props['camera'] = {
    { name = 'viewport_height' },
}

cs.meta.props['sprite'] = {
    { name = 'cell' },
    { name = 'size' },
    { name = 'depth' },
}

cs.meta.props['physics'] = {
    { name = 'type' },
    { name = 'mass' },
    --{ name = 'position' },
    { name = 'velocity' },
    { name = 'freeze_rotation' },
}

cs.edit_inspector.custom['physics'] = {
    add = function (inspector)
        -- 'add box' button
        inspector.add_box = cg.add {
            transform = { parent = inspector.window_body },
            gui = {
                color = cg.color(0.35, 0.15, 0.30, 1),
                valign = cg.GA_TABLE,
                halign = cg.GA_MIN,
            },
            gui_textbox = {},
        }
        cs.gui_text.set_str(cs.gui_textbox.get_text(inspector.add_box),
                            'add box')
    end,

    post_update = function (inspector)
        cs.physics.debug_draw(inspector.ent)

        -- 'add box' button
        if cs.gui.event_mouse_down(inspector.add_box) == cg.MC_LEFT then
            local bbox
            if cs.edit.bboxes_has(inspector.ent) then
                bbox = cs.edit.bboxes_get(inspector.ent)
            else
                bbox = cg.bbox(cg.vec2(-1, -1), cg.vec2(1, 1))
            end
            cs.physics.shape_add_box(inspector.ent, bbox)
            cs.edit.undo_save()
        end
    end,
}

cs.meta.props['gui'] = {
    { name = 'color' },
    { name = 'visible' },
    { name = 'focusable' },
    { name = 'captures_events' },
    { name = 'halign' },
    { name = 'valign' },
    { name = 'padding' },
}
cs.meta.props['gui_rect'] = {
    { name = 'size' },
    { name = 'hfit' },
    { name = 'vfit' },
    { name = 'hfill' },
    { name = 'vfill' },
}
cs.meta.props['gui_text'] = {
    { name = 'str' },
}
cs.meta.props['gui_textedit'] = {
    { name = 'cursor' },
    { name = 'numerical' },
}
