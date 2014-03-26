-- add cgame require path
package.path = package.path .. ';' .. cgame_data_path .. '/script/?.lua'

-- load 'cgame' module -- it makes itself available globally
require 'cgame'

-- set up console text and rebind Lua 'print'
local console_text = cg.add {
    entity = { persistent = true },
    group = { groups = 'builtin' },
    transform = { position = cg.vec2_zero },
    gui = { color = cg.color_black },
}
cs.console.set_entity(console_text)
print = function (k) cs.console.puts(tostring(k)) end

-- set up global keys
cs.app = {}
function cs.app.key_down(key)
    if key == cg.KC_ESCAPE then
        cs.game.quit()
    elseif key == cg.KC_E then
        cs.timing.set_paused(not cs.edit.get_enabled())
        cs.edit.set_enabled(not cs.edit.get_enabled())
    elseif key == cg.KC_P then
        cs.timing.set_paused(not cs.timing.get_paused())
    elseif key == cg.KC_C then
        cs.group.destroy('default')
        print("destroyed group 'default'")
    elseif key == cg.KC_S then
        print("saving group 'default' to file '"
                  .. cgame_usr_path .. "test.sav'")
        cs.group.set_save_filter('default', true)
        local s = cs.serializer.open_file(cgame_usr_path .. 'test.sav')
        cs.system.save_all(s)
        cs.serializer.close(s)
    elseif key == cg.KC_L then
        print("loading from file '" .. cgame_usr_path .. "test.sav'")
        local d = cs.deserializer.open_file(cgame_usr_path .. 'test.sav')
        cs.system.load_all(d)
        cs.deserializer.close(d)
    end
end

-- run startup script if specified
if cg.args[1] then dofile(cg.args[1]) end

