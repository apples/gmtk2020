local new_defensive_plant = require('archetypes.defensivePlant')
local new_valuable_plant = require('archetypes.valuablePlant')

local player = {}

function player.update(eid, delta)
    local controller = entities:get_component(eid, component.controller)
    local body = entities:get_component(eid, component.body)
    local transform = entities:get_component(eid, component.transform)
    local player = entities:get_component(eid, component.player)
    local currency = get_currency()

    local max_speed = 8
    local focus_speed = 5
    local acceleration = 150

    if controller.left then
        local to_max = math.abs(-max_speed - body.vel.x)
        local acc = math.min(to_max, delta * acceleration)
        body.vel.x = body.vel.x - acc
        local fs = focus_speed * delta
        player.focus.x = math.max(player.focus.x - fs, -3)
    elseif controller.right then
        local to_max = math.abs(max_speed - body.vel.x)
        local acc = math.min(to_max, delta * acceleration)
        body.vel.x = body.vel.x + acc
        local fs = focus_speed * delta
        player.focus.x = math.min(player.focus.x + fs, 3)
    else
        local to_max = math.abs(body.vel.x)
        local acc = math.min(to_max, delta * acceleration)
        if body.vel.x > 0 then
            body.vel.x = body.vel.x - acc
        else
            body.vel.x = body.vel.x + acc
        end
        local fs = focus_speed * acc / acceleration
        player.focus.x = player.focus.x * math.pow(1 / focus_speed / focus_speed, delta)
    end

    if controller.jump_pressed then
        body.vel.y = 10
    end

    if controller.sow_defensive and currency >= 20 then
        if not plant_at_position(transform.pos.x, transform.pos.y) then
            set_currency(currency - 20)

            new_defensive_plant(math.floor(transform.pos.x * 2) / 2, math.floor((transform.pos.y - .5) * 2) / 2)
        end
        
    end

    if controller.sow_valuable and currency >= 20 then
        if not plant_at_position(transform.pos.x, transform.pos.y) then
            set_currency(currency - 20)

            new_valuable_plant(math.floor(transform.pos.x * 2) / 2, math.floor((transform.pos.y - .5) * 2) / 2)
        end
        
    end
end

return player

