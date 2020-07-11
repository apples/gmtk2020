#include "player.hpp"

#include "../components.hpp"

enum states {
    IDLE,
    WALK,
};

void player_animation::update(ember::database& entities, const ember::database::ent_id& eid) {
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
            if (controller.left || controller.right) {
                set_state(WALK);
            }
            break;
        }
        case WALK: {
            if (!controller.left && !controller.right) {
                set_state(IDLE);
            }
            if (controller.left) {
                sprite.flip = false;
            }
            if (controller.right) {
                sprite.flip = true;
            }
            break;
        }
    }
}

auto player_animation::get_frame(ember::database& entities, const ember::database::ent_id& eid) -> animation_info {
    auto& sprite = entities.get_component<component::sprite>(eid);

    // Calculate frame
    switch (sprite.state) {
        case IDLE: {
            return {"player"};
        }
        case WALK: {
            return {
                "player_walk",
                {0.5, 0.5},
                {0, 0},
                {1, 1},
                int(sprite.time * 12) % 4,
            };
        }
    }

    return {"player"};
}