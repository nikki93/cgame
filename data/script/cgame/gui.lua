local root = cs.gui.get_root()
cs.group.set_groups(root, 'builtin')

cg.wrap_string('gui_text', 'str')


--- event ----------------------------------------------------------------------

cs.gui_event = {}

local event_handlers = cg.entity_table()
local event_defaults = {}

function cs.gui_event.add()
end

local function add_event(event, default)
    event_defaults[event] = default

    cs.gui_event['set_' .. event] = function (ent, f)
        if not event_handlers[ent] then
            event_handlers[ent] = {}
        end
        event_handlers[ent][event] = f
    end
end

add_event('focus_enter', false)
add_event('focus_exit', false)
add_event('mouse_down', cg.MC_NONE)
add_event('mouse_up', cg.MC_NONE)
add_event('key_down', cg.KC_NONE)
add_event('key_up', cg.KC_NONE)

function cs.gui_event.update_all()
    for ent in pairs(event_handlers) do
        if cs.entity.destroyed(ent) then event_handlers[ent] = nil end
    end

    for ent, handlers in pairs(event_handlers) do
        for event, f in pairs(handlers) do
            local r = cs.gui['event_' .. event](ent)
            if r ~= event_defaults[event] then
                f(ent, r)
            end
        end
    end
end


--- window ---------------------------------------------------------------------

cs.gui_window = { auto_saveload = true }

cs.gui_window.tbl = cg.entity_table()

function cs.gui_window.add(ent)
    if cs.gui_window.tbl[ent] then return end
    cs.gui_window.tbl[ent] = {}
    local window = cs.gui_window.tbl[ent]

    -- add ent to gui_rect as container
    cg.add {
        ent = ent,
        gui_rect = {},
        gui = { color = cg.color(0.3, 0.3, 0.5, 0.95) },
    }

    -- titlebar containing text, minimize button
    window.titlebar = cg.add {
        transform = { parent = ent },
        gui_rect = { hfill = true },
        gui = {
            padding = cg.vec2_zero,
            color = cg.color(0.15, 0.15, 0.35, 0.95),
            valign = cg.GA_TABLE,
            halign = cg.GA_MIN,
        },
    }
    window.title_buttons_area = cg.add {
        transform = { parent = window.titlebar },
        gui_rect = {},
        gui = {
            padding = cg.vec2_zero,
            color = cg.color(0.0, 0.0, 0.0, 0.0),
            valign = cg.GA_MAX,
            halign = cg.GA_TABLE,
        },
    }
    window.close_text = cg.add {
        transform = { parent = window.title_buttons_area },
        gui = {
            color = cg.color_white,
            valign = cg.GA_MAX,
            halign = cg.GA_TABLE,
        },
        gui_text = { str = 'x' },
    }
    window.minmax_text = cg.add {
        transform = { parent = window.title_buttons_area },
        gui = {
            color = cg.color_white,
            valign = cg.GA_MAX,
            halign = cg.GA_TABLE,
        },
        gui_text = { str = '-' },
    }
    window.title_text_area = cg.add {
        transform = { parent = window.titlebar },
        gui_rect = { hfill = true },
        gui = {
            padding = cg.vec2_zero,
            color = cg.color(0.0, 0.0, 0.0, 0.0),
            valign = cg.GA_MAX,
            halign = cg.GA_TABLE,
        },
    }
    window.title_text = cg.add {
        transform = { parent = window.title_text_area },
        gui = {
            color = cg.color_white,
            valign = cg.GA_MAX,
            halign = cg.GA_MID,
        },
        gui_text = { str = 'new window' },
    }

    -- body containing contents
    window.body = cg.add {
        transform = { parent = ent },
        gui_rect = {},
        gui = {
            padding = cg.vec2_zero,
            color = cg.color(0.0, 0.0, 0.0, 0.0),
            valign = cg.GA_TABLE,
            halign = cg.GA_MIN
        },
    }
end

function cs.gui_window.remove(ent)
    local window = cs.gui_window.tbl[ent]
    if window then cs.transform.destroy_rec(ent) end
    cs.gui_window.tbl[ent] = nil
end

function cs.gui_window.has(ent)
    return cs.gui_window.tbl[ent] ~= nil
end

cg.simple_props(cs.gui_window, {
    minimized = false,
    closeable = true,
    highlight = false
})

function cs.gui_window.set_title(ent, str)
    local window = cs.gui_window.tbl[ent]
    if window then cs.gui_text.set_str(window.title_text, str) end
end
function cs.gui_window.get_title(ent)
    local window = cs.gui_window.tbl[ent]
    if window then return cs.gui_text.get_str(window.title_text) end
end
function cs.gui_window.get_title_buttons_area(ent)
    local window = cs.gui_window.tbl[ent]
    if window then return window.title_buttons_area end
end

function cs.gui_window.get_body(ent)
    local window = cs.gui_window.tbl[ent]
    if window then return window.body end
end

-- window that is being dragged
local drag_window
local mouse_prev = nil, mouse_curr

function cs.gui_window.mouse_up(mouse)
    if mouse == cg.MC_LEFT then
        drag_window = nil
    end
end

function cs.gui_window.update_all()
    -- get mouse position
    local mouse_curr = cs.input.get_mouse_pos_pixels()
    if not mouse_prev then mouse_prev = mouse_curr end

    -- close button clicked?
    for ent, window in pairs(cs.gui_window.tbl) do
        if cs.gui.event_mouse_down(window.close_text) == cg.MC_LEFT
        and window.closeable then
            cs.entity.destroy(ent)
        end
    end

    -- clear destroyed
    if drag_window and cs.entity.destroyed(drag_window) then
        drag_window = nil
    end
    for ent in pairs(cs.gui_window.tbl) do
        if cs.entity.destroyed(ent) then cs.gui_window.remove(ent) end
    end

    -- update all
    for ent, window in pairs(cs.gui_window.tbl) do
        -- new drag motion?
        if cs.gui.event_mouse_down(window.titlebar) == cg.MC_LEFT
        and cs.gui.get_halign(ent) == cg.GA_NONE
        and cs.gui.get_valign(ent) == cg.GA_NONE then
            drag_window = ent
        end

        -- highlight?
        if window.highlight then
            cs.gui.set_color(window.title_text, cg.color(1, 1, 0.2, 1))
        else
            cs.gui.set_color(window.title_text, cg.color_white)
        end

        -- closeable?
        cs.gui.set_visible(window.close_text, window.closeable)

        -- update maximize/minimize
        if cs.gui.event_mouse_down(window.minmax_text) == cg.MC_LEFT then
            window.minimized = not window.minimized
        end
        cs.gui.set_visible(window.body, not window.minimized)
        cs.gui_text.set_str(window.minmax_text, window.minimized and '+' or '-')
    end

    -- move dragged window
    if drag_window then
        cs.transform.translate(drag_window, mouse_curr - mouse_prev)
    end

    mouse_prev = mouse_curr
end


--- textbox --------------------------------------------------------------------

cs.gui_textbox = { auto_saveload = true }

cs.gui_textbox.tbl = cg.entity_table()

function cs.gui_textbox.add(ent)
    if cs.gui_textbox.tbl[ent] then return end
    cs.gui_textbox.tbl[ent] = {}
    local gui_textbox = cs.gui_textbox.tbl[ent]

    -- add ent to gui_rect as container
    cg.add {
        ent = ent,
        gui_rect = {},
    }

    -- add text child
    gui_textbox.text = cg.add {
        transform = { parent = ent },
        gui = {
            color = cg.color_white,
            valign = cg.GA_MAX,
            halign = cg.GA_MIN
        },
        gui_text = {},
    }
end

function cs.gui_textbox.remove(ent)
    local textbox = cs.gui_textbox.tbl[ent]
    if textbox then cs.transform.destroy_rec(ent) end
    cs.gui_textbox.tbl[ent] = nil
end

function cs.gui_textbox.has(ent)
    return cs.gui_textbox.tbl[ent] ~= nil
end

cg.simple_props(cs.gui_textbox, {
    click_focus = false
})

function cs.gui_textbox.get_text(ent)
    local gui_textbox = cs.gui_textbox.tbl[ent]
    if gui_textbox then return gui_textbox.text end
end

function cs.gui_textbox.update_all()
    for ent in pairs(cs.gui_textbox.tbl) do
        if cs.entity.destroyed(ent) then cs.gui_textbox.remove(ent) end
    end

    for ent, textbox in pairs(cs.gui_textbox.tbl) do
        if textbox.click_focus
        and cs.gui.event_mouse_up(ent) == cg.MC_LEFT then
            cs.gui.set_focus(textbox.text, true)
        end
    end
end


--- gui_checkbox ---------------------------------------------------------------

cs.gui_checkbox = { auto_saveload = true }

cs.gui_checkbox.tbl = cg.entity_table()

function cs.gui_checkbox.add(ent)
    if cs.gui_checkbox.tbl[ent] then return end

    local checkbox = {}
    cs.gui_checkbox.tbl[ent] = checkbox

    checkbox.checked = false

    cs.gui_textbox.add(ent)
    checkbox.ent = ent
    checkbox.text = cs.gui_textbox.get_text(ent)
end

function cs.gui_checkbox.remove(ent)
    cs.gui_textbox.remove(ent)
    cs.gui_checkbox.tbl[ent] = nil
end

function cs.gui_checkbox.has(ent)
    return cs.gui_checkbox.tbl[ent] ~= nil
end

local function checkbox_toggle(checkbox)
    checkbox.checked = not checkbox.checked
    cs.gui.fire_event_changed(checkbox.ent)
end
function cs.gui_checkbox.toggle(ent)
    local checkbox = cs.gui_checkbox.tbl[ent]
    if checkbox then checkbox_toggle(checkbox) end
end
function cs.gui_checkbox.set_checked(ent, checked)
    local checkbox = cs.gui_checkbox.tbl[ent]
    -- do it this way to fire 'changed' event correctly
    if checkbox.checked ~= checked then checkbox_toggle(checkbox) end
end
function cs.gui_checkbox.get_checked(ent, checked)
    local checkbox = cs.gui_checkbox.tbl[ent]
    if checkbox then return checkbox.checked end
end

function cs.gui_checkbox.update_all(ent)
    for ent in pairs(cs.gui_checkbox.tbl) do
        if cs.entity.destroyed(ent) then cs.gui_checkbox.remove(ent) end
    end

    for ent, checkbox in pairs(cs.gui_checkbox.tbl) do
        if cs.gui.event_mouse_down(ent) == cg.MC_LEFT then
            checkbox_toggle(checkbox)
        end

        if checkbox.checked then
            cs.gui_text.set_str(checkbox.text, 'yes')
        else
            cs.gui_text.set_str(checkbox.text, 'no')
        end
    end
end
