-- add cgame require path
package.path = package.path .. ';' .. cgame_data_path .. '/?.lua'

-- load 'cgame' module as global
cgame = require 'cgame'
cg = cgame

-- print to console
print = function (k) cgame.console_puts(tostring(k)) end

-- got startup script argument?
if cgame_args[1] then dofile(cgame_args[1]) end

