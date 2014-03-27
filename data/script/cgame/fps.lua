cs.fps = {}

-- number of frames and elapsed time per round
local nframes = 0;
local timer = 0;

-- display gui entity
local display = cg.add {
    group = { groups = 'builtin' },
    edit = { editable = false },
    gui_text = { str = 'fps: ...' },
    gui = { color = cg.color(0, 0.4, 0.1, 1) },
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
    
    -- update display position
    local winsize = cs.game.get_window_size()
    local len = #cg.string(cs.gui_text.get_str(display))
    cs.transform.set_position(display, cg.vec2(winsize.x - 10 * len,
                                                   -winsize.y + 12))
end
