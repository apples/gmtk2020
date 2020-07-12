local vdom = require('vdom')

-- health
-- max_health
local function hearts(props)
    local icons = {}
    for i = 1, props.health do
        icons[i] = vdom.create_element('panel', {
            texture = 'ui_heart',
            width = 16 * 3,
            height = 16 * 3,
            halign = 'left',
            valign = 'top',
            left = 16 * 3 + 19 * (i-1) * 3,
            top = 16,
        })
    end
    for i = props.health + 1, props.max_health do
        icons[i] = vdom.create_element('panel', {
            texture = 'ui_heart_black',
            width = 16 * 3,
            height = 16 * 3,
            halign = 'left',
            valign = 'top',
            left = 16 * 3 + 19 * (i-1) * 3,
            top = 16,
        })
    end
    return vdom.create_element('widget', { width = '100%', height = '100%' },
        icons
    )
end

-- health
-- max_health
-- currency
-- fruits
return function(props)
    return vdom.create_element('widget', { width = '100%', height = '100%' },
        vdom.create_element(hearts, props),
        vdom.create_element('label', {
            color = '#f00',
            text = '$' .. props.currency,
            height = 32,
            valign = 'top',
            halign = 'left',
        }),
        vdom.create_element('label', {
            color = '#f00',
            text = 'Fruit: ' .. props.fruits,
            height = 32,
            valign = 'top',
            halign = 'center',
        })
    )
end

