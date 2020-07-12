#include "balloon_box.hpp"

#include "../components.hpp"

#include <glm/gtx/rotate_vector.hpp>

enum states {
    BALLOON,
};

void balloon_box_animation::update(ember::database& entities, const ember::database::ent_id& eid) {
    auto& sprite = entities.get_component<component::sprite>(eid);

    auto set_state = [&](int s) {
        sprite.state = s;
        sprite.time = 0;
    };

    // Process state
    switch (sprite.state) {
        case BALLOON: {
            break;
        }
    }
}

auto balloon_box_animation::get_frame(ember::database& entities, const ember::database::ent_id& eid) -> animation_info {
    auto& sprite = entities.get_component<component::sprite>(eid);
    auto& balloon = entities.get_component<component::balloon>(eid);

    // Calculate frame
    switch (sprite.state) {
        case BALLOON: {
            return {
                "balloon_box",
                {0.125, 0.5},
                {0, -0.25},
                {1, 2},
                balloon.air,
            };
        }
    }

    return {"balloon_box"};
}
