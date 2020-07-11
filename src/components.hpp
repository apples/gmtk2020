#pragma once

#include "ember/component_common.hpp"
#include "ember/entities.hpp"
#include "ember/net_id.hpp"
#include "ember/ez3d.hpp"

#include <sushi/sushi.hpp>

#include <chrono>
#include <functional>
#include <optional>
#include <string>
#include <memory>

#define EMBER_REFLECTION_ENABLE_REGISTRY
#include "ember/reflection_start.hpp"

namespace component {

/** Actor script used for various events */
struct script {
    std::string name; /** Script name within the 'actors.' namespace */
};
REFLECT(script, (name))

/** World transform */
struct transform : sushi::transform {};
REFLECT(transform, (pos)(rot)(scl))

struct sprite {
    std::string texture;
    glm::vec2 size = {1, 1};
    std::vector<int> frames;
    float time = 0;
};
REFLECT(sprite, (texture)(size)(frames)(time))

struct body {
    enum type_t {
        STATIC,
        KINEMATIC,
        DYNAMIC
    };

    enum layer_t {
        BALL,
        PADDLE,
        BRICK,
        N_TYPES,
    };

    type_t type;
    layer_t layer;
    std::array<bool, N_TYPES> collides_with = {};
    glm::vec2 size = {0, 0};
    glm::vec2 vel = {0, 0};
    glm::vec2 accel = {0, 0};
};
REFLECT(body, (type)(layer)(collides_with)(size)(vel)(accel))

struct controller {
    bool left = false;
    bool right = false;
    bool action_pressed = false;
    bool sow_defensive = false;
    bool sow_valuable = false;
};
REFLECT(controller, (left)(right)(action_pressed)(sow_defensive)(sow_valuable))

struct player {
    glm::vec2 focus = {0, 0};
};
REFLECT(player, (focus))

} // namespace component

#include "ember/reflection_end.hpp"
