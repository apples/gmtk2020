#include "default.hpp"

#include "../components.hpp"

void default_animation::update(ember::database& entities, const ember::database::ent_id& eid) {}

auto default_animation::get_frame(ember::database& entities, const ember::database::ent_id& eid) -> animation_info {
    auto& sprite = entities.get_component<component::sprite>(eid);
    return {sprite.name};
}
