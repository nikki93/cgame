win = cg.add {
    transform = { position = cg.vec2(400, -300) },
    gui_rect = { size = cg.vec2(200, 150) },
    gui = { color = cg.color(0.2, 0.2, 0.9, 0.5) },
}

text = cg.add {
    transform = { position = cg.vec2(10, -10), parent = win },
    gui_text = { str = 'hello, world!' },
    gui = { color = cg.color_white },
}

