-- add data_dir to 'require' path
package.path = package.path .. ';' .. cgame_data_path

-- load 'cgame' module as global
cgame = require 'cgame'

dofile 'test/test.lua'

