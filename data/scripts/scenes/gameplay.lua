local new_player = require('archetypes.player')

local scene = {}

local function make_ground()
    local transform = component.transform.new()

    local body = component.body.new()
    body.type = 0
    body.layer = 2
    body.size.x = 999
    body.size.y = 2

    local eid = entities:create_entity()
    entities:add_component(eid, transform)
    entities:add_component(eid, body)

    local function spawn_vertical_wall(x)
        local transform = component.transform.new()
        transform.pos.x = x

        local body = component.body.new()
        body.type = 0
        body.layer = 2
        body.size.x = 2
        body.size.y = 999

        local eid = entities:create_entity()
        entities:add_component(eid, transform)
        entities:add_component(eid, body)
    end

    spawn_vertical_wall(30)
    spawn_vertical_wall(-30)
end

function scene.init()
    print('Initializing gameplay...')

    new_player()
    make_ground()

    local sprite = component.sprite.new()
    sprite.texture = 'player';
    sprite.frames:add(0)

    local transform = component.transform.new()

    local eid = entities:create_entity()
    entities:add_component(eid, sprite)
    entities:add_component(eid, transform)

    print('Initialized gameplay.')
end

return scene
