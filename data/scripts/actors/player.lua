local new_plant = require('archetypes.defensivePlant')

local player = {}

function player.update(eid, delta)
    local controller = entities:get_component(eid, component.controller)
    local body = entities:get_component(eid, component.body)
    local transform = entities:get_component(eid, component.transform)
    local player = entities:get_component(eid, component.player)
    local currency = get_currency()

    local focus_speed = 15

    if controller.left then
        body.vel.x = -15
        transform.scl.x = 1
        player.focus.x = math.max(player.focus.x - delta * focus_speed, -3)
    elseif controller.right then
        body.vel.x = 15
        transform.scl.x = -1
        player.focus.x = math.min(player.focus.x + delta * focus_speed, 3)
    else
        body.vel.x = 0
    end

    if controller.jump_pressed then
        body.vel.y = 10
    end

    if controller.sow_defensive and currency >= 20 then
        --local hasPlantAtLocation = false

        if not plant_at_position(transform.pos.x, transform.pos.y) then
            set_currency(currency - 20)
            body.vel.y = -10--testing

            new_plant(math.floor(transform.pos.x * 2) / 2, math.floor((transform.pos.y - .5) * 2) / 2)
            --local eid = entities.create_entity()
            --entities.add_component(eid, component.plant_tag)
            --entities.add_component(eid, component.defensivePlant)
        end
        
    end
end

return player

