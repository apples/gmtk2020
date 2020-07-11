return function(x, y)
    local sprite = component.sprite.new()
    sprite.name = 'red_beetle'

    local transform = component.transform.new()
    transform.pos.x = x
    transform.pos.y = y

    local body = component.body.new()
    body.type = 1
    body.layer = 1
    body.left = -0.25
    body.right  = 0.25
    body.bottom = -0.25
    body.top = 0.25
    --body.collides_with[3] = true
    --body.accel.y = -10

    local script = component.script.new()
    script.name = 'red_beetle'

    local targeting = component.targeting.new()

    local health = component.health.new()
    health.current = 3
    health.max = 3

    local eid = entities:create_entity()
    entities:add_component(eid, sprite)
    entities:add_component(eid, transform)
    entities:add_component(eid, body)
    entities:add_component(eid, script)
    entities:add_component(eid, targeting)
    entities:add_component(eid, health)
end
