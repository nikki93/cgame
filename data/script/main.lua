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

    elseif key == cg.KC_C then
        print("destroying group 'default'")
        cs.group.destroy('default')

    end
end

local run = (cg.args[1] and loadfile(cg.args[1])) or loadfile('./main.lua')
if run then
    -- run script given, run it
    run()

    -- no camera? add default
    if cs.camera.get_current_camera() == cg.entity_nil then
        cg.add { camera = { viewport_height = 18.75 } }
    end
else
    -- no startup script

    -- add default camera
    cg.add { camera = { viewport_height = 18.75 } }

    -- go into edit mode
    cs.timing.set_paused(true)
    cs.edit.set_enabled(true)
end

cs.edit.undo_save()
