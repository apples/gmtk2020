#include "defensivePlant.hpp"

#include "../components.hpp"

enum states {
    IDLE,
};

void defensivePlant_animation::update(ember::database& entities, const ember::database::ent_id& eid) {
    auto& body = entities.get_component<component::body>(eid);
    auto& sprite = entities.get_component<component::sprite>(eid);
    auto& controller = entities.get_component<component::controller>(eid);

    auto set_state = [&](int s) {
        sprite.state = s;
        sprite.time = 0;
    };

    // Process state
    switch (sprite.state) {
        case IDLE: {
            break;
        }
    }
}

auto defensivePlant_animation::get_frame(ember::database& entities, const ember::database::ent_id& eid) -> animation_info {
    auto& sprite = entities.get_component<component::sprite>(eid);

    // Calculate frame
    switch (sprite.state) {
        case IDLE: {
            return {
                "DefensePlant1",
                {1, 1},
                {0, -0.5},
                {0.5, 0.25},
                0,
            };
        }
    }

    return {"DefensePlant1"};
}
