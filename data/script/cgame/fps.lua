cs.fps = {}

-- number of frames and elapsed time per round
local nframes = 0;
local timer = 0;

-- display gui entity
local display = cg.add {
    group = { groups = 'builtin' },
    edit = { editable = false },
    gui_text = { str = 'fps: ...' },
    gui = {
        color = cg.color(0, 0.4, 0.1, 1),
        halign = cg.GA_MAX, valign = cg.GA_MIN,
        padding = cg.vec2(1, 1),
    },
}

function cs.fps.update_all()
    -- timing
    nframes = nframes + 1
    timer = timer + cs.timing.true_dt
    if timer > 2.5 then -- recalculate every 2.5 seconds
        cs.gui_text.set_str(display, string.format('fps: %.2f',
                                                   nframes / timer))
        timer = 0
        nframes = 0
    end
end
