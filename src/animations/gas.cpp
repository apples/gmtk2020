#include "gas.hpp"

#include "../components.hpp"

#include <glm/gtx/rotate_vector.hpp>

enum states {
    GROW,
    CLOUD,
    DIE,
};

void gas_animation::update(ember::database& entities, const ember::database::ent_id& eid) {
    auto& body = entities.get_component<component::body>(eid);
    auto& sprite = entities.get_component<component::sprite>(eid);
    auto& controller = entities.get_component<component::controller>(eid);

    auto set_state = [&](int s) {
        sprite.state = s;
        sprite.time = 0;
    };

    // Process state
    switch (sprite.state) {
        case GROW: {
            if (sprite.time > 1/3.f) {
                set_state(CLOUD);
            }
            break;
        }
        case CLOUD: {
            if (sprite.time > 3.f) {
                set_state(DIE);
            }
            break;
        }
        case DIE: {
            break;
        }
    }
}

auto gas_animation::get_frame(ember::database& entities, const ember::database::ent_id& eid) -> animation_info {
    auto& sprite = entities.get_component<component::sprite>(eid);
    auto& body = entities.get_component<component::body>(eid);

    // Calculate frame
    switch (sprite.state) {
        case GROW: {
            return {
                "gas",
                {0.25, 0.5},
                {0, 0},
                {1, 1},
                int(sprite.time * 12) % 4,
            };
        }
        case CLOUD:
        case DIE: {
            return {
                "gas",
                {0.25, 0.5},
                {0, 0},
                {1, 1},
                int(sprite.time * 12) % 4 + 4,
            };
        }
    }

    return {"projectile"};
}
