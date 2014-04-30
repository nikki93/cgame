-- normal mode
cs.edit.modes.normal['S-;'] = cs.edit.command_start
cs.edit.modes.normal['u'] = cs.edit.undo

cs.edit.modes.normal['s'] = cs.edit.command_save
cs.edit.modes.normal['l'] = cs.edit.command_load
cs.edit.modes.normal['\''] = cs.edit.command_save_prefab
cs.edit.modes.normal['.'] = cs.edit.command_load_prefab

cs.edit.modes.normal['p'] = cs.edit.pause_toggle
cs.edit.modes.normal['S-p'] = cs.edit.stop

cs.edit.modes.normal['a'] = cs.edit.select_clear
cs.edit.modes.normal['<mouse_1>'] = cs.edit.select_click_single
cs.edit.modes.normal['C-<mouse_1>'] = cs.edit.select_click_multi

cs.edit.modes.normal['x'] = cs.edit.destroy
cs.edit.modes.normal['S-x'] = cs.edit.destroy_rec
cs.edit.modes.normal['S-d'] = cs.edit.duplicate

cs.edit.modes.normal['S-<mouse_1>'] = cs.edit.camera_drag_start
cs.edit.modes.normal['^S-<mouse_1>'] = cs.edit.camera_drag_end
cs.edit.modes.normal['<mouse_3>'] = cs.edit.camera_drag_start
cs.edit.modes.normal['^<mouse_3>'] = cs.edit.camera_drag_end
cs.edit.modes.normal['-'] = cs.edit.camera_zoom_out
cs.edit.modes.normal['='] = cs.edit.camera_zoom_in

cs.edit.modes.normal['g'] = cs.edit.grab_start
cs.edit.modes.normal['r'] = cs.edit.rotate_start
cs.edit.modes.normal['b'] = cs.edit.boxsel_start

cs.edit.modes.normal[','] = cs.edit.command_inspect
cs.edit.modes.normal['S-g'] = cs.edit.command_grid

-- grab mode
cs.edit.modes.grab['<enter>'] = cs.edit.grab_end
cs.edit.modes.grab['<escape>'] = cs.edit.grab_cancel
cs.edit.modes.grab['<mouse_1>'] = cs.edit.grab_end
cs.edit.modes.grab['<mouse_2>'] = cs.edit.grab_cancel
cs.edit.modes.grab['g'] = cs.edit.grab_snap_on
cs.edit.modes.grab['<left>'] = cs.edit.grab_move_left
cs.edit.modes.grab['<right>'] = cs.edit.grab_move_right
cs.edit.modes.grab['<up>'] = cs.edit.grab_move_up
cs.edit.modes.grab['<down>'] = cs.edit.grab_move_down
cs.edit.modes.grab['S-<left>'] = function () cs.edit.grab_move_left(5) end
cs.edit.modes.grab['S-<right>'] = function () cs.edit.grab_move_right(5) end
cs.edit.modes.grab['S-<up>'] = function () cs.edit.grab_move_up(5) end
cs.edit.modes.grab['S-<down>'] = function () cs.edit.grab_move_down(5) end

-- rotate mode
cs.edit.modes.rotate['<enter>'] = cs.edit.rotate_end
cs.edit.modes.rotate['<escape>'] = cs.edit.rotate_cancel
cs.edit.modes.rotate['<mouse_1>'] = cs.edit.rotate_end
cs.edit.modes.rotate['<mouse_2>'] = cs.edit.rotate_cancel

-- boxsel mode
cs.edit.modes.boxsel['<mouse_1>'] = cs.edit.boxsel_begin
cs.edit.modes.boxsel['C-<mouse_1>'] = cs.edit.boxsel_begin
cs.edit.modes.boxsel['^<mouse_1>'] = cs.edit.boxsel_end
cs.edit.modes.boxsel['^C-<mouse_1>'] = cs.edit.boxsel_end_add

-- phypoly mode
cs.edit.modes.phypoly['<enter>'] = cs.edit.phypoly_end
cs.edit.modes.phypoly['<escape>'] = cs.edit.phypoly_cancel
cs.edit.modes.phypoly['<mouse_1>'] = cs.edit.phypoly_add_vertex
