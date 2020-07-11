#include "projectile.hpp"

#include "../components.hpp"

#include <glm/gtx/rotate_vector.hpp>

enum states {
    INFLIGHT,
};

void projectile_animation::update(ember::database& entities, const ember::database::ent_id& eid) {
    auto& body = entities.get_component<component::body>(eid);
    auto& sprite = entities.get_component<component::sprite>(eid);
    auto& controller = entities.get_component<component::controller>(eid);

    auto set_state = [&](int s) {
        sprite.state = s;
        sprite.time = 0;
    };

    // Process state
    switch (sprite.state) {
        case INFLIGHT: {
            break;
        }
    }
}

auto projectile_animation::get_frame(ember::database& entities, const ember::database::ent_id& eid) -> animation_info {
    auto& sprite = entities.get_component<component::sprite>(eid);
    auto& body = entities.get_component<component::body>(eid);

    // Calculate frame
    switch (sprite.state) {
        case INFLIGHT: {
            return {
                "projectile",
                {1, 1},
                {0, 0},
                {0.25, 0.25},
                0,
                glm::rotate(
                    glm::length(body.vel) > 0.0001f ? glm::normalize(body.vel) : glm::vec2{0, -1}, glm::radians(45.f)),
            };
        }
    }

    return {"projectile"};
}
