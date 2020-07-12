local linq = require('linq')

local visitor = {}

--[[
function visitor.visit(entities, coms, func)
    entities:visit(
        function (eid)
            local first_missing = linq(coms)
                :where(function (com) return not entities:has_component(eid, com) end)
                :first()

            if first_missing then
                return
            end

            local com_vals = linq(coms)
                :select(function (com) return entities:get_component(eid, com) end)
                :tolist()

            func(eid, table.unpack(com_vals))
        end
    )
end
]]

function visitor.visit(entities, coms, func)
    if #coms > 0 then
        local first = coms[1]
        local rest = linq(coms):drop(1):tolist()

        first._visit(entities, function (eid, first_val)
            local first_missing = linq(rest)
                :where(function (com) return not entities:has_component(eid, com) end)
                :first()

            if first_missing then
                return
            end

            local com_vals = linq(rest)
                :select(function (com) return entities:get_component(eid, com) end)
                :tolist()

            func(eid, first_val, table.unpack(com_vals))
        end)
    else
        entities:visit(func)
    end
end

return visitor
