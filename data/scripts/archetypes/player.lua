return function()
    local sprite = component.sprite.new()
    sprite.name = 'player';

    local transform = component.transform.new()
    transform.pos.y = 2

    local body = component.body.new()
    body.type = 2
    body.layer = 0
    body.left = -0.25
    body.right = 0.25
    body.bottom = -0.5
    body.top = 0.5
    body.collides_with[2] = true
    body.accel.y = -10

    local controller = component.controller.new()

    local script = component.script.new()
    script.name = 'player'

    local player = component.player.new()

    local eid = entities:create_entity()
    entities:add_component(eid, sprite)
    entities:add_component(eid, transform)
    entities:add_component(eid, body)
    entities:add_component(eid, controller)
    entities:add_component(eid, script)
    entities:add_component(eid, player)
end
