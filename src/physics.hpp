#pragma once

#include "components.hpp"

#include "ember/engine.hpp"
#include "ember/entities.hpp"

#include <unordered_map>
#include <vector>

class physics_system {
public:
    void step(ember::engine& engine, ember::database& entities, float delta);
private:
    struct manifold {
        float left, right, bottom, top;
    };

    struct world_object : manifold {
        ember::database::ent_id eid;
        component::body* body;
        component::transform* transform;
    };

    float time = 0;
    std::unordered_map<int, std::vector<world_object>> buckets;
};
