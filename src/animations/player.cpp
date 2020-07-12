#include "player.hpp"

#include "../components.hpp"

enum states {
    IDLE = 0,
    WALK = 1,
    SPRAY = 2,
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
        case SPRAY: {
            if (sprite.time >= 0.5) {
                set_state(IDLE);
            } else if (sprite.time >= 4/24.f && std::abs(body.vel.x) > 0.001f) {
                set_state(WALK);
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
        case SPRAY: {
            return {
                "player_spray",
                {0.25, 0.5},
                {0, 0},
                {1, 1},
                std::min(int(sprite.time * 24), 4),
            };
        }
    }

    return {"player"};
}
