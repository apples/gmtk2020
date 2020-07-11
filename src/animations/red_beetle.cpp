#include "red_beetle.hpp"

#include "../components.hpp"

enum states {
    FLYING,
};

void red_beetle_animation::update(ember::database& entities, const ember::database::ent_id& eid) {
    auto& body = entities.get_component<component::body>(eid);
    auto& sprite = entities.get_component<component::sprite>(eid);

    auto set_state = [&](int s) {
        sprite.state = s;
        sprite.time = 0;
    };

    // Process state
    switch (sprite.state) {
        case FLYING: {
            if (body.vel.x < 0) {
                sprite.flip = true;
            } else {
                sprite.flip = false;
            }
            break;
        }
    }
}

auto red_beetle_animation::get_frame(ember::database& entities, const ember::database::ent_id& eid) -> animation_info {
    auto& sprite = entities.get_component<component::sprite>(eid);

    // Calculate frame
    switch (sprite.state) {
        case FLYING: {
            return {
                "red_beetle",
                {0.5, 0.5},
                {0, 0},
                {0.5, 0.5},
                int(sprite.time * 24) % 4,
            };
        }
    }

    return {"red_beetle"};
}
