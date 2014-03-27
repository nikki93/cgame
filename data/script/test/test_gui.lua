win = cg.add {
    transform = { position = cg.vec2(400, -100) },
    gui_rect = { size = cg.vec2(200, 400) },
    gui = {color = cg.color(0.2, 0.2, 0.9, 0.5) },
}
text = cg.add {
    transform = { parent = win },
    gui = {color = cg.color_white, valign = cg.GA_TABLE, halign = cg.GA_MIN },
    gui_text = { str = 'test line 1' },
}
text2 = cg.add {
    transform = { parent = win },
    gui = {color = cg.color_red, valign = cg.GA_TABLE, halign = cg.GA_MIN },
    gui_text = { str = 'test line 2...' },
}

win2 = cg.add {
    transform = { position = cg.vec2(20, -300), parent = win },
    gui_rect = { size = cg.vec2(150, 150) },
    gui = {
        color = cg.color(0.2, 0.9, 0.2, 0.5),
        valign = cg.GA_TABLE,
    },
}
text3 = cg.add {
    transform = { parent = win2 },
    gui = {color = cg.color_red, valign = cg.GA_TABLE, halign = cg.GA_MIN },
    gui_text = { str = 'test line 2...' },
}

text4 = cg.add {
    transform = { parent = win },
    gui = {color = cg.color_red, valign = cg.GA_TABLE, halign = cg.GA_MIN },
    gui_text = { str = 'test line 2...' },
}
