local refct = require 'reflect'

--- C system properties --------------------------------------------------------

cs.meta.props['transform'] = {
    { name = 'parent' },
    { name = 'position' },
    { name = 'rotation' },
    { name = 'scale' },
}

cs.meta.props['sprite'] = {
    { name = 'cell' },
    { name = 'size' },
    { name = 'depth' },
}

cs.meta.props['camera'] = {
    { name = 'viewport_height' },
}

cs.meta.props['physics'] = {
    { name = 'mass' },
    { name = 'position' },
    { name = 'velocity' },
    { name = 'velocity' },
}


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

property_types['Scalar'] = {
    create_view = function (inspector, prop)
        property_create_container(inspector, prop)
        property_create_label(inspector, prop)

        prop.textbox, prop.textedit
            = property_create_textbox { prop = prop, numerical = true }
    end,

    update_view = function (inspector, prop)
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

property_types['Vec2'] = {
    create_view = function (inspector, prop)
        property_create_container(inspector, prop)
        property_create_label(inspector, prop)

        prop.x_textbox, prop.x_textedit
            = property_create_textbox { prop = prop, numerical = true }
        prop.y_textbox, prop.y_textedit
            = property_create_textbox { prop = prop, numerical = true }
    end,

    update_view = function (inspector, prop)
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

property_types['Entity'] = {
    create_view = function (inspector, prop)
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

    update_view = function (inspector, prop)
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

local inspectors = cg.entity_table() -- Entity (sys --> inspector) map

local function property_type(inspector, name)
    local r = cg.get(inspector.sys, name, inspector.ent)

    local typ = type(r)
    if typ == 'cdata' then typ = refct.typeof(r).name end
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
    property_types[typ].create_view(inspector, inspector.props[name])
end

local function make_inspector(ent, sys)
    local inspector = {}

    inspector.ent = cg.Entity(ent)
    inspector.sys = sys

    inspector.window = cg.add {
        transform = { parent = cs.edit.gui_root },
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

local function update_inspector(inspector)
    if cs.entity.destroyed(inspector.window) then
        cs.edit_inspector.remove(inspector.ent, inspector.sys)
        return
    end

    cs.transform.set_parent(inspector.window, cs.edit.gui_root)

    cs.gui_window.set_highlight(inspector.window,
                                cs.edit.select[inspector.ent])
    local title = inspector.sys .. ' ' .. inspector.ent.id
    cs.gui_window.set_title(inspector.window, title)

    -- make everything uneditable/unsaveable etc.
    set_group_rec(inspector.window)

    -- update property views
    for _, prop in pairs(inspector.props) do
        property_types[prop.typ].update_view(inspector, prop)
    end
end

function cs.edit_inspector.update_all()
    for ent, _ in pairs(inspectors) do
        if cs.entity.destroyed(ent) then cs.edit_inspector.remove(ent) end
    end

    if not cs.edit.get_enabled() then return end

    for _, insps in pairs(inspectors) do
        for _, inspector in pairs(insps) do
            update_inspector(inspector)
        end
    end
end

function cs.edit_inspector.save_all()
    -- save window -> inspector entity table
    local d = cg.entity_table()
    for _, insps in pairs(inspectors) do
        for _, inspector in pairs(insps) do
            d[inspector.window] = inspector
        end
    end
    return d
end
function cs.edit_inspector.load_all(d)
    for win, inspector in pairs(d) do
        if not inspectors[inspector.ent] then
            inspectors[inspector.ent] = {}
        end
        inspectors[inspector.ent][inspector.sys] = inspector
    end
end
