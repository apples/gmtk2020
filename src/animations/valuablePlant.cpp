#include "valuablePlant.hpp"

#include "../components.hpp"

enum states {
    GROWTH0,
    GROWTH1,
};

void valuablePlant_animation::update(ember::database& entities, const ember::database::ent_id& eid) {
    auto& body = entities.get_component<component::body>(eid);
    auto& sprite = entities.get_component<component::sprite>(eid);
    auto& controller = entities.get_component<component::controller>(eid);

    auto set_state = [&](int s) {
        sprite.state = s;
        sprite.time = 0;
    };

    // Process state
    switch (sprite.state) {
        //case GROWTH1: {
            break;
        //}
    }
}

auto valuablePlant_animation::get_frame(ember::database& entities, const ember::database::ent_id& eid) -> animation_info {
    auto& sprite = entities.get_component<component::sprite>(eid);

    // Calculate frame
    switch (sprite.state) {
        case GROWTH0: {
            return {
                "DefensePlant1",
                {1, 1},
                {0, -0.5},
                {0.5, 0.25},
                0,
            };
        }
        case GROWTH1: {
            return {
                "DefensePlant2",
                {1, 1},
                {0, -0.5},
                {0.5, 1.0},
                0,
            };
        }
    }

    return {"DefensePlant1"};
}
