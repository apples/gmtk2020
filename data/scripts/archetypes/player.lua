return function()
    local sprite = component.sprite.new()
    sprite.name = 'player';

    local transform = component.transform.new()
    transform.pos.y = 2
    transform.pos.z = 1

    local body = component.body.new()
    body.type = 2
    body.layer = 1
    body.left = -0.25
    body.right = 0.25
    body.bottom = -0.5
    body.top = 0.5
    body.collides_with[2] = true
    body.collides_with[7] = true
    body.accel.y = -10

    local controller = component.controller.new()

    local script = component.script.new()
    script.name = 'player'

    local player = component.player.new()

    local balloon_tracker = component.balloon_tracker.new()

    local health = component.health.new()
    health.max = 5
    health.current = 5
    health.anim = true

    local eid = entities:create_entity()
    entities:add_component(eid, sprite)
    entities:add_component(eid, transform)
    entities:add_component(eid, body)
    entities:add_component(eid, controller)
    entities:add_component(eid, script)
    entities:add_component(eid, player)
    entities:add_component(eid, balloon_tracker)
    entities:add_component(eid, health)
end
