local new_beetle = require('archetypes.red_beetle')

local beetle_spawner = {}

function beetle_spawner.timer(eid, timer)
    timer.time = math.random() * 10 + 10
    local x = (world_width/2 - 2) * (math.random() > 0 and 1 or -1)
    local y = math.random() * 200 + 5
    new_beetle(x, y)
end

return beetle_spawner
