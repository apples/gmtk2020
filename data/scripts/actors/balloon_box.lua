local balloon_box = {}

function balloon_box.update(eid, delta)
    local body = entities:get_component(eid, component.body)
    local balloon = entities:get_component(eid, component.balloon)
    local transform = entities:get_component(eid, component.transform)

    if balloon.air >= 9 then
        if body.accel.x == 0 then
            body.accel.x = math.random() * 2 - 1
            body.accel.y = 4
        end
        body.accel.x = body.accel.x * math.pow(2, delta)
    end

    if math.abs(transform.pos.x) > world_width / 2 then
        set_currency(get_currency() + balloon.fruits * 10)
        add_delivered(balloon.fruits)
        entities:destroy_entity(eid)
    end
end

return balloon_box
