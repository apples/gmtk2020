#include "defensivePlant.hpp"

#include "../components.hpp"

enum states {
    GROWTH1,
    GROWTH2,
    GROWTH3,
    GROWTH4,
};

void defensivePlant_animation::update(ember::database& entities, const ember::database::ent_id& eid) {
    auto& body = entities.get_component<component::body>(eid);
    auto& sprite = entities.get_component<component::sprite>(eid);
    auto& controller = entities.get_component<component::controller>(eid);

    auto set_state = [&](int s) {
        sprite.state = s;
        sprite.time = 0;
    };
}

auto defensivePlant_animation::get_frame(ember::database& entities, const ember::database::ent_id& eid) -> animation_info {
    auto& sprite = entities.get_component<component::sprite>(eid);

    // Calculate frame
    switch (sprite.state) {
        case GROWTH1: {
            return {
                "DefensePlant1",
                {1, 1},
                {0, -0.5},
                {0.5, 0.25},
                0,
            };
        }
        case GROWTH2: {
            return {
                "DefensePlant2",
                {1, 1},
                {0, -0.5},
                {0.5, 1.0},
                0,
            };
        }
        case GROWTH3: {
            return {
                "DefensePlant3",
                {1, 1},
                {0, -0.5},
                {0.5, 1.0},
                0,
            };
        }
        case GROWTH4: {
            return {
                "DefensePlant4",
                {1, 1},
                {0, -0.5},
                {0.5, 2.0},
                0,
            };
        }
    }

    return {"DefensePlant1"};
}
