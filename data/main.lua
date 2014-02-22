-- add cgame require path
package.path = package.path .. ';' .. cgame_data_path .. '/?.lua'

-- load 'cgame' module as global
cgame = require 'cgame'

-- print to console
print = cgame.console_puts

dofile(cgame_data_path .. (cgame_args[1] or '/test/test.lua'))

