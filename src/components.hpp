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
    std::string name;
    int state = 0;
    bool flip = false;
    float time = 0;
};
REFLECT(sprite, (name)(state)(flip)(time))

struct body {
    enum type_t {
        STATIC = 0,
        KINEMATIC = 1, /** DO NOT USE */
        DYNAMIC = 2,
    };

    enum layer_t {
        PLAYER = 1,
        WALL = 2,
        PLANT = 3,
        PROJECTILE = 4,
        BUG = 5,
        GAS = 6,
        BOX = 7,
        N_TYPES = 7,
    };

    type_t type;
    layer_t layer;
    std::array<bool, N_TYPES> collides_with = {};
    std::array<bool, N_TYPES> events_with = {};
    float left = 0;
    float right = 0;
    float bottom = 0;
    float top = 0;
    glm::vec2 vel = {0, 0};
    glm::vec2 accel = {0, 0};
    glm::vec2 vel_damp = {0, 0};
    bool jump_through = false;
    bool pass_through = false;
    bool carry = false;
    bool on_ground = false;
};
REFLECT(body, (type)(layer)(collides_with)(events_with)(left)(right)(bottom)(top)(vel)(accel)(vel_damp)(jump_through)(pass_through)(carry)(on_ground))

struct controller {
    bool left = false;
    bool right = false;
    bool down = false;
    bool jump_pressed = false;
    bool action_pressed = false;
    bool pump_pressed = false;
    bool sow_defensive = false;
    bool sow_valuable = false;
    bool collect = false;
};
REFLECT(controller, (left)(right)(down)(jump_pressed)(action_pressed)(pump_pressed)(sow_defensive)(sow_valuable)(collect))

struct player {
    glm::vec2 focus = {0, 0};
};
REFLECT(player, (focus))

struct targeting {
    std::optional<ember::database::ent_id> target;
};
REFLECT(targeting, (target))

using plant_tag = ginseng::tag<struct plant_tag_T>;
REFLECT(plant_tag)

using valuable_tag = ginseng::tag<struct valuable_tag_T>;
REFLECT(valuable_tag)

struct shooter {
    float cooldown = 1;
    float cooldown_timer = 1;
};
REFLECT(shooter, (cooldown)(cooldown_timer))

struct health {
    int current = 1;
    int max = 1;
};
REFLECT(health, (current)(max))

struct growth {
    float growthTimer = 1;
    float growTime = 1;//how long it should take to mature again
    int stage = 0;
};
REFLECT(growth, (growthTimer)(growTime)(stage))

struct balloon {
    int air = 0;
};
REFLECT(balloon, (air))

struct balloon_tracker {
    std::optional<ember::database::ent_id> tracked;
};
REFLECT(balloon_tracker, (tracked))

} // namespace component

#include "ember/reflection_end.hpp"
