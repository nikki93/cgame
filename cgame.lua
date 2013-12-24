local ffi = require 'ffi'

-- reads C declarations from files, between 'script_begin'/'script_end' blocks
-- or on 'script_export' lines
function readdef(filenames)
    local str = ''

    for _, filename in pairs(filenames) do
        local file = assert(io.open(filename, 'r'))
        local in_script = 0
        str = str .. '    /* ' .. filename .. ' */\n'
        while true do
            local line = file:read()
            if line == nil then break end

            if line:find('script_begin') then
                in_script = in_script + 1
            elseif line:find('script_end') and in_script > 0 then
                in_script = in_script - 1
            elseif in_script > 0 or line:find('script_export') then
                str = str .. '    ' .. line .. '\n'
            end
        end
    end

    print('exposing to lua:')
    print(str)

    ffi.cdef(str)
    return ffi.C
end

-- create a table with some custom wrappers etc. that uses ffi.C as a
-- 'fallback'

local C = readdef
{
    'game.h',
    'entity.h',
    'transform.h',
    'sprite.h',

    'test/keyboard_controlled.h'
}

local cgame = setmetatable(
{
    Vec2 = ffi.typeof('Vec2')
}, { __index = C })

return cgame

