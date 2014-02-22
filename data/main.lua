-- add cgame require path
package.path = package.path .. ';' .. cgame_data_path .. '/?.lua'

-- load 'cgame' module as global
cgame = require 'cgame'

-- print to console
print = function (k) cgame.console_puts(tostring(k)) end

-- run scratch buffer on shift+R
cgame.add_system
{
    name = 'scratch',

    key_down = function (key)
        if key == cgame.KC_R and cgame.input_key_down(cgame.KC_LEFT_SHIFT) then
            dofile(cgame_usr_path .. 'scratch.lua')
        end
    end,
}

dofile(cgame_data_path .. (cgame_args[1] or 'test/test.lua'))

