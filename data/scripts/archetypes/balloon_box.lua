return function(x, y, dir)
    local sprite = component.sprite.new()
    sprite.name = 'balloon_box'

    local transform = component.transform.new()
    transform.pos.x = x
    transform.pos.y = y

    local body = component.body.new()
    body.type = 0
    body.layer = 7
    body.left = -0.5
    body.right  = 0.5
    body.bottom = -0.5
    body.top = 0.5
    body.jump_through = true
    body.carry = true

    local script = component.script.new()
    script.name = 'balloon_box'

    local balloon = component.balloon.new()
    balloon.fruits = get_fruits()
    set_fruits(0)

    local health = component.health.new()
    health.current = 3
    health.max = 3
    health.anim = true

    local eid = entities:create_entity()
    entities:add_component(eid, sprite)
    entities:add_component(eid, transform)
    entities:add_component(eid, body)
    entities:add_component(eid, script)
    entities:add_component(eid, balloon)
    entities:add_component(eid, health)
end
