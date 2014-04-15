--- layout ---------------------------------------------------------------------

cs.edit.bottom_rect = cg.add {
    transform = { parent = cs.edit.gui_root },
    group = { groups = 'builtin' },
    edit = { editable = false },
    gui_rect = { hfill = true, size = cg.vec2(cs.game.get_window_size().x, 0) },
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
    gui_rect = { hfill = true, size = cg.vec2(cs.game.get_window_size().x, 0) },
    gui = {
        color = cg.color(0.7, 0.2, 0.2, 1.0),
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
    gui_rect = { hfill = true, size = cg.vec2(cs.game.get_window_size().x, 0) },
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

-- 'edit' text
cs.edit.edit_text = cg.add {
    transform = { parent = cs.edit.status_bar },
    group = { groups = 'builtin' },
    edit = { editable = false },
    gui_text = { str = 'edit' },
    gui = {
        color = cg.color_white,
        halign = cg.GA_TABLE, valign = cg.GA_MAX,
        padding = cg.vec2(5, 2),
    },
}

-- select text
cs.edit.select_text = cg.add {
    transform = { parent = cs.edit.status_bar },
    group = { groups = 'builtin' },
    edit = { editable = false },
    gui_text = { str = '' },
    gui = {
        visible = false,
        color = cg.color_white,
        halign = cg.GA_TABLE, valign = cg.GA_MAX,
        padding = cg.vec2(24, 2),
    },
}

-- mode text
cs.edit.mode_text = cg.add {
    transform = { parent = cs.edit.status_bar },
    group = { groups = 'builtin' },
    edit = { editable = false },
    gui_text = { str = '' },
    gui = {
        visible = false,
        color = cg.color_white,
        halign = cg.GA_TABLE, valign = cg.GA_MAX,
        padding = cg.vec2(24, 2),
    },
}


function cs.edit.set_mode_text(s)
    cs.gui.set_visible(cs.edit.mode_text, true)
    cs.gui_text.set_str(cs.edit.mode_text, s)
end
function cs.edit.hide_mode_text()
    cs.gui.set_visible(cs.edit.mode_text, false)
end
