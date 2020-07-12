local new_defensive_plant = require('archetypes.defensivePlant')
local new_valuable_plant = require('archetypes.valuablePlant')
local new_gas = require('archetypes.gas')
local new_balloon_box = require('archetypes.balloon_box')

local player = {}

function player.update(eid, delta)
    local controller = entities:get_component(eid, component.controller)
    local body = entities:get_component(eid, component.body)
    local transform = entities:get_component(eid, component.transform)
    local player = entities:get_component(eid, component.player)
    local sprite = entities:get_component(eid, component.sprite)
    local tracker = entities:get_component(eid, component.balloon_tracker)
    local currency = get_currency()
    local fruits = get_fruits()

    local max_speed = 8
    local focus_speed = 5
    local acceleration = 150

    local is_acting =
        sprite.state == 2 and sprite.time < 4/24 or
        sprite.state == 3 and sprite.time < 4/24
    
    if not is_acting and controller.left then
        local to_max = math.abs(-max_speed - body.vel.x)
        local acc = math.min(to_max, delta * acceleration)
        body.vel.x = body.vel.x - acc
    elseif not is_acting and controller.right then
        local to_max = math.abs(max_speed - body.vel.x)
        local acc = math.min(to_max, delta * acceleration)
        body.vel.x = body.vel.x + acc
    else
        local to_max = math.abs(body.vel.x)
        local acc = math.min(to_max, delta * acceleration)
        if body.vel.x > 0 then
            body.vel.x = body.vel.x - acc
        else
            body.vel.x = body.vel.x + acc
        end
    end

    if controller.left then
        local to_max = math.abs(-max_speed - body.vel.x)
        local acc = math.min(to_max, delta * acceleration)
        local fs = focus_speed * delta
        player.focus.x = math.max(player.focus.x - fs, -3)
    elseif controller.right then
        local to_max = math.abs(max_speed - body.vel.x)
        local acc = math.min(to_max, delta * acceleration)
        local fs = focus_speed * delta
        player.focus.x = math.min(player.focus.x + fs, 3)
    else
        local to_max = math.abs(body.vel.x)
        local acc = math.min(to_max, delta * acceleration)
        local fs = focus_speed * acc / acceleration
        player.focus.x = player.focus.x * math.pow(1 / focus_speed / focus_speed, delta)
    end

    if body.on_ground and controller.jump_pressed then
        body.vel.y = 10
    end

    if not is_acting and controller.action_pressed then
        sprite.state = 2
        sprite.time = 0
        local gasdir = sprite.flip and 0.5 or -0.5
        new_gas(gasdir + transform.pos.x, transform.pos.y, gasdir * 2)
    end

    if not is_acting and body.on_ground and controller.pump_pressed then
        if tracker.tracked then
            sprite.state = 3
            sprite.time = 0
            local balloon = entities:get_component(tracker.tracked, component.balloon)
            if balloon.air < 9 then
                balloon.air = balloon.air + 1
            end
            if balloon.air == 9 then
                set_currency(currency + (10 * fruits))
                set_fruits(0)
            end
        else
            new_balloon_box(transform.pos.x, transform.pos.y)
        end
    end

    if body.on_ground and controller.sow_defensive and currency >= 20 then
        if not plant_at_position(transform.pos.x, transform.pos.y) then
            set_currency(currency - 20)

            new_defensive_plant(math.floor(transform.pos.x * 2) / 2, math.floor((transform.pos.y - .5) * 2) / 2)
        end
    end

    if body.on_ground and controller.sow_valuable and currency >= 20 then
        if not plant_at_position(transform.pos.x, transform.pos.y) and
            not plant_at_position(transform.pos.x + .5, transform.pos.y) and
            not plant_at_position(transform.pos.x - .5, transform.pos.y) then
            set_currency(currency - 20)

            new_valuable_plant(math.floor(transform.pos.x * 2) / 2, math.floor((transform.pos.y - .5) * 2) / 2)
        end
    end

    if controller.collect and fruits < 10 then
        local eid = pick_fruit(transform.pos.x, transform.pos.y)

        -- if eid then
        --     set_fruits = fruits + 1;
        -- end
    end
end

return player

