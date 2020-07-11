#pragma once

#include "physics.hpp"
#include "animation_state_machine.hpp"

#include "ember/camera.hpp"
#include "ember/entities.hpp"
#include "ember/scene.hpp"

#include <sushi/sushi.hpp>
#include <sol.hpp>

#include <cmath>
#include <memory>
#include <tuple>
#include <vector>
#include <random>

class scene_gameplay final : public ember::scene {
public:
    scene_gameplay(ember::engine& eng, scene* prev);

    virtual void init() override;
    virtual void tick(float delta) override;
    virtual void render() override;
    virtual auto handle_game_input(const SDL_Event& event) -> bool override;
    virtual auto render_gui() -> sol::table override;

private:
    ember::camera::orthographic camera;
    ember::database entities;
    int currency;
    physics_system physics;
    sol::table gui_state;
    sushi::mesh_group sprite_mesh;
    sushi::mesh_group tilemap_mesh;
    sushi::mesh_group screen_mesh;
    int world_width;
    std::mt19937 rng;
    std::unordered_map<std::string, std::shared_ptr<animation_state_machine>> animations;
};
