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
function cs.animation.remove(ent, anim)
    local entry = cs.animation.tbl[ent]
    if entry then
        if anim then
            if entry.curr_anim == anim then
                entry.curr_anim = nil
            end
            entry.anims[anim] = nil
        else
            cs.animation.tbl[ent] = nil
        end
    end
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
        entry.t = anim.strip.t > 0 and anim.strip.t or 1
        local v = cg.Vec2(anim.strip.base)
        v.x = v.x + (frame - 1) * cs.sprite.get_texsize(entry.ent).x
        cs.sprite.set_texcell(entry.ent, v)
    elseif anim.frames then
        local frm = anim.frames[frame]
        entry.t = frm.t > 0 and frm.t or 1
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

cs.edit_inspector.custom['animation'] = {
    add = function (inspector)
        -- current animation
        inspector.curr_anim = cg.edit_field_create {
            field_type = 'enum', parent = inspector.window_body,
            label = 'curr_anim'
        }

        -- animation list
        inspector.anim_views_container = cg.add {
            transform = { parent = inspector.window_body },
            gui = {
                padding = cg.vec2_zero,
                color = cg.color_clear,
                valign = cg.GA_TABLE,
                halign = cg.GA_MIN,
            },
            gui_rect = { hfill = true },
        }
        inspector.anim_views = {}

        -- 'add strip' button
        inspector.add_strip = cg.add {
            transform = { parent = inspector.window_body },
            gui = {
                color = cg.color(0.35, 0.15, 0.30, 1),
                valign = cg.GA_TABLE,
                halign = cg.GA_MID,
            },
            gui_textbox = {},
        }
        cs.gui_text.set_str(cs.gui_textbox.get_text(inspector.add_strip),
                            'add strip')
    end,

    post_update = function (inspector)
        local ent = inspector.ent
        local entry = cs.animation.tbl[ent]
        local anims = entry.anims

        -- current animation
        cg.edit_field_post_update(
            inspector.curr_anim, entry.curr_anim or '',
            function (v) cs.animation.switch(ent, v) end,
            anims)

        -- add strip?
        if cs.gui.event_mouse_down(inspector.add_strip) == cg.MC_LEFT then
            local function new_strip(s)
                local strips = { [s] = { n = 1, t = 1, base = cg.vec2(0, 0) } }
                cs.animation.set_strips(ent, strips)
            end
            cs.edit.command_start('new strip name: ', new_strip)
        end

        -- add missing views
        for name, anim in pairs(anims) do
            if not inspector.anim_views[name] then
                local view = {}
                inspector.anim_views[name] = view

                view.window = cg.add {
                    transform = { parent = inspector.anim_views_container },
                    gui_window = { title = name, minimized = true },
                    gui_rect = { hfill = true },
                    gui = {
                        valign = cg.GA_TABLE,
                        halign = cg.GA_MIN,
                    }
                }
                view.window_body = cs.gui_window.get_body(view.window)

                view.n = cg.edit_field_create {
                    field_type = 'Scalar', parent = view.window_body,
                    label = 'n'
                }
                view.t = cg.edit_field_create {
                    field_type = 'Scalar', parent = view.window_body,
                    label = 't'
                }
                view.base = cg.edit_field_create {
                    field_type = 'Vec2', parent = view.window_body,
                    label = 'base'
                }
            end
        end

        -- remove extra views
        for name, view in pairs(inspector.anim_views) do
            if not anims[name] then
                cs.gui_window.remove(view.window)
                inspector.anim_views[name] = nil
            end
        end

        -- update views
        for name, view in pairs(inspector.anim_views) do
            if cs.entity.destroyed(view.window) then
                cs.animation.remove(ent, name)
            else
                local anim = anims[name]
                cg.edit_field_post_update(
                    view.n, anim.n,
                    function (v) anim.n = v end)
                cg.edit_field_post_update(
                    view.t, anim.strip.t,
                    function (v)
                        anim.strip.t = v
                        entry.t = v > 0 and v or 1
                    end)
                cg.edit_field_post_update(
                    view.base, anim.strip.base,
                    function (v) anim.strip.base = v end)
            end
        end
    end,
}
