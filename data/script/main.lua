-- add cgame require path and load cgame
package.path = package.path .. ';' .. cgame_data_path .. '/script/?.lua'
require 'cgame'

-- misc. keys
cs.app = {}
function cs.app.key_down(key)
    if cs.gui.has_focus() then return end

    if key == cg.KC_Q then
        cs.game.quit()

    elseif key == cg.KC_1 then
        cs.console.set_visible(not cs.console.get_visible())

    elseif key == cg.KC_E then
        cs.timing.set_paused(not cs.edit.get_enabled())
        cs.edit.set_enabled(not cs.edit.get_enabled())

    elseif key == cg.KC_P then
        cs.timing.set_paused(not cs.timing.get_paused())

    elseif key == cg.KC_C then
        print("destroying group 'default'")
        cs.group.destroy('default')

    end
end

if cg.args[1] then
    -- startup script specified, run it
    dofile(cg.args[1])
else
    -- no startup script

    -- default camera
    cg.add { camera = { viewport_height = 18 } }
end

