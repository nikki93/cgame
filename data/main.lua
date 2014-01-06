-- add cgame require path
package.path = package.path .. ';' .. cgame_data_path .. '/?.lua'

-- load 'cgame' module as global
cgame = require 'cgame'

dofile(cgame_data_path .. (cgame_args[1] or '/test/test.lua'))

