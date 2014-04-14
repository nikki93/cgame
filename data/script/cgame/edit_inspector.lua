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

-- returns textfield, textedit
local function property_create_textfield(inspector, prop, numerical)
    local textfield = cg.add {
        transform = { parent = prop.container },
        gui = {
            color = cg.color(0.2, 0.2, 0.4, 1),
            valign = cg.GA_MAX,
            halign = cg.GA_TABLE,
        },
        textfield = {},
    }
    local textedit = cs.textfield.get_textedit(textfield)
    if numerical then cs.gui_textedit.set_numerical(textedit, true) end
    return textfield, textedit
end

property_types['Scalar'] = {
    create_view = function (inspector, prop)
        property_create_container(inspector, prop)
        property_create_label(inspector, prop)
        
        prop.textfield, prop.textedit
            = property_create_textfield(inspector, prop)
    end,

    update_view = function (inspector, prop)
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

        prop.x_textfield, prop.x_textedit
            = property_create_textfield(inspector, prop)
        prop.y_textfield, prop.y_textedit
            = property_create_textfield(inspector, prop)
    end,

    update_view = function (inspector, prop)
        local v = cg.get(inspector.sys, prop.name, inspector.ent)
        local changed = false

        if cs.gui.event_changed(prop.x_textedit) then
            v.x = cs.gui_textedit.get_num(prop.x_textedit)
            changed = true
        elseif not cs.gui.get_focus(prop.x_textedit) then
            cs.gui_text.set_str(prop.x_textedit, string.format('%.4f', v.x))
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


--- inspector ------------------------------------------------------------------

cs.edit_inspector = {}

local inspectors = cg.entity_table() -- Entity (sys --> inspector) map

local function add_property(inspector, typ, name)
    if inspector.props[name] then return end

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
    add_property(inspector, 'Vec2', 'position')
    add_property(inspector, 'Scalar', 'rotation')
    
    return inspector
end

function cs.edit_inspector.add(ent, sys)
    if not inspectors[ent] then
        inspectors[ent] = {}
    end
    
    if inspectors[ent][sys] then return end
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

local function set_group_rec(ent)
    cs.edit.set_editable(ent, false)
    cs.group.set_groups(ent, 'builtin')

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
