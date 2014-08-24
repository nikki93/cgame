local bump = require 'bump'

local world = bump.newWorld()

cs.bump = cg.simple_sys()

cg.simple_prop(cs.bump, 'bbox', cg.bbox(cg.vec2(-0.5, -0.5),
                                        cg.vec2(0.5, 0.5)))

local function _update_rect(obj, add)
    if world:hasItem(obj.ent.id)
    and obj.last_dirty == cs.transform.get_dirty_count(obj.ent) then
        return
    end

    local lt = obj.bbox.min + cs.transform.get_position(obj.ent)
    local wh = obj.bbox.max - obj.bbox.min
    if world:hasItem(obj.ent.id) then
        world:move(obj.ent.id, lt.x, lt.y, wh.x, wh.y)
    else
        world:add(obj.ent.id, lt.x, lt.y, wh.x, wh.y)
    end

    obj.last_dirty = cs.transform.get_dirty_count(obj.ent)
end

function cs.bump.create(obj)
    cs.transform.add(obj.ent)
    _update_rect(obj, true)
end
function cs.bump.destroy(obj)
    if world:hasItem(obj.ent.id) then
        world:remove(obj.ent.id)
    end
end

function cs.bump.set_position(ent, pos)
    local obj = cs.bump.tbl[ent]
    assert(obj, 'entity must be in bump system')
    cs.transform.set_position(ent, pos)
    _update_rect(obj)
end

local function _filter_wrap(filter)
    return filter and function (id)
        return filter(cg.Entity { id = id })
    end
end

function cs.bump.sweep(ent, p, filter)
    local obj = cs.bump.tbl[ent]
    assert(obj, 'entity must be in bump system')
    _update_rect(obj)

    p = obj.bbox.min + cs.transform.get_position(ent) + (p or cg.vec2_zero)

    local wfilter = _filter_wrap(filter)
    local cols, len = world:check(obj.ent.id, p.x, p.y, wfilter)
    local min = obj.bbox.min
    local ecols = {}
    for i = 1, len do
        local col = cols[i]
        local tl, tt, nx, ny, sl, st = col:getSlide()
        table.insert(ecols, {
            other = cg.Entity { id = col.other },
            touch = cg.vec2(tl, tt) - min,
            normal = cg.vec2(nx, ny),
            slide = cg.vec2(sl, st) - min,
        })
    end
    return ecols
end

function cs.bump.slide(ent, p, filter)
    local obj = cs.bump.tbl[ent]
    assert(obj, 'entity must be in bump system')
    _update_rect(obj)

    p = cs.transform.get_position(ent) + p

    local min = obj.bbox.min
    local wfilter = _filter_wrap(filter)

    local function rslide(ax, ay, bx, by, depth)
        if depth > 3 then return ax, ay, {} end

        world:move(obj.ent.id, ax, ay)
        local cols, len = world:check(obj.ent.id, bx, by, wfilter)
        if len == 0 then return bx, by, {} end

        -- find best next collision recursively
        local m = -1
        local mcols, mcol, mtx, mty, mnx, mny, msx, msy
        for i = 1, len do
            world:move(obj.ent.id, ax, ay) -- TODO: avoid re-moving
            local tx, ty, nx, ny, sx, sy = cols[i]:getSlide()
            local px, py, pcols = rslide(tx, ty, sx, sy, depth + 1)
            local dx, dy = px - ax, py - ay
            local d = dx * dx + dy * dy
            if d > m then
                m = d
                bx, by = px, py
                mcols, mcol = pcols, cols[i]
                mtx, mty, mnx, mny, msx, msy = tx, ty, nx, ny, sx, sy
            end
        end

        -- add next collision and return
        table.insert(mcols, {
            other = cg.Entity { id = mcol.other },
            touch = cg.vec2(mtx, mty) - min,
            normal = cg.vec2(mnx, mny),
            slide = cg.vec2(msx, msy) - min,
        })
        return bx, by, mcols
    end

    local a = cs.transform.get_position(obj.ent) + min
    local bx, by, cols = rslide(a.x, a.y, p.x + min.x, p.y + min.y, 0)
    cs.transform.set_position(ent, cg.vec2(bx - min.x, by - min.y))
    _update_rect(obj)
    return cols
end

function cs.bump.update(obj)
    _update_rect(obj)
    cs.edit.bboxes_update(obj.ent, obj.bbox)
end
