
local player = {}

function player.update(eid, delta)
    local controller = entities:get_component(eid, component.controller)
    local body = entities:get_component(eid, component.body)
    local transform = entities:get_component(eid, component.transform)
    local player = entities:get_component(eid, component.player)

    if controller.left then
        body.vel.x = -15
        transform.scl.x = 1
        player.focus.x = math.max(player.focus.x - delta * 40, -3)
    elseif controller.right then
        body.vel.x = 15
        transform.scl.x = -1
        player.focus.x = math.min(player.focus.x + delta * 40, 3)
    else
        body.vel.x = 0
    end

    if controller.jump_pressed then
        body.vel.y = 10
    end

    if controller.sow_defensive then--and currency > 20 then
        --set_currency(currency - 20)
        body.vel.y = -10
    end
end

return player

