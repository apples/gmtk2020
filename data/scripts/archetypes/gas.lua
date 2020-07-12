return function(x, y, dir)
    local sprite = component.sprite.new()
    sprite.name = 'gas'

    local transform = component.transform.new()
    transform.pos.x = x
    transform.pos.y = y

    local body = component.body.new()
    body.type = 2
    body.layer = 6
    body.left = -0.25
    body.right  = 0.25
    body.bottom = -0.25
    body.top = 0.25
    body.events_with[5] = true
    body.vel = vec2.new(dir * 5, 0)
    body.accel.y = 2
    body.vel_damp = vec2.new(0.5, 0)

    local script = component.script.new()
    script.name = 'gas'

    local eid = entities:create_entity()
    entities:add_component(eid, sprite)
    entities:add_component(eid, transform)
    entities:add_component(eid, body)
    entities:add_component(eid, script)
end
