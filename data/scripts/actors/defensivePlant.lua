local floaty_projectile = require('archetypes.floaty_projectile')

local defensivePlant = {}

function defensivePlant.shoot(eid)
    local xform = entities:get_component(eid, component.transform)
    local body = entities:get_component(eid, component.body)
    floaty_projectile(xform.pos.x, xform.pos.y + .25 + body.top, 10 * (math.random() * 2 - 1), 10)
end

function defensivePlant.grow(eid, stage)
    local sprite = entities:get_component(eid, component.sprite)
    local body = entities:get_component(eid, component.body)
    
    if(stage == 1) then
        sprite.state = 1
        body.top = 1.0
    elseif(stage == 2) then
        sprite.state = 2
        body.top = 1.0
    elseif(stage == 3) then
        sprite.state = 3
        body.top = 2.0
    end
    
end

return defensivePlant
