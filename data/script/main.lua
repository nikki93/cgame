-- add cgame require path
package.path = package.path .. ';' .. cgame_data_path .. '/script/?.lua'

-- load 'cgame' module -- it makes itself available globally
require 'cgame'

-- set up console text and rebind Lua 'print'
local console_text = cg.add {
    entity = { persistent = true },
    transform = { position = cg.vec2_zero },
    gui = { color = cg.color_black },
}
cs.console.set_entity(console_text)
print = function (k) cs.console.puts(tostring(k)) end

-- run startup script if specified
if cg.args[1] then dofile(cg.args[1]) end

