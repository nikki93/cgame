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
