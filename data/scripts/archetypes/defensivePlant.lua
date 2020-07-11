return function()
    local sprite = component.sprite.new()
    sprite.texture = 'DefensePlant1';
    sprite.size.y = 1/1
    sprite.frames:add(0)

    local transform = component.transform.new()
    transform.pos.y = 4

    local body = component.body.new()
    body.type = 2
    body.layer = 1
    body.size.x = 1
    body.size.y = 1/4
    --body.collides_with[3] = true
    --body.accel.y = -10

    local controller = component.controller.new()

    local script = component.script.new()
    script.name = 'defensivePlant'

    local defensivePlant = component.defensivePlant.new()

    local eid = entities:create_entity()
    entities:add_component(eid, sprite)
    entities:add_component(eid, transform)
    entities:add_component(eid, body)
    entities:add_component(eid, controller)
    entities:add_component(eid, script)
    entities:add_component(eid, defensivePlant)
end
