--- custom inspector -----------------------------------------------------------

local function add_circle(ent, bbox)
    local r = 0.5 * math.min(bbox.max.x - bbox.min.x, bbox.max.y - bbox.min.y)
    local mid = 0.5 * (bbox.max + bbox.min)

    local function add_r(rs)
        local function add_midx(midxs)
            local function add_midy(midys)
                cs.physics.shape_add_circle(ent, tonumber(rs) or 0,
                                            cg.vec2(tonumber(midxs) or 0,
                                                    tonumber(midys) or 0))
            end
            cs.edit.command_start('offset y: ', add_midy, nil, false,
                                  tostring(mid.y))
        end
        cs.edit.command_start('offset x: ', add_midx, nil, false,
                              tostring(mid.x))
    end

    cs.edit.command_start('radius: ', add_r, nil, false, tostring(r))
end

local function add_box(ent, bbox)
    local mindim = math.min(bbox.max.x - bbox.min.x, bbox.max.y - bbox.min.y)

    local function add(s)
        -- reduce bbox size to account for radius, then add
        local r = math.min(tonumber(s) or 0, 0.5 * mindim)
        local bbox2 = cg.bbox(cg.vec2(bbox.min.x + r, bbox.min.y + r),
                              cg.vec2(bbox.max.x - r, bbox.max.y - r))
        cs.physics.shape_add_box(ent, bbox2, r)
    end

    cs.edit.command_start('rounding radius: ', add)
end

local function add_poly(ent)
    local function add(s)
        cs.edit.phypoly_start(ent, tonumber(s) or 0)
    end
    cs.edit.command_start('rounding radius: ', add)
end

cs.edit_inspector.custom['physics'] = {
    add = function (inspector)
        -- add buttons
        inspector.add_container = cg.add {
            transform = { parent = inspector.window_body },
            gui = {
                padding = cg.vec2_zero,
                color = cg.color_clear,
                valign = cg.GA_TABLE,
                halign = cg.GA_MIN,
            },
            gui_rect = { hfill = true },
        }

        inspector.add_box = cg.add {
            transform = { parent = inspector.add_container },
            gui = {
                color = cg.color(0.35, 0.15, 0.30, 1),
                valign = cg.GA_MAX,
                halign = cg.GA_TABLE,
            },
            gui_textbox = {},
        }
        cs.gui_text.set_str(cs.gui_textbox.get_text(inspector.add_box),
                            'add box')

        inspector.add_poly = cg.add {
            transform = { parent = inspector.add_container },
            gui = {
                color = cg.color(0.35, 0.15, 0.30, 1),
                valign = cg.GA_MAX,
                halign = cg.GA_TABLE,
            },
            gui_textbox = {},
        }
        cs.gui_text.set_str(cs.gui_textbox.get_text(inspector.add_poly),
                            'add poly')


        inspector.add_circle = cg.add {
            transform = { parent = inspector.add_container },
            gui = {
                color = cg.color(0.35, 0.15, 0.30, 1),
                valign = cg.GA_MAX,
                halign = cg.GA_TABLE,
            },
            gui_textbox = {},
        }
        cs.gui_text.set_str(cs.gui_textbox.get_text(inspector.add_circle),
                            'add circle')

        inspector.shapes = {}
    end,

    post_update = function (inspector)
        cs.physics.debug_draw(inspector.ent)

        -- 'add box' button
        if cs.gui.event_mouse_down(inspector.add_box) == cg.MC_LEFT then
            local bbox
            if cs.edit.bboxes_has(inspector.ent) then
                bbox = cs.edit.bboxes_get(inspector.ent)
            else
                bbox = cg.bbox(cg.vec2(-1, -1), cg.vec2(1, 1))
            end
            add_box(inspector.ent, bbox)
            cs.edit.undo_save()
        end

        -- 'add poly' button
        if cs.gui.event_mouse_down(inspector.add_poly) == cg.MC_LEFT then
            add_poly(inspector.ent)
        end

        -- 'add circle' button
        if cs.gui.event_mouse_down(inspector.add_circle) == cg.MC_LEFT then
            local bbox
            if cs.edit.bboxes_has(inspector.ent) then
                bbox = cs.edit.bboxes_get(inspector.ent)
            else
                bbox = cg.bbox(cg.vec2(-1, -1), cg.vec2(1, 1))
            end
            add_circle(inspector.ent, bbox)
            cs.edit.undo_save()
        end

        local nshapes = cs.physics.get_num_shapes(inspector.ent)

        while nshapes > #inspector.shapes do
            local shape = {}
            table.insert(inspector.shapes, shape)
            shape.window = cg.add {
                transform = { parent = inspector.window_body },
                gui_window = {},
                gui_rect = { hfill = true },
                gui = {
                    valign = cg.GA_TABLE,
                    halign = cg.GA_MIN,
                }
            }
            shape.window_body = cs.gui_window.get_body(shape.window)

            shape.poly_container = cg.add {
                transform = { parent = shape.window_body },
                gui = {
                    padding = cg.vec2_zero,
                    color = cg.color_clear,
                    valign = cg.GA_TABLE,
                    halign = cg.GA_MIN,
                },
                gui_rect = { hfill = true },
            }

            shape.poly_info = cg.add {
                transform = { parent = shape.poly_container },
                gui = {
                    color = cg.color_white,
                    valign = cg.GA_MID,
                    halign = cg.GA_TABLE,
                },
                gui_text = {},
            }
        end

        while nshapes < #inspector.shapes do
            local shape = table.remove(inspector.shapes)
            cs.gui_window.remove(shape.window)
        end

        for i = #inspector.shapes, 1, -1 do -- backwards for safe remove
            local shape = inspector.shapes[i]
            if cs.entity.destroyed(shape.window) then
                cs.physics.shape_remove(inspector.ent, i - 1)
                table.remove(inspector.shapes, i)
            else
                local t = cs.physics.shape_get_type(inspector.ent, i - 1)
                cs.gui_window.set_title(shape.window,
                                        cg.enum_tostring('PhysicsShape', t))
                if t == cg.PS_POLYGON then
                    local n = cs.physics.poly_get_num_verts(inspector.ent,
                                                            i - 1)
                    cs.gui_text.set_str(shape.poly_info, n .. ' vertices')
                end
            end
        end
    end,
}


--- phypoly mode (draw polygon shape) ------------------------------------------

local phypoly_ent, phypoly_verts, phypoly_radius

cs.edit.modes.phypoly = {}

local function phypoly_update_verts()
    if #phypoly_verts >= 4 then
        phypoly_verts = cs.physics.convex_hull(phypoly_verts)
    end
end

function cs.edit.phypoly_start(ent, radius)
    cs.edit.set_mode('phypoly')
    phypoly_ent = ent
    phypoly_verts = {}
    phypoly_radius = radius or 0
end
function cs.edit.phypoly_end()
    if #phypoly_verts < 3 then return end

    cs.edit.set_mode('normal')
    cs.physics.shape_add_poly(phypoly_ent, phypoly_verts, phypoly_radius)
    phypoly_ent = nil
    phypoly_verts = nil
    cs.edit.undo_save()
end
function cs.edit.phypoly_cancel()
    cs.edit.set_mode('normal')
    phypoly_ent = nil
    phypoly_verts = nil
end

function cs.edit.phypoly_add_vertex()
    local m = cs.camera.unit_to_world(cs.input.get_mouse_pos_unit())
    -- TODO: remove scaling issue
    local t = cg.mat3_inverse(cs.transform.get_world_matrix(phypoly_ent))
    table.insert(phypoly_verts, cg.Vec2(cg.mat3_transform(t, m)))
    phypoly_update_verts()
end

function cs.edit.modes.phypoly.enter()
    cs.edit.set_mode_text('phypoly')
end

function cs.edit.modes.phypoly.update_all()
    if not cs.physics.has(phypoly_ent) then cs.edit.phypoly_cancel() end

    if #phypoly_verts > 1 then
        for i = 1, #phypoly_verts do
            local j = i < #phypoly_verts and i + 1 or 1
            local a = cs.transform.local_to_world(phypoly_ent,
                                                  phypoly_verts[i])
            local b = cs.transform.local_to_world(phypoly_ent,
                                                  phypoly_verts[j])
            cs.edit.line_add(a, b, 5, cg.color(1.0, 0.0, 0.0, 1.0))
        end
    end
end
