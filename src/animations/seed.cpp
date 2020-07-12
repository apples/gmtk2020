#include "seed.hpp"

#include "../components.hpp"

#include <glm/gtx/rotate_vector.hpp>

enum states {
    COPTER,
};

void seed_animation::update(ember::database& entities, const ember::database::ent_id& eid) {
    auto& sprite = entities.get_component<component::sprite>(eid);

    auto set_state = [&](int s) {
        sprite.state = s;
        sprite.time = 0;
    };

    // Process state
    switch (sprite.state) {
        case COPTER: {
            break;
        }
    }
}

auto seed_animation::get_frame(ember::database& entities, const ember::database::ent_id& eid) -> animation_info {
    auto& sprite = entities.get_component<component::sprite>(eid);
    auto& body = entities.get_component<component::body>(eid);

    // Calculate frame
    switch (sprite.state) {
        case COPTER: {
            return {
                "seed",
                {0.25, 1},
                {0, 0},
                {0.25, 0.25},
                int(sprite.time * 12) % 4,
                glm::length(body.vel) > 0.0001f ? glm::normalize(body.vel) : glm::vec2{0, -1},
            };
        }
    }

    return {"seed"};
}
