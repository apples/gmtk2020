local floaty_projectile = require('archetypes.floaty_projectile')

local defensivePlant = {}

function defensivePlant.shoot(eid)
    local xform = entities:get_component(eid, component.transform)
    floaty_projectile(xform.pos.x, xform.pos.y + 1, 10 * (math.random() * 2 - 1), 10)
end

return defensivePlant
