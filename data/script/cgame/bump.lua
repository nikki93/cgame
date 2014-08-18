local bump = require 'bump'

local world = bump.newWorld()

cs.bump = cg.simple_sys()

cg.simple_prop(cs.bump, 'bbox', cg.bbox(cg.vec2(-0.5, -0.5),
                                        cg.vec2(0.5, 0.5)))

function _update_rect(obj, add)
    local rect = obj.rect
    if not rect then
        rect = { ent = obj.ent }
        obj.rect = rect
    end
    
    local pos = cs.transform.get_position(obj.ent)
    local bbt = cg.bbox(obj.bbox.min + pos, obj.bbox.max + pos)
    rect.l, rect.t = bbt.min.x, bbt.min.y
    local bbs = bbt.max - bbt.min
    rect.w, rect.h = bbs.x, bbs.y
    if not world.rects[rect] then
        world:add(rect, rect.l, rect.t, rect.w, rect.h)
    end
    world:move(rect, rect.l, rect.t, rect.w, rect.h)
end

function cs.bump.create(obj)
    _update_rect(obj, true)
end
function cs.bump.destroy(obj)
    world:remove(obj.rect)
end

function cs.bump.get_collisions(ent, p)
    local obj = cs.bump.tbl[ent]
    assert(obj, 'entity must be in bump system')
    local mp = p and (obj.bbox.min + p)
    local cols, len = world:check(obj.rect,
                                  mp and mp.x or obj.rect.l,
                                  mp and mp.y or obj.rect.p)
    local ecols = {}

    for i = 1, len do
        local col = cols[i]
        local min = cs.bump.tbl[col.other.ent].bbox.min
        local function conv(x, y)
            return cg.vec2(x, y) - min
        end
        local tl, tt, nx, ny, sl, st = col:getSlide()
        table.insert(ecols, {
            other = col.other.ent,
            touch = cg.vec2(tl, tt) - min,
            normal = cg.vec2(nx, ny),
            slide = cg.vec2(sl, st) - min,
        })
    end
    return ecols
end

function cs.bump.slide(ent, np, first)
    local obj = cs.bump.tbl[ent]
    assert(obj, 'entity must be in bump system')
    local rcols = {}

    local x, y = np.x + obj.bbox.min.x, np.y + obj.bbox.min.y
    local tl, tt, nx, ny
    local hit = {}
    while true do
        local cols, len = world:check(obj.rect, x, y)
        if len == 0 then
            obj.rect.l, obj.rect.t = x, y
            world:move(obj.rect, x, y)
            break
        end
        local col = cols[1]
        tl, tt, nx, ny, x, y = col:getSlide()
        table.insert(rcols, {
            other = col.other.ent,
            normal = cg.vec2(nx, ny) ,
        })
        obj.rect.l, obj.rect.t = tl, tt
        world:move(obj.rect, tl, tt)
        if hit[col.other] or first == true then break end
        hit[col.other] = true
    end
    cs.transform.set_position(ent, cg.vec2(obj.rect.l - obj.bbox.min.x,
                                           obj.rect.t - obj.bbox.min.y))

    return rcols
end

function cs.bump.update(obj)
    _update_rect(obj)
end
