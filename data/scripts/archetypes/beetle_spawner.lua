return function()
    local script = component.script.new()
    script.name = 'beetle_spawner'

    local timer = component.timer.new()
    timer.time = 5

    local eid = entities:create_entity()
    entities:add_component(eid, script)
    entities:add_component(eid, timer)
end
