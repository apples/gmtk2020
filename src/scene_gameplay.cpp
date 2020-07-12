#include "scene_gameplay.hpp"

#include "scene_mainmenu.hpp"
#include "components.hpp"
#include "meshes.hpp"

#include "animations/default.hpp"
#include "animations/player.hpp"
#include "animations/red_beetle.hpp"
#include "animations/defensivePlant.hpp"
#include "animations/valuablePlant.hpp"
#include "animations/projectile.hpp"
#include "animations/gas.hpp"
#include "animations/balloon_box.hpp"
#include "animations/death.hpp"
#include "animations/seed.hpp"

#include "ember/camera.hpp"
#include "ember/engine.hpp"
#include "ember/vdom.hpp"
#include "ember/perf.hpp"

#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <sushi/sushi.hpp>

// Scene constructor
// Initializes private members and loads any prerequisite assets (usually small ones!).
// Scene is not actually the current scene yet, so interactions with the engine state and rendering are forbidden.
scene_gameplay::scene_gameplay(ember::engine& engine, ember::scene* prev)
    : scene(engine),
      camera(), // Camera has a sane default constructor, it is tweaked below
      entities(), // Entity database has no constructor parameters
      currency(),
      fruits(),
      physics(), // Physics system
      gui_state{engine.lua.create_table()}, // Gui state is initialized to an empty Lua table
      sprite_mesh{get_sprite_mesh()}, // Sprite and tilemap meshes is created statically
      tilemap_mesh{},
      screen_mesh{},
      world_width(80),
      rng(std::random_device{}()),
      animations{},
      delivered{0} {
    tilemap_mesh = get_tilemap_mesh(world_width);
    screen_mesh = get_screen_mesh(11.25 * 16 / 9.f, 11.25);
    camera.aspect_ratio = 16/9.f;
    camera.height = 11.25; // Height of the camera viewport in world units, in this case 32 tiles
    camera.near = -1; // Near plane of an orthographic view is away from camera, so this is actually +1 view range on Z

    // Prepare animations
    animations["default"] = std::make_shared<default_animation>();
    animations["player"] = std::make_shared<player_animation>();
    animations["red_beetle"] = std::make_shared<red_beetle_animation>();
    animations["defensivePlant"] = std::make_shared<defensivePlant_animation>();
    animations["valuablePlant"] = std::make_shared<valuablePlant_animation>();
    animations["projectile"] = std::make_shared<projectile_animation>();
    animations["gas"] = std::make_shared<gas_animation>();
    animations["balloon_box"] = std::make_shared<balloon_box_animation>();
    animations["death"] = std::make_shared<death_animation>();
    animations["seed"] = std::make_shared<seed_animation>();
}

// Scene initialization
// Distinct from construction, all private members should have valid values now.
// Used to initialize Lua state, initialize world, populate entities, etc.
// Basically "load the stage".
void scene_gameplay::init() {
    // We want scripts to have access to the entities as a global variable, so it is set here.
    engine->lua["entities"] = std::ref(entities);

    engine->lua["world_width"] = world_width;

    engine->lua["set_currency"] = [this](int c) { currency = c; };
    engine->lua["get_currency"] = [this]() { return currency; };

    engine->lua["set_fruits"] = [this](int c) { fruits = c; };
    engine->lua["get_fruits"] = [this]() { return fruits; };

    engine->lua["add_delivered"] = [this](int i) { delivered += i; };

    engine->lua["plant_at_position"] = [this](float x, float y) {
        auto snappedPos = glm::vec2(std::floor(x * 2) / 2, std::floor((y - .5) * 2) / 2);
        std::optional<ember::database::ent_id> result = std::nullopt;
        entities.visit([&](ember::database::ent_id eid, component::plant_tag, component::transform& trans) {
            if(glm::length(glm::vec2(trans.pos) - snappedPos) < 0.1f) {
                result = eid;
            }
        });
        return result;
    };

    engine->lua["pick_fruit"] = [this](float x, float y) {
        auto snappedPos = glm::vec2(std::floor(x * 2) / 2, std::floor((y - .5) * 2) / 2);
        std::optional<ember::database::ent_id> Eid = std::nullopt;

        entities.visit([&](ember::database::ent_id eid, component::valuable_tag, component::transform& trans, component::growth& grow) {
            if(trans.pos.x == snappedPos.x && trans.pos.y == snappedPos.y && grow.stage > 0) {
                Eid = eid;
                grow.stage = 0;
                grow.growthTimer = grow.growTime;
                if (auto script = entities.get_component<component::script*>(eid)) {
                    engine->call_script("actors." + script->name, "grow", eid, grow.stage);
                }

                fruits++;
            }
        });
        return Eid;
    };

    engine->lua["to_title"] = [this]() {
        engine->queue_transition<scene_mainmenu>();
    };

    engine->lua["math"]["randomseed"](rng());

    // Call the "init" function in the "data/scripts/scenes/gameplay.lua" script, with no params.
    engine->call_script("scenes.gameplay", "init");

    currency = 200;
}

// Tick/update function
// Performs all necessary game processing based on the delta time.
// Updates gui_state as necessary.
// Basically does everything except rendering.
void scene_gameplay::tick(float delta) {
    // Timer system
    ember::perf::start_section("scene_gameplay.timer");
    entities.visit([&](ember::database::ent_id eid, component::timer& timer, const component::script& script) {
        bool edge = timer.time > 0;
        timer.time -= delta;
        if (timer.time <= 0 && edge) {
            engine->call_script("actors." + script.name, "timer", eid, timer);
        }
    });
    ember::perf::end_section();

    // Targeting system
    ember::perf::start_section("scene_gameplay.targeting");
    entities.visit([&](component::targeting& targeting) {
        if (!targeting.target || !entities.exists(*targeting.target)) {
            auto num_plants = entities.count_components<component::plant_tag>() +
                              entities.count_components<component::player>() +
                              entities.count_components<component::balloon>();
            if (num_plants > 0) {
                auto roll = std::uniform_int_distribution{0, num_plants - 1};
                auto result = roll(rng);
                targeting.target = std::nullopt;
                entities.visit([&](ember::database::ent_id eid, component::plant_tag) {
                    if (result == 0) {
                        targeting.target = eid;
                    }
                    --result;
                });
                entities.visit([&](ember::database::ent_id eid, component::balloon) {
                    if (result == 0) {
                        targeting.target = eid;
                    }
                    --result;
                });
                entities.visit([&](ember::database::ent_id eid, component::player) {
                    if (result == 0) {
                        targeting.target = eid;
                    }
                    --result;
                });
            }
        }
    });
    ember::perf::end_section();

    // Shooter system
    ember::perf::start_section("scene_gameplay.shooter");
    entities.visit([&](ember::database::ent_id eid, component::shooter& shooter) {
        shooter.cooldown_timer -= delta;
        if (shooter.cooldown_timer <= 0) {
            if (auto script = entities.get_component<component::script*>(eid)) {
                engine->call_script("actors." + script->name, "shoot", eid);
            }
            shooter.cooldown_timer = shooter.cooldown;
        }
    });
    ember::perf::end_section();

    // Seeder system
    ember::perf::start_section("scene_gameplay.seeder");
    entities.visit([&](ember::database::ent_id eid, component::seeder& seeder) {
        seeder.cooldown_timer -= delta;
        if (seeder.cooldown_timer <= 0) {
            if (auto script = entities.get_component<component::script*>(eid)) {
                engine->call_script("actors." + script->name, "seed", eid);
            }
            seeder.cooldown_timer = seeder.cooldown;
        }
    });
    ember::perf::end_section();

    // Scripting system
    ember::perf::start_section("scene_gameplay.scripting");
    engine->call_script("systems.scripting", "visit", delta);
    ember::perf::end_section();

    // Balloon tracker system
    ember::perf::start_section("scene_gameplay.balloon_tracker");
    entities.visit([&](component::balloon_tracker& tracker, const component::transform& transform) {
        auto dist = 1.5f;
        tracker.tracked = std::nullopt;
        entities.visit([&](ember::database::ent_id eid,
                           const component::balloon& balloon,
                           const component::transform& balloon_transform) {
            auto d = glm::length(balloon_transform.pos - transform.pos);
            if (d < dist) {
                tracker.tracked = eid;
                dist = d;
            }
        });
    });
    ember::perf::end_section();

    // Physics system
    ember::perf::start_section("scene_gameplay.physics");
    physics.step(*engine, entities, delta);
    ember::perf::end_section();


    // Health system
    ember::perf::start_section("scene_gameplay.health");
    entities.visit([&](ember::database::ent_id eid, component::health& health) {
        if (health.current > health.max) {
            health.current = health.max;
        }
        health.invulnerable_time -= delta;
        health.invulnerable = health.invulnerable_time > 0;
        if (health.current <= 0) {
            if (health.anim) {
                auto iter = end(animations);
                bool flip = false;
                if (auto sprite = entities.get_component<component::sprite*>(eid)) {
                    iter = animations.find(sprite->name);
                    flip = sprite->flip;
                }
                if (iter == end(animations)) {
                    iter = animations.find("default");
                }

                auto info = iter->second->get_frame(entities, eid);

                if (flip) {
                    info.scale.x *= -1;
                }


                auto anim = entities.create_entity();
                entities.add_component(anim, component::death_anim{
                    info,
                    {(rng()&1) ? -10 : 10, 10},
                    2,
                });
                entities.add_component(anim, component::sprite{"death"});
                if (auto transform = entities.get_component<component::transform*>(eid)) {
                    entities.add_component(anim, *transform);
                }
            }
            entities.destroy_entity(eid);
            engine->lua["play_sfx"]("pew", 1.f);
        }
    });
    ember::perf::end_section();

    // Death anim system
    ember::perf::start_section("scene_gameplay.death_anim");
    entities.visit(
        [&](ember::database::ent_id eid, component::death_anim& death_anim, component::transform& transform) {
            death_anim.time -= delta;
            if (death_anim.time <= 0) {
                entities.destroy_entity(eid);
                return;
            }
            death_anim.vel.y -= 20 * delta;
            transform.pos += glm::vec3(death_anim.vel * delta, 0);
            death_anim.base_info.heading = glm::rotate(death_anim.base_info.heading, glm::radians(720.f) * delta);
        });
    ember::perf::end_section();

    // Growth system
    entities.visit([&](ember::database::ent_id eid, component::growth& growth) {
        growth.growthTimer -= delta;
        if (growth.growthTimer <= 0) {
            growth.stage++;
            if (auto script = entities.get_component<component::script*>(eid)) {
                engine->call_script("actors." + script->name, "grow", eid, growth.stage);
            }
            growth.growthTimer = growth.growTime;
        }
    });

    // Camera system
    ember::perf::start_section("scene_gameplay.camera");
    entities.visit([&](const component::player& player, const component::transform& transform) {
        auto range = world_width / 2.f - camera.height * camera.aspect_ratio / 2.f;
        camera.pos = glm::floor(
                         glm::vec3{glm::clamp(
                                       glm::vec2{transform.pos} + player.focus,
                                       glm::vec2{-range, camera.height / 2},
                                       glm::vec2{range, 999.f}),
                                   0.f} *
                         32.f) /
                     32.f;
    });
    ember::perf::end_section();

    // Sprite system
    ember::perf::start_section("scene_gameplay.sprite");
    entities.visit([&](ember::database::ent_id eid, component::sprite& sprite) {
        sprite.time += delta;
        auto iter = animations.find(sprite.name);
        if (iter != end(animations)) {
            iter->second->update(entities, eid);
        }
    });
    ember::perf::end_section();

    // Reset controllers
    ember::perf::start_section("scene_gameplay.reset_controllers");
    entities.visit([&](component::controller& con) {
        con.jump_pressed = false;
        con.action_pressed = false;
        con.pump_pressed = false;
        con.snip_pressed = false;
        con.sow_defensive = false;
        con.sow_valuable = false;
    });
    ember::perf::end_section();

    // Player UI meters
    ember::perf::start_section("scene_gameplay.player_ui");
    gui_state["health"] = 0;
    entities.visit([&](const component::player& player, const component::health& health) {
        gui_state["health"] = health.current;
        gui_state["max_health"] = health.max;
    });
    ember::perf::end_section();

    gui_state["currency"] = currency;
    gui_state["fruits"] = fruits;
    gui_state["delivered"] = delivered;
}

// Render function
// Notice the lack of delta time.
// Performs all world rendering, but not GUI.
// Responsible for setting up the OpenGL state, binding shaders, etc.
// Must be *fast*!
// The EZ3D helper can be used for efficient 3D rendering, currently there is no equivalent for 2D.
void scene_gameplay::render() {
    // Set up some OpenGL state. This is mostly copy-pasted and should be self-explanatory.
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glEnable(GL_SAMPLE_COVERAGE);
    //glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);

    // Bind the basic shader and set some defaults.
    engine->basic_shader.bind();
    engine->basic_shader.set_cam_forward(get_forward(camera));
    engine->basic_shader.set_tint({1, 1, 1, 1});
    engine->basic_shader.set_hue(0);
    engine->basic_shader.set_saturation(1);

    // Get projection and view matrices from camera.
    auto proj = get_proj(camera);
    auto view = get_view(camera);

    // Render background
    {
        ember::perf::start_section("scene_gameplay.render_background");
        auto modelmat = glm::mat4{1.f};

        for (int i = 0; i < 3; ++i) {
            auto w = camera.height * camera.aspect_ratio;
            auto h = camera.height;
            auto pan_strength = i * 0.25f;
            auto pan_pos = -glm::mod(
                glm::vec2{camera.pos} * pan_strength, glm::vec2{w, h});

            for (const auto& offset : {
                     glm::vec2{0, 0},
                     glm::vec2{w, 0},
                     glm::vec2{0, h},
                     glm::vec2{w, h},
                 }) {
                auto bgview = glm::translate(glm::mat4{1.f}, glm::vec3{pan_pos + offset, 0});

                engine->basic_shader.set_uvmat(glm::mat3{1.f});
                engine->basic_shader.set_normal_mat(glm::inverseTranspose(bgview * modelmat));
                engine->basic_shader.set_MVP(proj * bgview * modelmat);

                sushi::set_texture(0, *engine->texture_cache.get("background/" + std::to_string(i)));
                sushi::draw_mesh(screen_mesh);
            }
        }
        ember::perf::end_section();
    }

    // Render tilemap
    {
        ember::perf::start_section("scene_gameplay.render_tilemap");
        auto modelmat = glm::mat4(1.f);

        engine->basic_shader.set_uvmat(glm::mat3{1.f});
        engine->basic_shader.set_normal_mat(glm::inverseTranspose(view * modelmat));
        engine->basic_shader.set_MVP(proj * view * modelmat);

        sushi::set_texture(0, *engine->texture_cache.get("tiles"));
        sushi::draw_mesh(tilemap_mesh);
        ember::perf::end_section();
    }

    // Render entities
    ember::perf::start_section("scene_gameplay.render_entities");
    const auto cam_w = camera.height*camera.aspect_ratio;
    entities.visit(
        [&](ember::database::ent_id eid, const component::sprite& sprite, const component::transform& transform) {
            auto d = glm::abs(glm::vec3(glm::vec2(transform.pos), 0) - camera.pos);
            if (d.x > cam_w/2 + 2 && d.y > camera.height/2 + 2) {
                return;
            }

            ember::perf::start_section("scene_gameplay.render_entities.get_anim_info");
            auto iter = animations.find(sprite.name);
            if (iter == end(animations)) {
                iter = animations.find("default");
            }

            auto info = iter->second->get_frame(entities, eid);

            if (sprite.flip) {
                info.scale.x *= -1;
            }
            ember::perf::end_section();

            ember::perf::start_section("scene_gameplay.render_entities.calc_matrices");
            auto spritemat = glm::scale(glm::mat4{1}, glm::vec3{info.scale, 1});
            spritemat = glm::translate(spritemat, glm::vec3{-info.origin, 0});
            spritemat = glm::rotate(spritemat, glm::atan(info.heading.y, info.heading.x), glm::vec3{0, 0, 1});
            auto modelmat = to_mat4(transform) * spritemat;
            ember::perf::end_section();

            // Calculate UV matrix for rendering the correct sprite.
            ember::perf::start_section("scene_gameplay.render_entities.calc_uvs");
            auto cols = int(1 / info.uv_size.x);
            auto uvoffset = glm::vec2{info.frame % cols, info.frame / cols} * info.uv_size;
            auto uvmat = glm::mat3{1.f};
            uvmat = glm::translate(uvmat, uvoffset);
            uvmat = glm::scale(uvmat, info.uv_size);
            ember::perf::end_section();

            // Set matrix uniforms.
            ember::perf::start_section("scene_gameplay.render_entities.set_uniforms");
            engine->basic_shader.set_uvmat(uvmat);
            engine->basic_shader.set_normal_mat(glm::inverseTranspose(view * modelmat));
            engine->basic_shader.set_MVP(proj * view * modelmat);
            ember::perf::end_section();

            ember::perf::start_section("scene_gameplay.render_entities.draw");
            auto tex = engine->texture_cache.get(info.texture);

            if (tex) {
                sushi::set_texture(0, *tex);
            } else {
                std::cout << "Warning: Texture not found: " << info.texture << std::endl;
            }

            sushi::draw_mesh(sprite_mesh);
            ember::perf::end_section();
        });
    ember::perf::end_section();
}

// Handle input events, called asynchronously
auto scene_gameplay::handle_game_input(const SDL_Event& event) -> bool {
    using component::controller;

    // Updates a single key for all controllers
    auto update = [&](bool press, bool controller::*key, bool controller::*pressed) {
        entities.visit([&](controller& con) {
            if (key) con.*key = press;
            if (pressed) con.*pressed = press;
        });
    };

    // Processes a key event
    auto process_key = [&](const SDL_KeyboardEvent& key){
        auto pressed = key.state == SDL_PRESSED;
        switch (key.keysym.sym) {
            case SDLK_LEFT:
            case SDLK_a:
                update(pressed, &controller::left, nullptr);
                return true;
            case SDLK_RIGHT:
            case SDLK_d:
                update(pressed, &controller::right, nullptr);
                return true;
            case SDLK_DOWN:
            case SDLK_s:
                update(pressed, &controller::down, nullptr);
                return true;
            case SDLK_SPACE:
            case SDLK_UP:
            case SDLK_w:
            case SDLK_z:
                update(pressed, nullptr, &controller::jump_pressed);
                return true;
            case SDLK_x:
                update(pressed, nullptr, &controller::action_pressed);
                return true;
            case SDLK_c:
                update(pressed, nullptr, &controller::pump_pressed);
                return true;
            case SDLK_f:
                update(pressed, nullptr, &controller::snip_pressed);
                return true;
            case SDLK_q:
                update(pressed, nullptr, &controller::sow_defensive);
                return true;
            case SDLK_e:
                update(pressed, nullptr, &controller::sow_valuable);
                return true;
            case SDLK_LSHIFT:
                update(pressed, &controller::collect, nullptr);
                return true;
            default:
                return false;
        }
    };

    switch (event.type) {
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            if (event.key.repeat == 0) {
                return process_key(event.key);
            }
            break;
    }
    return false;
}

// Render GUI, which means returning the result of `create_element`.
auto scene_gameplay::render_gui() -> sol::table {
    return ember::vdom::create_element(engine->lua, "gui.scene_gameplay.root", gui_state, engine->lua.create_table());
}
