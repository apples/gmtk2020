local valuablePlant = {}

function valuablePlant.grow(eid, stage)
    local sprite = entities:get_component(eid, component.sprite)
    local body = entities:get_component(eid, component.body)
    
    if(stage == 0) then
        sprite.state = 0
    elseif(stage == 1) then
        sprite.state = 1
    end
    
end

return valuablePlant
