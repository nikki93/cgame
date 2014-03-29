-- create console text entity in top-left corner
local console_text = cg.add {
    group = { groups = 'builtin' },
    edit = { editable = false },
    gui = {
        color = cg.color_black,
        halign = cg.GA_MIN, valign = cg.GA_MAX,
        padding = cg.vec2(1, 1),
    },
}
cs.console.set_entity(console_text)

-- rebind Lua 'print' to print to console
function print(...)
    local args = table.pack(...)
    local sargs = {}
    for i = 1, args.n do
        sargs[i] = tostring(args[i])
    end
    cs.console.puts(table.concat(sargs, '    '))
end
