local floaty_projectile = require('archetypes.floaty_projectile')
local seed = require('archetypes.seed')

local defensivePlant = {}

function defensivePlant.shoot(eid)
    local xform = entities:get_component(eid, component.transform)
    local body = entities:get_component(eid, component.body)

    floaty_projectile(xform.pos.x, xform.pos.y + .25 + body.top, 10 * (math.random() * 2 - 1), 10)
end

function defensivePlant.seed(eid)
    local xform = entities:get_component(eid, component.transform)
    local body = entities:get_component(eid, component.body)

    seed(xform.pos.x, xform.pos.y + .25 + body.top, 3 * (math.random() * 2 - 1), 10)
end

function defensivePlant.grow(eid, stage)
    local sprite = entities:get_component(eid, component.sprite)
    local body = entities:get_component(eid, component.body)
    local shooter = entities:get_component(eid, component.shooter)
    local growth = entities:get_component(eid, component.growth)
    
    if(stage == 0) then
        sprite.state = 0
        body.top = 0.25
        shooter.cooldown = 4
        growth.growTime = 5
        if entities:has_component(eid, component.seeder) then
            entities:remove_component(eid, component.seeder)
        end
    elseif(stage == 1) then
        sprite.state = 1
        body.top = 1.0
        shooter.cooldown = 2
        growth.growTime = 10
        if entities:has_component(eid, component.seeder) then
            entities:remove_component(eid, component.seeder)
        end
    elseif(stage == 2) then
        sprite.state = 2
        body.top = 1.0
        shooter.cooldown = 1
        growth.growTime = 15
        if entities:has_component(eid, component.seeder) then
            entities:remove_component(eid, component.seeder)
        end
    elseif(stage == 3) then
        sprite.state = 3
        body.top = 2.0
        shooter.cooldown = 0.5
        growth.growTime = 20

        local seeder = component.seeder.new()
        seeder.cooldown = 4
        seeder.cooldown_timer = 4
        entities:add_component(eid, seeder)
    end
end

return defensivePlant
