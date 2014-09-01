local ffi = require 'ffi'

function cs.edit.destroy_rec()
    for ent in pairs(cs.edit.select) do
        cs.transform.destroy_rec(ent)
    end

    cs.edit.undo_save()
end

function cs.edit.destroy()
    for ent in pairs(cs.edit.select) do
        cs.entity.destroy(ent)
    end

    cs.edit.undo_save()
end

function cs.edit.duplicate()
    -- save just current selection to a string
    for ent in pairs(cs.edit.select) do
        if cs.transform.has(ent) then
            cs.transform.set_save_filter_rec(ent, true)
        else
            cs.entity.set_save_filter(ent, true)
        end
    end
    local s = cg.store_open()
    cs.system.save_all(s)
    local str = ffi.string(cg.store_write_str(s))
    cg.store_close(s)

    -- clear selection
    cs.edit.select_clear()

    -- load from the string -- they were selected on save and so will be
    -- selected when loaded
    local d = cg.store_open_str(str)
    cs.system.load_all(d)
    cg.store_close(d)

    cs.edit.undo_save()
end
