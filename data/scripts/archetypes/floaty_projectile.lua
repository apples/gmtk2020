return function(x, y, vx, vy)
    local sprite = component.sprite.new()
    sprite.name = 'projectile';

    local transform = component.transform.new()
    transform.pos = vec2.new(x, y)

    local body = component.body.new()
    local hbr = 1/16
    body.type = 2
    body.layer = 4
    body.left = -hbr
    body.right = hbr
    body.bottom = -hbr
    body.top = hbr
    body.collides_with[1] = true
    body.collides_with[2] = true
    body.collides_with[5] = true
    body.events_with[1] = true
    body.events_with[2] = true
    body.events_with[5] = true
    body.events_with[7] = true
    body.vel = vec2.new(vx, vy)
    body.accel.y = -5
    body.vel_damp = vec2.new(0.5, 0.5)

    local script = component.script.new()
    script.name = 'floaty_projectile'

    local eid = entities:create_entity()
    entities:add_component(eid, sprite)
    entities:add_component(eid, transform)
    entities:add_component(eid, body)
    entities:add_component(eid, script)
end
