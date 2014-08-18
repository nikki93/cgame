-- any exported C function/varaible f is available as cg.f, eg. C function
-- vec2(...) is available as cg.vec2(...) in Lua
local ffi = require 'ffi'
cg = setmetatable({}, { __index = ffi.C })

-- command-line args
cg.args = cgame_args

-- useful ffi functions
cg.string = ffi.string


-- import everything

require 'cgame.util'
require 'cgame.struct'
require 'cgame.entity_table'

require 'cgame.system'
require 'cgame.entity'
require 'cgame.name'
require 'cgame.group'
require 'cgame.input'
require 'cgame.gui'
require 'cgame.edit'
require 'cgame.animation'
require 'cgame.bump'
require 'cgame.physics'
require 'cgame.sound'

require 'cgame.console'
require 'cgame.fps'
