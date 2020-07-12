local vdom = require('vdom')

return function(props)
    return vdom.create_element('widget', { width = '100%', height = '100%' },
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

