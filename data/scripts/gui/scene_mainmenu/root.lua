local vdom = require('vdom')

-- start_game: () => ()
return function(props)
    return vdom.create_element('panel', { width = '100%', height = '100%', texture = 'background/0' },
    vdom.create_element('panel', { width = '100%', height = '100%', texture = 'background/1' },
vdom.create_element('panel', { width = '100%', height = '100%', texture = 'background/2' },
vdom.create_element('panel', { width = '50%', height = '50%', valign='center', halign='center', color = '#000c' },
        vdom.create_element(
            'label',
            {
                halign = 'center',
                bottom = 720/2 + 30 * 3,
                color = '#fff',
                text = 'Overplanted!',
                height = 24 * 3,
            }
        ),
        vdom.create_element(
            'label',
            {
                halign = 'center',
                bottom = 720/2 - 60 * 3,
                color = '#fff',
                text = '[START GAME]',
                height = 24 * 2,
                on_click = props.start_game,
            }
        )
    ))))
end
