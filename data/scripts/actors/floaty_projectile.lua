local floaty_projectile = {}

function floaty_projectile.physics_pre_collide(eid, other)
    if not entities:has_component(other, component.plant_tag) then
        local health = entities:get_component(other, component.health)
        if health then
            health.current = health.current - 1
            play_sfx('hurt', 1)
        end
    end
    entities:destroy_entity(eid)
end

return floaty_projectile
