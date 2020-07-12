local new_player = require('archetypes.player')
local new_defensive = require('archetypes.defensivePlant')
local new_valuable = require('archetypes.valuablePlant')
local new_red_beetle = require('archetypes.red_beetle')
local new_platform = require('archetypes.platform')
local new_beetle_spawner = require('archetypes.beetle_spawner')

local scene = {}

local function make_ground()
    local transform = component.transform.new()

    local body = component.body.new()
    body.type = 0
    body.layer = 2
    body.left = -999
    body.right = 999
    body.bottom = -10
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
        body.bottom = -10
        body.top = 999

        local eid = entities:create_entity()
        entities:add_component(eid, transform)
        entities:add_component(eid, body)
    end

    spawn_vertical_wall(40)
    spawn_vertical_wall(-40)
end

local function spawn_initial_plants()
    new_defensive((world_width / 2) - 5, 1)
    new_defensive((world_width / 2) - 7, 1)
    new_defensive((-world_width / 2) + 5, 1)
    new_defensive((-world_width / 2) + 7, 1)

    new_valuable((world_width / 2) - 2, 1)
    new_valuable((-world_width / 2) + 2, 1)
end

function scene.init()
    print('Initializing gameplay...')

    new_player()
    make_ground()
    new_beetle_spawner()
    spawn_initial_plants()

    play_bgm('Beeswax', 1)

    print('Generating terrain...')
    for i = 0, 600 do
        local y = math.random(5, 600)
        local w = math.random(2, 20)
        local x1 = math.random(-world_width, world_width - w)
        for x = x1, x1+w do
            new_platform(x, y)
        end
    end

    print('Initialized gameplay.')
end

return scene
