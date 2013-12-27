-- add cgame require path
package.path = package.path .. ';' .. cgame_require_path

-- load 'cgame' module as global
cgame = require 'cgame'

dofile 'test/test.lua'

