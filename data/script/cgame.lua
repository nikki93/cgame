-- inherit C functions
local ffi = require 'ffi'
cgame = setmetatable({}, { __index = ffi.C })

-- aliases
cg = cgame
cg.args = cgame_args

-- useful ffi functions
cg.string = ffi.string

require 'cgame.util'
require 'cgame.struct'
require 'cgame.entity_table'
require 'cgame.system'
require 'cgame.input'
require 'cgame.edit'
require 'cgame.fps'
