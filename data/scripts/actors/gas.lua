local gas = {}

function gas.update(eid, delta)
    local sprite = entities:get_component(eid, component.sprite)

    if sprite.state == 2 then
        entities:destroy_entity(eid)
    end
end

function gas.physics_pre_collide(eid, other)
    if not entities:has_component(other, component.plant_tag) then
        local health = entities:get_component(other, component.health)
        if health then
            health.current = health.current - 1
        end
    end
    entities:destroy_entity(eid)
end

return gas
