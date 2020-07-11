local floaty_projectile = require('archetypes.floaty_projectile')

local defensivePlant = {}

function defensivePlant.shoot(eid)
    local xform = entities:get_component(eid, component.transform)
    floaty_projectile(xform.pos.x, xform.pos.y + 1, 10 * (math.random() * 2 - 1), 10)
end

function defensivePlant.grow(eid, stage)
    local sprite = entities:get_component(eid, component.sprite)
    
    if(stage < 4) then
        sprite.state = stage;
    end
    -- if(stage == 1) then
    --     sprite.state = 1
    -- elseif(stage == 2) then
    --     sprite.state = 2
    -- elseif(stage == 3) then
    --     sprite.state = 3
    -- end

end

return defensivePlant
