local new_player = require('archetypes.player')

local scene = {}

local function make_ground()
    local transform = component.transform.new()

    local body = component.body.new()
    body.type = 0
    body.layer = 2
    body.left = -999
    body.right = 999
    body.bottom = -999
    body.top = 1

    local eid = entities:create_entity()
    entities:add_component(eid, transform)
    entities:add_component(eid, body)

    local function spawn_vertical_wall(x)
        local transform = component.transform.new()
        transform.pos.x = x

        local body = component.body.new()
        body.type = 0
        body.layer = 2
        body.left = -1
        body.right = 1
        body.bottom = -999
        body.top = 999

        local eid = entities:create_entity()
        entities:add_component(eid, transform)
        entities:add_component(eid, body)
    end

    spawn_vertical_wall(40)
    spawn_vertical_wall(-40)
end

function scene.init()
    print('Initializing gameplay...')

    new_player()
    make_ground()

    print('Initialized gameplay.')
end

return scene
