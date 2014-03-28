win1 = cg.add {
    gui_rect = {},
    gui = {
        color = cg.color(0.2, 0.2, 0.9, 0.5),
        valign = cg.GA_TABLE,
        halign = cg.GA_MAX,
    },
}

win2 = cg.add {
    gui_rect = {},
    gui = {
        color = cg.color(0.2, 0.2, 0.9, 0.5),
        valign = cg.GA_TABLE,
        halign = cg.GA_MAX,
    },
}

text1 = cg.add {
    transform = { parent = win1 },
    gui = { color = cg.color_white, valign = cg.GA_TABLE, halign = cg.GA_MIN },
    gui_text = { str = 'transform' },
}

text2 = cg.add {
    transform = { parent = win2 },
    gui = { color = cg.color_white, valign = cg.GA_TABLE, halign = cg.GA_MIN },
    gui_text = { str = 'sprite' },
}

win3 = cg.add {
    transform = { parent = win2 },
    gui_rect = {},
    gui = {
        color = cg.color(0.2, 0.9, 0.2, 0.5),
        valign = cg.GA_TABLE,
        halign = cg.GA_MIN,
    },
}
text3 = cg.add {
    transform = { parent = win3 },
    gui = { color = cg.color_white, valign = cg.GA_TABLE, halign = cg.GA_MIN },
    gui_text = { str = 'size: (?, ?)' },
}

win4 = cg.add {
    transform = { parent = win2 },
    gui_rect = {},
    gui = {
        color = cg.color(0.2, 0.9, 0.2, 0.5),
        valign = cg.GA_TABLE,
        halign = cg.GA_MIN,
    },
}

win5 = cg.add {
    transform = { parent = win4 },
    gui_rect = {},
    gui = {
        color = cg.color(0.9, 0.5, 0.2, 0.5),
        valign = cg.GA_MAX,
        halign = cg.GA_TABLE,
    },
}
text4 = cg.add {
    transform = { parent = win5 },
    gui = { color = cg.color_white, valign = cg.GA_TABLE, halign = cg.GA_MIN },
    gui_text = { str = 'prop1' },
}
text4 = cg.add {
    transform = { parent = win5 },
    gui = { color = cg.color_white, valign = cg.GA_TABLE, halign = cg.GA_MIN },
    gui_text = { str = 'prop2' },
}


win6 = cg.add {
    transform = { parent = win4 },
    gui_rect = {},
    gui = {
        color = cg.color(0.9, 0.5, 0.2, 0.5),
        valign = cg.GA_MAX,
        halign = cg.GA_TABLE,
    },
}
text5 = cg.add {
    transform = { parent = win6 },
    gui = { color = cg.color_white, valign = cg.GA_TABLE, halign = cg.GA_MIN },
    gui_text = { str = 'prop3' },
}
