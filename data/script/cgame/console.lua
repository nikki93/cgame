-- create console text entity in top-left corner
local console_text = cg.add {
    group = { groups = 'builtin' },
    edit = { editable = false },
    gui = {
        color = cg.color_black,
        halign = cg.GA_MIN, valign = cg.GA_MAX,
        padding = cg.vec2_zero,
    },
}
cs.console.set_entity(console_text)

-- rebind Lua 'print' to print to console
print = function (k) cs.console.puts(tostring(k)) end
