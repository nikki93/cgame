cs.edit_inspector = {}

-- Entity -> (sys -> inspector data) map
local inspectors = cg.entity_table()

local property_create_view = {
    Scalar = function (inspector, prop)
        prop.view = cg.add {
            transform = {
                parent = cs.gui_window.get_body(inspector.window)
            },
            gui = {
                color = cg.color_white,
                valign = cg.GA_TABLE,
                halign = cg.GA_MIN,
            },
            gui_textedit = {},
            gui_text = { str = prop.name .. ': <default>' }
        }
    end,

    Vec2 = function (inspector, prop)
        prop.view = cg.add {
            transform = {
                parent = cs.gui_window.get_body(inspector.window)
            },
            gui = {
                color = cg.color_white,
                valign = cg.GA_TABLE,
                halign = cg.GA_MIN,
            },
            gui_textedit = {},
            gui_text = { str = prop.name .. ': <default>' }
        }
    end,
}

local property_update_view = {
    Scalar = function (inspector, prop)
        if not cs.gui.get_focus(prop.view) then
            local s = cg.get(inspector.sys, prop.name, inspector.ent)
            cs.gui_text.set_str(prop.view, prop.name .. ': '
                                    .. string.format('%.2f', s))
        end
    end,

    Vec2 = function (inspector, prop)
        if not cs.gui.get_focus(prop.view) then
            local v = cg.get(inspector.sys, prop.name, inspector.ent)
            cs.gui_text.set_str(prop.view, prop.name .. ': '
                                    .. string.format('%.2f, %.2f', v.x, v.y))
        end
    end,
}

local function add_property(inspector, typ, name)
    if inspector.props[name] then return end

    inspector.props[name] = {
        typ = typ,
        name = name,
    }
    property_create_view[typ](inspector, inspector.props[name])
end

local function make_inspector(ent, sys)
    local inspector = {}

    inspector.ent = cg.Entity(ent)
    inspector.sys = sys

    inspector.window = cg.add {
        transform = { parent = cs.edit.gui_root },
        gui_window = { title = sys .. ' ' .. ent.id },
        gui = {
            valign = cg.GA_TABLE,
            halign = cg.GA_MAX,
        }
    }
    
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
        inspectors[inspector.ent][inspector.sys] = nil
        return
    end
    
    -- make everything uneditable/unsaveable etc.
    set_group_rec(inspector.window)

    -- update property views
    for _, prop in pairs(inspector.props) do
        property_update_view[prop.typ](inspector, prop)
    end
end

function cs.edit_inspector.update_all()
    for ent, _ in pairs(inspectors) do
        if cs.entity.destroyed(ent) then cs.edit_inspector.remove(ent) end
    end

    for _, insps in pairs(inspectors) do
        for _, inspector in pairs(insps) do
            update_inspector(inspector)
        end
    end
end

-- function cs.edit_inspector.save_all()
--     return { tbl = inspectors }
-- end
-- function cs.edit_inspector.load_all(d)
--     cg.entity_table_merge(inspectors, d.tbl)
-- end
