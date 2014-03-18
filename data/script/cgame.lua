-- inherit C functions
local ffi = require 'ffi'
cgame = setmetatable({}, { __index = ffi.C })

-- aliases
cg = cgame
cg.args = cgame_args

require 'cgame.util'
require 'cgame.struct'
require 'cgame.entity_table'
require 'cgame.system'
