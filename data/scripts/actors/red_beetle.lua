local red_beetle = {}

function red_beetle.update(eid, delta)
    local body = entities:get_component(eid, component.body)
    local sprite = entities:get_component(eid, component.sprite)
    local transform = entities:get_component(eid, component.transform)
    local targeting = entities:get_component(eid, component.targeting)

    local max_speed = 5
    local acceleration = 3
    local acceleration_brakes = 3

    if body.vel.x < 0 then
        sprite.scale.x = math.abs(sprite.scale.x) * -1
    elseif body.vel.x > 0 then
        sprite.scale.x = math.abs(sprite.scale.x)
    end

    if targeting.target and entities:exists(targeting.target) then
        local target_transform = entities:get_component(targeting.target, component.transform)

        local dir = (target_transform.pos - transform.pos) * 0.5
        dir.y = dir.y + dir:length() / 2

        local ideal_vel = dir:normalized() * max_speed

        local accel = (ideal_vel - body.vel):normalized()

        local wrong_way = accel:dot(body.vel) < 0

        local new_vel = body.vel + accel * delta * acceleration * (wrong_way and acceleration_brakes or 1)

        if new_vel:length() > max_speed then
            new_vel = new_vel:normalized() * max_speed
        end

        body.vel = new_vel
    else
        body.vel = body.vel * 0.95 * delta
    end
end

function red_beetle.physics_post_collide(eid, other)
    if entities:has_component(other, component.plant_tag) then
        entities:destroy_entity(other)
    end
end

return red_beetle
