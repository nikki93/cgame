cs.edit.select = cg.entity_table()

-- get some selected entity, or nil if none selected
function cs.edit.select_get_first()
    for ent in pairs(cs.edit.select) do
        return ent
    end
    return nil
end

function cs.edit.select_toggle(ent)
    if cs.edit.select[ent] then
        cs.edit.select[ent] = nil
    else
        cs.edit.select[ent] = true
    end
end

function cs.edit.select_clear()
    cs.edit.select = cg.entity_table()
end


--- click select ---------------------------------------------------------------

local function _get_entities_under_mouse()
    local ents = {}

    local m = cs.camera.unit_to_world(cs.input.get_mouse_pos_unit())
    for i = 0, cs.edit.bboxes_get_num() - 1 do
        local pair = cs.edit.bboxes_get_nth(i)

        -- transform m to local space
        local t = cg.mat3_inverse(cs.transform.get_world_matrix(pair.ent))
        if cg.bbox_contains(pair.bbox, cg.mat3_transform(t, m)) then
            table.insert(ents, cg.Entity(pair.ent))
        end
    end

    -- sort by distance to mouse
    local distcomp = function (e1, e2)
        local p1 = cs.transform.get_world_position(e1)
        local p2 = cs.transform.get_world_position(e2)
        return cg.vec2_dist(p1, m) < cg.vec2_dist(p2, m)
    end
    table.sort(ents, distcomp)

    return ents
end

function cs.edit.select_click_single()
    -- anything under mouse?
    local ents = _get_entities_under_mouse()
    if #ents == 0 then
        cs.edit.select = cg.entity_table()
        cs.edit.undo_save()
        return
    end

    -- if something's already selected, select the next thing
    ents[#ents + 1] = ents[1]    -- duplicate first at end to wrap-around
    local sel = 0
    for i = 1, #ents - 1 do
        sel = i
        if cs.edit.select[ents[i]] then
            break
        end
    end
    cs.edit.select = cg.entity_table()
    cs.edit.select[ents[sel + 1]] = true

    cs.edit.undo_save()
end

function cs.edit.select_click_multi()
    -- anything under mouse?
    local ents = _get_entities_under_mouse()
    if #ents == 0 then
        cs.edit.undo_save()
        return
    end

    -- if something isn't selected, select it
    for i = 1, #ents do
        if not cs.edit.select[ents[i]] then
            cs.edit.select[ents[i]] = true
            cs.edit.undo_save()
            return
        end
    end

    -- otherwise deselect the first
    cs.edit.select[ents[1]] = nil

    cs.edit.undo_save()
end


--- boxsel ---------------------------------------------------------------------

local boxsel_has_begun, boxsel_init_mouse_pos

local function _boxsel_select()
end

function cs.edit.boxsel_start()
    cs.edit.set_mode('boxsel')
end
function cs.edit.boxsel_begin()
    boxsel_has_begun = true
    boxsel_init_mouse_pos = cs.input.get_mouse_pos_pixels()
end
function cs.edit.boxsel_end()
    cs.edit.select = cg.entity_table()
    cs.edit.boxsel_end_add()
end
function cs.edit.boxsel_end_add()
    if boxsel_has_begun then
        b = cs.camera.pixels_to_world(boxsel_init_mouse_pos)
        e = cs.camera.pixels_to_world(cs.input.get_mouse_pos_pixels())
        bb = cg.BBox(cg.bbox_bound(b, e))

        for i = 0, cs.edit.bboxes_get_num() - 1 do
            local ent = cg.Entity(cs.edit.bboxes_get_nth(i).ent)
            if cg.bbox_contains(bb, cs.transform.get_world_position(ent)) then
                cs.edit.select[ent] = true
            end
        end
    end

    cs.edit.set_mode('normal')
    cs.edit.undo_save()
end

cs.edit.boxsel_box = cg.add {
    group = { groups = 'builtin' },
    edit = { editable = false },
    transform = { position = cg.vec2(-20, -20) },
    gui_rect = { size = cg.vec2(10, 10), hfit = false, vfit = false },
    gui = { captures_events = false, color = cg.color(0.8, 0.5, 0.1, 0.3) },
}

cs.edit.modes.boxsel = {}

function cs.edit.modes.boxsel.enter()
    cs.edit.set_mode_text('boxsel')

    boxsel_has_begun = false
end

function cs.edit.modes.boxsel.exit()
    cs.transform.set_position(cs.edit.boxsel_box, cg.vec2(-20, -20))
    cs.gui_rect.set_size(cs.edit.boxsel_box, cg.vec2(10, 10))
end

function cs.edit.modes.boxsel.update_all()
    if not boxsel_has_begun then return end

    m = cs.input.get_mouse_pos_pixels()
    b = cg.BBox(cg.bbox_bound(m, boxsel_init_mouse_pos))
    cs.transform.set_position(cs.edit.boxsel_box, cg.vec2(b.min.x, b.max.y))
    cs.gui_rect.set_size(cs.edit.boxsel_box, b.max - b.min)
end
