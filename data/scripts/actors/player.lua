local new_plant = require('archetypes.defensivePlant')

local player = {}

local function set_idle(eid)
    local sprite = entities:get_component(eid, component.sprite)
    if sprite.texture ~= 'player' then
        sprite.texture = 'player'
        sprite.uv_size = vec2.new(1, 1)
        sprite.frames:clear()
        sprite.frames:add(0)
        sprite.time = 0
    end
end

local function set_walk(eid)
    local sprite = entities:get_component(eid, component.sprite)
    if sprite.texture ~= 'player_walk' then
        sprite.texture = 'player_walk'
        sprite.uv_size = vec2.new(0.5, 0.5)
        sprite.frames:clear()
        sprite.frames:add(0)
        sprite.frames:add(1)
        sprite.frames:add(2)
        sprite.frames:add(3)
        sprite.time = 0
    end
end

function player.update(eid, delta)
    local controller = entities:get_component(eid, component.controller)
    local body = entities:get_component(eid, component.body)
    local transform = entities:get_component(eid, component.transform)
    local player = entities:get_component(eid, component.player)
    local currency = get_currency()

    local focus_speed = 15

    if controller.left then
        set_walk(eid)
        body.vel.x = -15
        transform.scl.x = 1
        player.focus.x = math.max(player.focus.x - delta * focus_speed, -3)
    elseif controller.right then
        set_walk(eid)
        body.vel.x = 15
        transform.scl.x = -1
        player.focus.x = math.min(player.focus.x + delta * focus_speed, 3)
    else
        set_idle(eid)
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

