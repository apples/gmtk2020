#include "death.hpp"

#include "../components.hpp"

void death_animation::update(ember::database& entities, const ember::database::ent_id& eid) {}

auto death_animation::get_frame(ember::database& entities, const ember::database::ent_id& eid) -> animation_info {
    auto& death_anim = entities.get_component<component::death_anim>(eid);
    return death_anim.base_info;
}
