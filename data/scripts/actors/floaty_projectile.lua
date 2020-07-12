local floaty_projectile = {}

function floaty_projectile.physics_pre_collide(eid, other)
    if not entities:has_component(other, component.plant_tag) then
        local health = entities:get_component(other, component.health)
        if health and not health.invulnerable then
            health.current = health.current - 1
            health.invulnerable_time = 1
            play_sfx('hurt', 1)
            entities:destroy_entity(eid)
        end
    end
end

function floaty_projectile.physics_post_collide(eid, other)
    local body = entities:get_component(eid, component.body)
    if body.on_ground then
        entities:destroy_entity(eid)
    end
end

return floaty_projectile
