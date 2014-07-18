cs.animation = { auto_saveload = true }

cs.animation.tbl = cg.entity_table()

function cs.animation.add(ent)
    if cs.animation.tbl[ent] then return end

    cs.animation.tbl[ent] = {
        ent = ent,
        anims = {},
        curr_anim = nil,   -- name of animation
        frame = 1,         -- index of frame
        t = 1,             -- time left in this frame
    }
end
function cs.animation.remove(ent)
    cs.animation.tbl[ent] = nil
end
function cs.animation.has(ent)
    return cs.animation.tbl[ent] ~= nil
end

-- utility for contiguous strips of frames
function cs.animation.set_strips(ent, tbl)
    local entry = cs.animation.tbl[ent]
    assert(entry, 'entity must be in animation system')
    for anim, strip in pairs(tbl) do
        entry.anims[anim] = { n = strip.n, strip = strip }
    end
end

-- manual specification of every frame and its duration
function cs.animation.set_frames(ent, tbl)
    local entry = cs.animation.tbl[ent]
    assert(entry, 'entity must be in animation system')
    for anim, frames in pairs(anim) do
        entry.anims[anim] = { n = #frames, frames = frames }
    end
end

local function _enter_frame(entry, frame, anim)
    anim = anim or entry.anims[entry.curr_anim]
    entry.frame = frame
    if anim.strip then
        entry.t = anim.strip.t
        local v = cg.Vec2(anim.strip.base)
        v.x = v.x + (frame - 1) * cs.sprite.get_texsize(entry.ent).x
        cs.sprite.set_texcell(entry.ent, v)
    elseif anim.frames then
        local frm = anim.frames[frame]
        entry.t = frm.t
        if frm.texcell then cs.sprite.set_texcell(entry.ent, frm.texcell) end
        if frm.texsize then cs.sprite.set_texsize(entry.ent, frm.texsize) end
    end
end

function cs.animation.switch(ent, anim)
    local entry = cs.animation.tbl[ent]
    assert(entry, 'entity must be in animation system')
    assert(entry.anims[anim], "must have an animation with name '"
               .. anim .. "'")
    if entry.curr_anim ~= anim then
        entry.curr_anim = anim
        _enter_frame(entry, 1)
    end
end

function cs.animation.start(ent, anim)
    local entry = cs.animation.tbl[ent]
    assert(entry, 'entity must be in animation system')
    assert(entry.anims[anim], "must have an animation with name '"
               .. anim .. "'")
    entry.curr_anim = anim
    _enter_frame(entry, 1)
end

function cs.animation.update_all()
    cg.entity_table_remove_destroyed(cs.animation.tbl, cs.animation.remove)

    if cs.timing.get_paused() then return end

    for ent, entry in pairs(cs.animation.tbl) do
        if entry.curr_anim then
            local dt = cs.timing.dt
            local anim = entry.anims[entry.curr_anim]

            -- next frame?
            while entry.t <= dt do
                _enter_frame(entry, entry.frame >= anim.n
                                 and 1 or entry.frame + 1, anim)
                dt = dt - entry.t
            end

            entry.t = entry.t - dt
        end
    end
end
