win = cg.add {
    gui_window = { title = 'parent window!' },
    gui = {
        valign = cg.GA_TABLE,
        halign = cg.GA_MAX,
    },
}

text1 = cg.add {
    transform = { parent = cs.gui_window.get_body(win) },
    gui = {
        color = cg.color_white,
        valign = cg.GA_TABLE,
        halign = cg.GA_MIN
    },
    gui_text = { str = 'body is a very very long text\nof many lines!' },
}

win2 = cg.add {
    gui_window = { title = 'child window!' },
    gui_rect = { hfill = true },
    transform = { parent = cs.gui_window.get_body(win) },
    gui = {
        valign = cg.GA_TABLE,
        halign = cg.GA_MIN,
    },
}

text2 = cg.add {
    transform = { parent = cs.gui_window.get_body(win2) },
    gui = {
        color = cg.color_white,
        valign = cg.GA_TABLE,
        halign = cg.GA_MIN
    },
    gui_text = { str = 'prop1: x, y, z' },
}

win = cg.add {
    gui_window = { title = 'parent window 2!', closeable = false },
    gui = {
        valign = cg.GA_TABLE,
        halign = cg.GA_MAX,
    },
}

win2 = cg.add {
    gui_window = { title = 'child window!' },
    gui_rect = { hfill = true },
    transform = { parent = cs.gui_window.get_body(win) },
    gui = {
        valign = cg.GA_TABLE,
        halign = cg.GA_MIN,
    },
}

text2 = cg.add {
    transform = { parent = cs.gui_window.get_body(win2) },
    gui = {
        color = cg.color_white,
        valign = cg.GA_TABLE,
        halign = cg.GA_MIN
    },
    gui_text = { str = 'body is a very very long text\nof many lines!' },
}

win = cg.add {
    transform = { position = cg.vec2(42, -42) },
    gui_window = { title = 'free window!' },
}

text1 = cg.add {
    transform = { parent = cs.gui_window.get_body(win) },
    gui = {
        color = cg.color_white,
        valign = cg.GA_TABLE,
        halign = cg.GA_MIN
    },
    gui_text = { str = 'drag titlebar to move' },
}

win2 = cg.add {
    gui_window = { title = 'child window!' },
    gui_rect = { hfill = true },
    transform = { parent = cs.gui_window.get_body(win) },
    gui = {
        valign = cg.GA_TABLE,
        halign = cg.GA_MIN,
    },
}

text2 = cg.add {
    transform = { parent = cs.gui_window.get_body(win2) },
    gui = {
        color = cg.color_white,
        valign = cg.GA_TABLE,
        halign = cg.GA_MIN
    },
    gui_text = { str = 'I move with my parent!' },
}

textedit = cg.add {
    transform = { parent = cs.gui_window.get_body(win2) },
    gui = {
        color = cg.color_white,
        valign = cg.GA_TABLE,
        halign = cg.GA_MIN
    },
    gui_text = { str = 'edit me!!' },
    gui_event = {
        key_down = function (e, k)
            if k == cg.KC_A then print('A!') end
        end
    },
    gui_textedit = {},
}

textedit = cg.add {
    transform = { parent = cs.gui_window.get_body(win2) },
    gui = {
        color = cg.color_white,
        valign = cg.GA_TABLE,
        halign = cg.GA_MIN
    },
    gui_text = { str = "I'm editable too..." },
    gui_textedit = {},
}
