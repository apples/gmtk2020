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

-- fruits
local function fruit_panel(props)
    return vdom.create_element('panel', {
            texture = 'ui_fruit_panel',
            width = 128 * 3,
            height = 32 * 3,
            valign = 'top',
            top = 0,
            halign = 'center',
        },
            vdom.create_element('label', {
                height = 30 * 3,
                color = props.fruits < 10 and '#cfd5d6' or '#ff4546',
                text = props.fruits,
                valign = 'top',
                bottom = -34 * 3,
                halign = 'left',
                left = 52 * 3,
            }),
            props.fruits == 10 and vdom.create_element('label', {
                height = 15 * 3,
                color = '#ff4546',
                text = '(max)',
                valign = 'top',
                bottom = -8 * 3,
                halign = 'left',
                left = 86 * 3,
            })
        )
end

-- currency
local function money_panel(props)
    return vdom.create_element('panel', {
            texture = 'ui_fruit_panel',
            width = 128 * 3,
            height = 32 * 3,
            valign = 'bottom',
            halign = 'left',
            left = -52 * 3,
        },
            vdom.create_element('label', {
                height = 30 * 3,
                color = props.currency > 0 and '#4e4042' or '#ee4042',
                text = '$'..props.currency,
                valign = 'top',
                bottom = -34 * 3,
                halign = 'left',
                left = 52 * 3,
            })
        )
end

-- health
-- max_health
-- currency
-- fruits
return function(props)
    return vdom.create_element('widget', { width = '100%', height = '100%' },
        vdom.create_element(hearts, props),
        vdom.create_element(money_panel, props),
        vdom.create_element(fruit_panel, props)
    )
end

