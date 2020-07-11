local floaty_projectile = {}

function floaty_projectile.update(eid, delta)
    local body = entities:get_component(eid, component.body)
    local transform = entities:get_component(eid, component.transform)

end

function floaty_projectile.physics_pre_collide(eid, other)
end

return floaty_projectile
