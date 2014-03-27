-- create console text entity in top-left corner
local console_text = cg.add {
    group = { groups = 'builtin' },
    edit = { editable = false },
    transform = { position = cg.vec2_zero },
    gui = { color = cg.color_black },
}
cs.console.set_entity(console_text)

-- rebind Lua 'print' to print to console
print = function (k) cs.console.puts(tostring(k)) end
