#pragma once

#include "ember/entities.hpp"

#include <glm/glm.hpp>

#include <string>

class animation_state_machine {
public:
    struct animation_info {
        std::string texture;
        glm::vec2 uv_size = {1, 1};
        glm::vec2 origin = {0, 0};
        glm::vec2 scale = {1, 1};
        int frame = 0;
    };

    virtual ~animation_state_machine() = 0;

    virtual void update(ember::database& entities, const ember::database::ent_id& eid) = 0;

    virtual auto get_frame(ember::database& entities, const ember::database::ent_id& eid) -> animation_info = 0;
};

inline animation_state_machine::~animation_state_machine() = default;
