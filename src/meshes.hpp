#pragma once

#include <sushi/sushi.hpp>

auto get_sprite_mesh() -> sushi::mesh_group;

auto get_tilemap_mesh(int width) -> sushi::mesh_group;

auto get_screen_mesh(float width, float height) -> sushi::mesh_group;
