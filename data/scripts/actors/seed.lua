local new_plant = require('archetypes.defensivePlant')

local seed = {}

function seed.physics_post_collide(eid, other)
    local transform = entities:get_component(eid, component.transform)
    local body = entities:get_component(eid, component.body)
    if body.on_ground then
        local existing = plant_at_position(transform.pos.x, transform.pos.y + 0.5)
        if not existing then
            local sx = math.floor(transform.pos.x * 2) / 2
            local sy = math.floor(transform.pos.y * 2) / 2
            new_plant(sx, sy)
            play_sfx('plant', 1)
        end
        entities:destroy_entity(eid)
    end
end

return seed
