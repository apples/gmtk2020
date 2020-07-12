#include "physics.hpp"

#include "components.hpp"

#include "ember/perf.hpp"

void physics_system::step(ember::engine& engine, ember::database& entities, float delta) {
    time += delta;

    delta = 1.f/120.f;

    auto collides_with = [&](const component::body& a, const component::body& b) {
        return a.collides_with[b.layer - 1];
    };

    auto events_with = [&](const component::body& a, const component::body& b) {
        return a.events_with[b.layer - 1];
    };

    auto n_steps = int(time / delta);

    for (int i = 0; i < n_steps; ++i) {
        ember::perf::start_section("physics.single_step");
        using type_t = component::body::type_t;

        struct manifold {
            float left, right, bottom, top;
        };

        struct world_object : manifold {
            ember::database::ent_id eid;
            component::body* body;
            component::transform* transform;
        };

        ember::perf::start_section("physics.alloc_world");
        std::vector<world_object> objects;
        objects.reserve(entities.count_components<component::body>());
        ember::perf::end_section();

        ember::perf::start_section("physics.gather");
        entities.visit([&](ember::database::ent_id eid, component::body& body, component::transform& transform){
            body.vel *= glm::pow(glm::vec2{1, 1} - body.vel_damp, glm::vec2{delta, delta});
            body.vel += body.accel * delta;
            transform.pos += glm::vec3{body.vel * delta, 0};
            objects.push_back({
                {
                    transform.pos.x + body.left,
                    transform.pos.x + body.right,
                    transform.pos.y + body.bottom,
                    transform.pos.y + body.top,
                },
                eid,
                &body,
                &transform,
            });
        });
        ember::perf::end_section();

        ember::perf::start_section("physics.sort");
        std::sort(begin(objects), end(objects), [](auto& a, auto& b) {
            return a.left < b.left;
        });
        ember::perf::end_section();

        ember::perf::start_section("physics.alloc_current");
        std::vector<world_object*> current;
        current.reserve(objects.size());
        ember::perf::end_section();

        ember::perf::start_section("physics.sweep");
        for (auto& obj : objects) {
            ember::perf::start_section("physics.prune");
            current.erase(std::remove_if(begin(current), end(current), [&](auto* a){
                return a->right < obj.left || !entities.exists(a->eid);
            }), end(current));
            ember::perf::end_section();

            ember::perf::start_section("physics.inner_loop");
            for (auto& obj2 : current) {
                auto no_collide = !collides_with(*obj.body, *obj2->body) && !collides_with(*obj2->body, *obj.body);
                auto no_event = !events_with(*obj.body, *obj2->body) && !events_with(*obj2->body, *obj.body);

                if (no_collide && no_event) {
                    continue;
                }

                ember::perf::start_section("physics.calc_manifold");
                auto overlap = manifold{
                    std::max(obj.left, obj2->left),
                    std::min(obj.right, obj2->right),
                    std::max(obj.bottom, obj2->bottom),
                    std::min(obj.top, obj2->top),
                };

                auto w = overlap.right - overlap.left;
                auto h = overlap.top - overlap.bottom;
                ember::perf::end_section();

                if (w > 0 && h > 0) {
                    auto* ap = &obj;
                    auto* bp = obj2;
                    if (bp->body->type < ap->body->type) {
                        std::swap(ap, bp);
                    }
                    auto& a = *ap;
                    auto& b = *bp;

                    // Pre-collide
                    ember::perf::start_section("physics.pre_collide");
                    if (events_with(*b.body, *a.body)) {
                        if (auto b_script = entities.get_component<component::script*>(b.eid)) {
                            engine.call_script(
                                "systems.physics", "pre_collide", "actors." + b_script->name, b.eid, a.eid);
                            if (!entities.exists(obj.eid)) {
                                break;
                            }
                            if (!entities.exists(obj2->eid)) {
                                continue;
                            }
                        }
                    }
                    if (events_with(*a.body, *b.body)) {
                        if (auto a_script = entities.get_component<component::script*>(a.eid)) {
                            engine.call_script(
                                "systems.physics", "pre_collide", "actors." + a_script->name, a.eid, b.eid);
                            if (!entities.exists(obj.eid)) {
                                break;
                            }
                            if (!entities.exists(obj2->eid)) {
                                continue;
                            }
                        }
                    }
                    ember::perf::end_section();

                    // Collide
                    if (!no_collide) {
                        ember::perf::start_section("physics.collide");
                        if (a.body->type == type_t::DYNAMIC && b.body->type == type_t::DYNAMIC) {
                            if (w < h) {
                                auto resolve_x = w / 2;
                                if (a.transform->pos.x < b.transform->pos.x) {
                                    resolve_x = -resolve_x;
                                }
                                a.transform->pos.x += resolve_x;
                                b.transform->pos.x -= resolve_x;
                                std::swap(a.body->vel.x, b.body->vel.x);
                                a.left += resolve_x;
                                a.right += resolve_x;
                                b.left -= resolve_x;
                                b.right -= resolve_x;
                            } else {
                                auto resolve_y = h / 2;
                                if (a.transform->pos.y < b.transform->pos.y) {
                                    resolve_y = -resolve_y;
                                }
                                a.transform->pos.y += resolve_y;
                                b.transform->pos.y -= resolve_y;
                                std::swap(a.body->vel.y, b.body->vel.y);
                                a.bottom += resolve_y;
                                a.top += resolve_y;
                                b.bottom -= resolve_y;
                                b.top -= resolve_y;
                            }
                        } else if (b.body->type == type_t::DYNAMIC) {
                            if (w < h) {
                                if (!a.body->jump_through) {
                                    auto resolve_x = w;
                                    if (a.transform->pos.x < b.transform->pos.x) {
                                        resolve_x = -resolve_x;
                                    }
                                    b.transform->pos.x -= resolve_x;
                                    b.body->vel.x = 0;
                                    b.left -= resolve_x;
                                    b.right -= resolve_x;
                                }
                            } else {
                                auto above = a.transform->pos.y < b.transform->pos.y;
                                if (!a.body->jump_through || above && b.body->vel.y < 0) {
                                    auto resolve_y = h;
                                    if (above) {
                                        resolve_y = -resolve_y;
                                    }
                                    b.transform->pos.y -= resolve_y;
                                    b.body->vel.y = 0;
                                    b.bottom -= resolve_y;
                                    b.top -= resolve_y;
                                }
                            }
                        } else if (a.body->type == type_t::KINEMATIC && b.body->type == type_t::KINEMATIC) {
                            throw std::runtime_error("Not implemented!");
                        } else if (b.body->type == type_t::KINEMATIC) {
                            if (w < h) {
                                auto resolve_x = w;
                                if (a.transform->pos.x < b.transform->pos.x) {
                                    resolve_x = -resolve_x;
                                }
                                b.transform->pos.x -= resolve_x;
                                b.left -= resolve_x;
                                b.right -= resolve_x;
                            } else {
                                auto resolve_y = h;
                                if (a.transform->pos.y < b.transform->pos.y) {
                                    resolve_y = -resolve_y;
                                }
                                b.transform->pos.y -= resolve_y;
                                b.bottom -= resolve_y;
                                b.top -= resolve_y;
                            }
                        }
                        ember::perf::end_section();
                    }

                    // Post-collide
                    ember::perf::start_section("physics.post_collide");
                    if (events_with(*b.body, *a.body)) {
                        if (auto b_script = entities.get_component<component::script*>(b.eid)) {
                            engine.call_script(
                                "systems.physics", "post_collide", "actors." + b_script->name, b.eid, a.eid);
                            if (!entities.exists(obj.eid)) {
                                break;
                            }
                            if (!entities.exists(obj2->eid)) {
                                continue;
                            }
                        }
                    }
                    if (events_with(*a.body, *b.body)) {
                        if (auto a_script = entities.get_component<component::script*>(a.eid)) {
                            engine.call_script(
                                "systems.physics", "post_collide", "actors." + a_script->name, a.eid, b.eid);
                            if (!entities.exists(obj.eid)) {
                                break;
                            }
                        }
                    }
                    ember::perf::end_section();
                }
            }

            current.push_back(&obj);
        }
        ember::perf::end_section();

        ember::perf::end_section();
    }

    time -= delta * n_steps;
}
