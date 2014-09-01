local ffi = require 'ffi'
local refct = require 'reflect'

--- edit_field -----------------------------------------------------------------

local field_types = {}

-- edit_field makes it easy to provide editors for simple properties of various
-- types (Vec2, Scalar etc.)

-- 'args' may contain:
--      field_type: the field type (boolean, string, Scalar, enum, Vec2,
--                                  Color, Entity)
--      ctype: the name of the underlying C type, if any
--      parent: the parent GUI entity
--      label: descriptive label (optional)
--      halign, valign: alignment for container (optional)
function cg.edit_field_create(args)
    assert(args.field_type, 'field type expected')
    local field = field_types[args.field_type].create(args)
    field.type = args.field_type
    return field
end

-- updates display to val
function cg.edit_field_update(field, val)
    local f = field_types[field.type].update
    if f then f(field, val) end
end

-- updates display to val, calls setter with new value if edited
function cg.edit_field_post_update(field, ...)
    local f = field_types[field.type].post_update
    if f then f(field, unpack({...})) end
end

-- returns textbox, text
local function field_create_textbox(args)
    local textbox = cg.add {
        transform = { parent = args.field.container },
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

local function field_create_common(args)
    local field = {}

    field.container = cg.add {
        transform = { parent = args.parent },
        gui = {
            padding = cg.vec2_zero,
            color = cg.color_clear,
            valign = args.valign or cg.GA_TABLE,
            halign = args.halign or cg.GA_MIN,
        },
        gui_rect = { hfill = true },
    }

    if args.label then
        field.name = args.label
        field.label = cg.add {
            transform = { parent = field.container },
            gui = {
                color = cg.color_white,
                valign = cg.GA_MID,
                halign = cg.GA_TABLE,
            },
            gui_text = { str = args.label }
        }
    end

    return field
end

field_types['boolean'] = {
    create = function (args)
        local field = field_create_common(args)
        field.checkbox = cg.add {
            transform = { parent = field.container },
            gui = {
                color = cg.color(0.2, 0.2, 0.4, 1),
                valign = cg.GA_MAX,
                halign = cg.GA_TABLE,
            },
            gui_checkbox = {},
        }
        return field
    end,

    post_update = function (field, val, setter)
        if cs.gui.event_changed(field.checkbox) then
            setter(cs.gui_checkbox.get_checked(field.checkbox))
            cs.edit.undo_save()
        else
            cs.gui_checkbox.set_checked(field.checkbox, val)
        end
    end
}

field_types['string'] = {
    create = function (args)
        local field = field_create_common(args)
        field.textbox, field.textedit = field_create_textbox { field = field }
        return field
    end,

    post_update = function (field, val, setter)
        if cs.gui.event_focus_exit(field.textedit) then cs.edit.undo_save() end
        if cs.gui.event_changed(field.textedit) then
            setter(cs.gui_text.get_str(field.textedit))
        elseif not cs.gui.get_focus(field.textedit) then
            cs.gui_text.set_str(field.textedit, val)
        end
    end,
}

field_types['Scalar'] = {
    create = function (args)
        local field = field_create_common(args)
        field.textbox, field.textedit =
            field_create_textbox { field = field, numerical = true }
        return field
    end,

    post_update = function (field, val, setter)
        if cs.gui.event_focus_exit(field.textedit) then cs.edit.undo_save() end
        if cs.gui.event_changed(field.textedit) then
            setter(cs.gui_textedit.get_num(field.textedit))
        elseif not cs.gui.get_focus(field.textedit) then
            cs.gui_text.set_str(field.textedit, string.format('%.4f', val))
        end
    end
}

-- if it's a C enum field the C enum values are automatically used, else a
-- set of values must be provided as an extra parameter to
-- cg.edit_field_post_update(...)
field_types['enum'] = {
    create = function (args)
        local field = field_create_common(args)
        field.enumtype = args.ctype
        field.textbox, field.text =
            field_create_textbox { field = field, editable = false }
        return field
    end,

    post_update = function (field, val, setter, vals)
        if cs.gui.event_mouse_down(field.textbox) == cg.MC_LEFT then
            local function setter_wrap(s)
                setter(s)
                cs.edit.undo_save()
            end
            vals = field.enumtype and cg.enum_values(field.enumtype) or vals
            local comp = cs.edit.command_completion_substr(vals)
            local prompt = 'set ' .. (field.name or '') .. ': '
            cs.edit.command_start(prompt, setter_wrap, comp, true)
        end
        val = field.enumtype and cg.enum_tostring(field.enumtype, val) or val
        cs.gui_text.set_str(field.text, val)
    end,
}

field_types['Vec2'] = {
    create = function (args)
        local field = field_create_common(args)
        field.x_field = cg.edit_field_create {
            field_type = 'Scalar', ctype = 'Scalar',
            parent = field.container, valign = cg.GA_MAX, halign = cg.GA_TABLE
        }
        field.y_field = cg.edit_field_create {
            field_type = 'Scalar', ctype = 'Scalar',
            parent = field.container, valign = cg.GA_MAX, halign = cg.GA_TABLE
        }
        return field
    end,

    post_update = function (field, val, setter)
        cg.edit_field_post_update(field.x_field, val.x,
                                  function (x) setter(cg.vec2(x, val.y)) end)
        cg.edit_field_post_update(field.y_field, val.y,
                                  function (y) setter(cg.vec2(val.x, y)) end)
    end,
}

field_types['Color'] = {
    create = function (args)
        local field = field_create_common(args)
        field.r_field = cg.edit_field_create {
            field_type = 'Scalar', ctype = 'Scalar',
            parent = field.container, valign = cg.GA_MAX, halign = cg.GA_TABLE
        }
        field.g_field = cg.edit_field_create {
            field_type = 'Scalar', ctype = 'Scalar',
            parent = field.container, valign = cg.GA_MAX, halign = cg.GA_TABLE
        }
        field.b_field = cg.edit_field_create {
            field_type = 'Scalar', ctype = 'Scalar',
            parent = field.container, valign = cg.GA_MAX, halign = cg.GA_TABLE
        }
        field.a_field = cg.edit_field_create {
            field_type = 'Scalar', ctype = 'Scalar',
            parent = field.container, valign = cg.GA_MAX, halign = cg.GA_TABLE
        }
        return field
    end,

    post_update = function (field, val, setter)
        cg.edit_field_post_update(
            field.r_field, val.r,
            function (r) setter(cg.color(r, val.g, val.b, val.a)) end)
        cg.edit_field_post_update(
            field.g_field, val.g,
            function (g) setter(cg.color(val.r, g, val.b, val.a)) end)
        cg.edit_field_post_update(
            field.b_field, val.b,
            function (b) setter(cg.color(val.r, val.g, b, val.a)) end)
        cg.edit_field_post_update(
            field.a_field, val.a,
            function (a) setter(cg.color(val.r, val.g, val.b, a)) end)
    end,
}

field_types['Entity'] = {
    create = function (args)
        local field = field_create_common(args)
        field.enumtype = args.ctype
        field.textbox, field.text =
            field_create_textbox { field = field, editable = false }

        -- 'set' button
        field.pick = cg.add {
            transform = { parent = field.container },
            gui = {
                color = cg.color(0.35, 0.15, 0.30, 1),
                valign = cg.GA_MAX,
                halign = cg.GA_TABLE,
            },
            gui_textbox = {},
        }
        cs.gui_text.set_str(cs.gui_textbox.get_text(field.pick), 'set')

        return field
    end,

    post_update = function (field, val, setter)
        -- pick new value?
        if cs.gui.event_mouse_down(field.pick) == cg.MC_LEFT then
            val = cs.edit.select_get_first() or cg.entity_nil
            setter(val)
            cs.edit.undo_save()
        end

        -- display, select on click
        cs.gui_text.set_str(field.text, val == cg.entity_nil and '(nil)'
                                or string.format('[%d]', val.id))
        if cs.gui.event_mouse_down(field.textbox) == cg.MC_LEFT
        and val ~= cg.entity_nil then
            cs.edit.select_clear()
            cs.edit.select[val] = true
        end
    end
}

field_types['BBox'] = {
    create = function (args)
        local field = field_create_common(args)
        field.min_field = cg.edit_field_create {
            field_type = 'Vec2', ctype = 'Vec2',
            parent = field.container, valign = cg.GA_MAX, halign = cg.GA_TABLE
        }
        field.max_field = cg.edit_field_create {
            field_type = 'Vec2', ctype = 'Vec2',
            parent = field.container, valign = cg.GA_MAX, halign = cg.GA_TABLE
        }
        return field
    end,

    post_update = function (field, val, setter)
        cg.edit_field_post_update(field.min_field, val.min,
                                  function (m) setter(cg.bbox(m, val.max)) end)
        cg.edit_field_post_update(field.max_field, val.max,
                                  function (m) setter(cg.bbox(val.min, m)) end)
    end,
}

--- inspector ------------------------------------------------------------------

cs.edit_inspector = { inspect = false }

cs.edit_inspector.custom = {} -- custom inspectors -- eg. for physics

local inspectors = cg.entity_table() -- Entity (sys --> inspector) map

cs.edit_inspector.gui_root = cg.add {
    transform = { parent = cs.edit.gui_root },
    group = { groups = 'builtin edit_inspector' },
    edit = { editable = false },
    gui_rect = { hfill = true, vfill = true },
    gui = {
        captures_events = false,
        color = cg.color(0, 0, 0, 0), -- invisible
        halign = cg.GA_MIN, valign = cg.GA_MAX,
        padding = cg.vec2_zero,
    }
}

-- forward event to custom event handler
local function custom_event(inspector, evt)
    local custom = cs.edit_inspector.custom[inspector.sys]
    if custom then
        local f = cs.edit_inspector.custom[inspector.sys][evt]
        if f then f(inspector) end
    end
end

-- returns field type, C type
local function property_type(inspector, name)
    local r = cg.get(inspector.sys, name, inspector.ent)

    local field_type = type(r)
    local ctype = nil
    if field_type == 'cdata' then
        local refctt = refct.typeof(r)
        ctype = refctt.name
        if refctt.what == 'enum' then field_type = 'enum'
        else field_type = ctype end
    end
    if field_type == 'number' then field_type = 'Scalar' end

    return field_type, ctype
end

-- add field for property
local function add_property(inspector, name)
    if inspector.props[name] then return end -- already exists

    local field_type, ctype = property_type(inspector, name)
    if not field_type or not field_types[field_type] then return end

    inspector.props[name] = {
        name = name,
        field = cg.edit_field_create {
            field_type = field_type,
            ctype = ctype,
            parent = inspector.window_body,
            label = name,
        },
    }
end

-- add all properties for an inspector, either through cs.meta.props or
-- through automatic discovery
local function add_properties(inspector)
    if cs.meta.props[inspector.sys] then
        for _, p in ipairs(cs.meta.props[inspector.sys]) do
            add_property(inspector, p.name)
        end
    elseif rawget(cs, inspector.sys) then
        for f in pairs(cs[inspector.sys]) do
            if string.sub(f, 1, 4) == 'set_' then
                local prop = string.sub(f, 5, string.len(f))
                if cs[inspector.sys]['get_' .. prop] then
                    add_property(inspector, prop)
                end
            end
        end
    end
end

-- return inspector object
local function make_inspector(ent, sys)
    local inspector = {}

    inspector.ent = cg.Entity(ent)
    inspector.sys = sys

    -- put near entity initially
    local pos = cg.vec2(16, -16)
    if cs.transform.has(ent) then
        pos = cs.transform.local_to_world(ent, cg.vec2_zero)
        pos = cs.transform.world_to_local(cs.edit_inspector.gui_root, pos)
            + cg.vec2(22, -10)
    end

    -- window
    inspector.window = cg.add {
        transform = {
            parent = cs.edit_inspector.gui_root,
            position = pos
        },
        gui_window = {},
        gui = {}
    }
    inspector.last_pos = pos
    inspector.window_body = cs.gui_window.get_body(inspector.window)
    inspector.docked = false

    -- dock toggle button
    inspector.dock_text = cg.add {
        transform = {
            parent = cs.gui_window.get_title_buttons_area(inspector.window)
        },
        gui = {
            color = cg.color_white,
            valign = cg.GA_MAX,
            halign = cg.GA_TABLE
        },
        gui_text = { str = '<' },
    }

    -- 'remove' button
    inspector.remove_text = cg.add {
        transform = {
            parent = cs.gui_window.get_title_buttons_area(inspector.window)
        },
        gui = {
            color = cg.color_red,
            valign = cg.GA_MAX,
            halign = cg.GA_TABLE,
        },
        gui_text = { str = 'r' },
    }

    -- property fields
    inspector.props = {}
    add_properties(inspector)

    custom_event(inspector, 'add')
    return inspector
end

-- add a sys inspector for Entity ent
function cs.edit_inspector.add(ent, sys)
    local adder = cs[sys].add
    if not adder then
        error("system '" .. sys .. "' has no 'add(...)' function")
    end

    if not inspectors[ent] then
        inspectors[ent] = {}
    end

    if inspectors[ent][sys] then return end
    if not cs[sys].has(ent) then adder(ent) end
    inspectors[ent][sys] = make_inspector(ent, sys)
end

-- remove sys inspector for Entity ent -- sys is optional, removes all
-- inspectors on ent if not specified
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
    for k in pairs(cs.meta.props) do sys[k] = true end
    for k in pairs(cs) do
        if cs[k].inspect ~= false and cs[k].add then
            sys[k] = true
        end
    end
    return sys
end

local function remove_destroyed()
    -- if closed a window, save an undo point
    local some_closed = false

    for ent, insps in pairs(inspectors) do
        if cs.entity.destroyed(ent) then
            cs.edit_inspector.remove(ent)
        else
            for _, inspector in pairs(insps) do
                if cs.gui.event_mouse_down(inspector.remove_text)
                == cg.MC_LEFT then
                    cs[inspector.sys].remove(inspector.ent)
                    cs.edit_inspector.remove(inspector.ent, inspector.sys)
                    some_closed = true
                elseif cs.entity.destroyed(inspector.window)
                or not cs[inspector.sys].has(ent) then
                    cs.edit_inspector.remove(inspector.ent, inspector.sys)
                    some_closed = true
                end
            end
        end
    end

    if some_closed then cs.edit.undo_save() end
end

-- make entities uneditable/unsaveable etc. recursively
local function update_group_editable_rec(ent)
    cs.edit.set_editable(ent, false)
    cs.group.set_groups(ent, 'builtin edit_inspector')

    if cs.transform.has(ent) then
        local children = cs.transform.get_children(ent)
        for i = 0, cs.transform.get_num_children(ent) - 1 do
            update_group_editable_rec(children[i])
        end
    end
end

local function update_inspector(inspector)
    cs.transform.set_parent(inspector.window, cs.edit_inspector.gui_root)

    cs.gui_window.set_highlight(inspector.window,
                                cs.edit.select[inspector.ent])
    local title = inspector.sys
    cs.gui_window.set_title(inspector.window, title)

    -- docking
    if cs.gui.event_mouse_down(inspector.dock_text) == cg.MC_LEFT then
        inspector.docked = not inspector.docked
        if not inspector.docked then
            cs.transform.set_position(inspector.window, inspector.last_pos)
        end
    end
    if inspector.docked then
        cs.gui.set_halign(inspector.window, cg.GA_MAX)
        cs.gui.set_valign(inspector.window, cg.GA_TABLE)
        cs.gui_text.set_str(inspector.dock_text, '<')
    else
        inspector.last_pos = cs.transform.get_position(inspector.window)
        cs.gui.set_halign(inspector.window, cg.GA_NONE)
        cs.gui.set_valign(inspector.window, cg.GA_NONE)
        cs.gui_text.set_str(inspector.dock_text, '>')
    end

    add_properties(inspector) -- capture newly added properties

    update_group_editable_rec(inspector.window)

    for _, prop in pairs(inspector.props) do
        cg.edit_field_update(prop.field,
                             cg.get(inspector.sys, prop.name, inspector.ent))
    end
    custom_event(inspector, 'update')
end

function cs.edit_inspector.update_all()
    cs.transform.set_parent(cs.edit_inspector.gui_root, cs.edit.gui_root)
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
        cs.edit.line_add(a, b, 0, cg.color(1, 0, 1, 0.6))
    end

    for _, prop in pairs(inspector.props) do
        cg.edit_field_post_update(
            prop.field,
            cg.get(inspector.sys, prop.name, inspector.ent),
            function (val) cg.set(inspector.sys, prop.name,
                                  inspector.ent, val) end)
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
            if cs.entity.get_save_filter(inspector.ent) then
                data.tbl[inspector.window] = inspector
            end
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
    { name = 'current' },
    { name = 'viewport_height' },
}

cs.meta.props['sprite'] = {
    { name = 'size' },
    { name = 'texcell' },
    { name = 'texsize' },
    { name = 'depth' },
}

cs.meta.props['physics'] = {
    { name = 'type' },
    { name = 'mass' },
    { name = 'freeze_rotation' },
    { name = 'velocity' },
    { name = 'force' },
    { name = 'angular_velocity' },
    { name = 'torque' },
    { name = 'velocity_limit' },
    { name = 'angular_velocity_limit' },
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

cs.meta.props['sound'] = {
    { name = 'path' },
    { name = 'playing' },
    { name = 'seek' },
    { name = 'finish_destroy' },
    { name = 'loop' },
    { name = 'gain' },
}
