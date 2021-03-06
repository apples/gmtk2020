return function(x, y)
    local sprite = component.sprite.new()
    sprite.name = 'platform'

    local transform = component.transform.new()
    transform.pos.x = x
    transform.pos.y = y

    local body = component.body.new()
    body.type = 0
    body.layer = 2
    body.left = -0.5
    body.right  = 0.5
    body.bottom = -0.5
    body.top = 0.5
    body.jump_through = true

    local eid = entities:create_entity()
    entities:add_component(eid, sprite)
    entities:add_component(eid, transform)
    entities:add_component(eid, body)
end
