#pragma once

#include "../animation_state_machine.hpp"

class red_beetle_animation : public animation_state_machine {
public:
    virtual void update(ember::database& entities, const ember::database::ent_id& eid) override;
    virtual auto get_frame(ember::database& entities, const ember::database::ent_id& eid) -> animation_info override;
};
