--- layout ---------------------------------------------------------------------

cs.edit.bottom_rect = cg.add {
    transform = { parent = cs.edit.gui_root },
    group = { groups = 'builtin' },
    edit = { editable = false },
    gui_rect = { hfill = true },
    gui = {
        color = cg.color(0, 0, 0, 0), -- invisible
        captures_events = false;
        halign = cg.GA_MIN, valign = cg.GA_MIN,
        padding = cg.vec2_zero,
    }
}

cs.edit.status_bar = cg.add {
    transform = { parent = cs.edit.bottom_rect },
    group = { groups = 'builtin' },
    edit = { editable = false },
    gui_rect = { hfill = true },
    gui = {
        color = cg.color(0.8, 0.3, 0.3, 1.0),
        halign = cg.GA_MIN, valign = cg.GA_TABLE,
        padding = cg.vec2_zero,
    }
}
cg.add {
    transform = { parent = cs.edit.status_bar },
    group = { groups = 'builtin' },
    edit = { editable = false },
    gui_text = { str = '' },
    gui = {
        color = cg.color_black,
        halign = cg.GA_MIN, valign = cg.GA_MAX,
        padding = cg.vec2(2, 2),
    },
}


cs.edit.bottom_bar = cg.add {
    transform = { parent = cs.edit.bottom_rect },
    group = { groups = 'builtin' },
    edit = { editable = false },
    gui_rect = { hfill = true },
    gui = {
        color = cg.color(0.9, 0.9, 0.9, 1.0),
        halign = cg.GA_MIN, valign = cg.GA_TABLE,
        padding = cg.vec2_zero,
    }
}
cg.add {
    transform = { parent = cs.edit.bottom_bar },
    group = { groups = 'builtin' },
    edit = { editable = false },
    gui_text = { str = '' },
    gui = {
        color = cg.color_black,
        halign = cg.GA_MIN, valign = cg.GA_MAX,
        padding = cg.vec2(2, 2),
    },
}


--- text -----------------------------------------------------------------------

local function create_status_textbox(gap, label)
    local textbox = cg.add {
        transform = { parent = cs.edit.status_bar },
        group = { groups = 'builtin' },
        edit = { editable = false },
        gui = {
            color = cg.color(0.6, 0.1, 0.1, 1),
            halign = cg.GA_TABLE, valign = cg.GA_MAX,
            padding = cg.vec2(gap, 0),
        },
        gui_rect = {},
    }
    local text = cg.add {
        transform = { parent = textbox },
        group = { groups = 'builtin' },
        edit = { editable = false },
        gui_text = { str = label or '' },
        gui = {
            color = cg.color_white,
            halign = cg.GA_MIN, valign = cg.GA_MAX,
            padding = cg.vec2(4, 2),
        },
    }
    return textbox, text
end

cs.edit.edit_textbox, cs.edit.edit_text = create_status_textbox(0, 'edit')
cs.edit.grid_textbox, cs.edit.grid_text = create_status_textbox(7, '')
cs.edit.select_textbox, cs.edit.select_text = create_status_textbox(7, '')
cs.edit.mode_textbox, cs.edit.mode_text = create_status_textbox(7, '')

cs.edit.play_textbox, cs.edit.play_text = create_status_textbox(0, '\xcb')
cs.gui.set_halign(cs.edit.play_textbox, cg.GA_MAX)

function cs.edit.set_mode_text(s)
    cs.gui.set_visible(cs.edit.mode_textbox, true)
    cs.gui_text.set_str(cs.edit.mode_text, s)
end
function cs.edit.hide_mode_text()
    cs.gui.set_visible(cs.edit.mode_textbox, false)
end
cs.edit.hide_mode_text()

--- command text ---------------------------------------------------------------

cs.edit.command_bar = cg.add {
    transform = { parent = cs.edit.bottom_bar },
    group = { groups = 'builtin' },
    edit = { editable = false },
    gui_rect = { hfill = true },
    gui = {
        visible = false,
        color = cg.color_clear,
        halign = cg.GA_MIN, valign = cg.GA_TABLE,
        padding = cg.vec2_zero,
    }
}
cs.edit.command_text_colon = cg.add {
    transform = { parent = cs.edit.command_bar },
    group = { groups = 'builtin' },
    edit = { editable = false },
    gui = {
        color = cg.color_black,
        halign = cg.GA_TABLE, valign = cg.GA_MAX,
        padding = cg.vec2(2, 2),
    },
    gui_text = { str = ':' },
}
cs.edit.command_text = cg.add {
    transform = { parent = cs.edit.command_bar },
    group = { groups = 'builtin' },
    edit = { editable = false },
    gui = {
        color = cg.color_black,
        halign = cg.GA_TABLE, valign = cg.GA_MAX,
        padding = cg.vec2(0, 2),
    },
    gui_text = { str = '' },
    gui_textedit = {},
}
cs.edit.command_completions_text = cg.add {
    transform = { parent = cs.edit.command_bar },
    group = { groups = 'builtin' },
    edit = { editable = false },
    gui = {
        color = cg.color(0.5, 0.30, 0.05, 1.0),
        halign = cg.GA_TABLE, valign = cg.GA_MAX,
        padding = cg.vec2(0, 2),
    },
    gui_text = { str = 'completion1 | completion2' },
    gui_textedit = {},
}
