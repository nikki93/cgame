function cs.edit.destroy_rec()
    for ent, _ in pairs(cs.edit.select) do
        cs.transform.destroy_rec(ent)
    end

    cs.edit.undo_save()
end

function cs.edit.destroy()
    for ent, _ in pairs(cs.edit.select) do
        cs.entity.destroy(ent)
    end

    cs.edit.undo_save()
end

function cs.edit.duplicate()
    -- save just current selection to a string
    for ent, _ in pairs(cs.edit.select) do
        if cs.transform.has(ent) then
            cs.transform.set_save_filter_rec(ent, true)
        else
            cs.entity.set_save_filter(ent, true)
        end
    end
    local s = cs.serializer.open_str()
    cs.system.save_all(s)
    local str = ffi.string(cs.serializer.get_str(s))
    cs.serializer.close(s)

    -- clear selection
    cs.edit.select_clear()

    -- load from the string -- they were selected on save and so will be
    -- selected when loaded
    local d = cs.deserializer.open_str(str)
    cs.system.load_all(d)
    cs.deserializer.close(d)

    cs.edit.undo_save()
end
