local floaty_projectile = {}

function floaty_projectile.update(eid, delta)
    local body = entities:get_component(eid, component.body)
    local transform = entities:get_component(eid, component.transform)

end

function floaty_projectile.physics_pre_collide(eid, other)
    if not entities:has_component(other, component.plant_tag) then
        local health = entities:get_component(other, component.health)
        if health then
            health.current = health.current - 1
            entities:destroy_entity(eid)
        end
    end
end

return floaty_projectile
