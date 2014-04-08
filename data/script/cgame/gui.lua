local root = cs.gui.get_root()
cs.group.set_groups(root, 'builtin')


--- event ----------------------------------------------------------------------

cs.gui_event = {}

local event_handlers = cg.entity_table()
local event_defaults = {}

function cs.gui_event.add()
end

local function add_event(event, default)
    event_defaults[event] = default

    cs.gui_event['set_' .. event] = function (ent, f)
        if not event_handlers[ent] then
            event_handlers[ent] = {}
        end
        event_handlers[ent][event] = f
    end
end

add_event('mouse_down', cg.MC_NONE)
add_event('mouse_up', cg.MC_NONE)

function cs.gui_event.update_all()
    for ent, _ in pairs(event_handlers) do
        if cs.entity.destroyed(ent) then event_handlers[ent] = nil end
    end

    for ent, handlers in pairs(event_handlers) do
        for event, f in pairs(handlers) do
            local r = cs.gui['event_' .. event](ent)
            if r ~= event_defaults[event] then
                f(ent, r)
            end
        end
    end
end
