local ffi = require 'ffi'
cgame = setmetatable({}, { __index = ffi.C })

require 'cgame.util'
require 'cgame.struct'
require 'cgame.entity_table'
require 'cgame.system'
