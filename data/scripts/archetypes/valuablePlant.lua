return function(x, y)
    local sprite = component.sprite.new()
    sprite.name = 'valuablePlant'

    local transform = component.transform.new()
    transform.pos.x = x
    transform.pos.y = y

    local body = component.body.new()
    body.type = 0
    body.layer = 1
    body.left = -0.25
    body.right  = 0.25
    body.bottom = 0
    body.top = 0.25
    --body.collides_with[3] = true
    --body.accel.y = -10

    local script = component.script.new()
    script.name = 'valuablePlant'

    local shooter = component.shooter.new()
    
    local growth = component.growth.new()

    local eid = entities:create_entity()
    entities:add_component(eid, sprite)
    entities:add_component(eid, transform)
    entities:add_component(eid, body)
    entities:add_component(eid, script)
    entities:add_component(eid, shooter)
    entities:add_component(eid, growth)
    entities:add_component(eid, component.plant_tag.new())
end