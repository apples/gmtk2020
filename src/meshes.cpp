#include "meshes.hpp"

auto get_sprite_mesh() -> sushi::mesh_group {
    sushi::mesh_group_builder mb;
    mb.enable(sushi::attrib_location::POSITION);
    mb.enable(sushi::attrib_location::TEXCOORD);
    mb.enable(sushi::attrib_location::NORMAL);

    mb.mesh("sprite");

    auto left = -0.5f;
    auto right = 0.5f;
    auto bottom = -0.5f;
    auto top = 0.5f;

    auto bottomleft = mb.vertex().position({left, bottom, 0}).texcoord({0, 1}).normal({0, 1, 0}).get();
    auto topleft = mb.vertex().position({left, top, 0}).texcoord({0, 0}).normal({0, 1, 0}).get();
    auto bottomright = mb.vertex().position({right, bottom, 0}).texcoord({1, 1}).normal({0, 1, 0}).get();
    auto topright = mb.vertex().position({right, top, 0}).texcoord({1, 0}).normal({0, 1, 0}).get();

    mb.tri(bottomleft, topleft, topright);
    mb.tri(topright, bottomright, bottomleft);

    return mb.get();
}

auto get_tilemap_mesh(int width) -> sushi::mesh_group {
    sushi::mesh_group_builder mb;
    mb.enable(sushi::attrib_location::POSITION);
    mb.enable(sushi::attrib_location::TEXCOORD);
    mb.enable(sushi::attrib_location::NORMAL);

    mb.mesh("tilemap");

    for (int c = 0; c < width; ++c) {
        auto left = -width/2.f + c;
        auto right = left + 1;
        auto top = 1.f;
        auto bottom = top - 1;

        auto uvleft = 0.f;
        auto uvright = 0.125f;
        auto uvtop = 0.f;
        auto uvbottom = 1.f;

        auto bottomleft = mb.vertex().position({left, bottom, 0}).texcoord({uvleft, uvbottom}).normal({0, 1, 0}).get();
        auto topleft = mb.vertex().position({left, top, 0}).texcoord({uvleft, uvtop}).normal({0, 1, 0}).get();
        auto bottomright =
            mb.vertex().position({right, bottom, 0}).texcoord({uvright, uvbottom}).normal({0, 1, 0}).get();
        auto topright = mb.vertex().position({right, top, 0}).texcoord({uvright, uvtop}).normal({0, 1, 0}).get();

        mb.tri(bottomleft, topleft, topright);
        mb.tri(topright, bottomright, bottomleft);
    }

    for (int r = 1; r < 3; ++r) {
        auto left = -width/2.f;
        auto right = left + 1;
        auto top = r + 1.f;
        auto bottom = top - 1;

        auto uvleft = 0.125f;
        auto uvright = 0.25f;
        auto uvtop = 0.f;
        auto uvbottom = 1.f;

        auto bottomleft = mb.vertex().position({left, bottom, 0}).texcoord({uvleft, uvbottom}).normal({0, 1, 0}).get();
        auto topleft = mb.vertex().position({left, top, 0}).texcoord({uvleft, uvtop}).normal({0, 1, 0}).get();
        auto bottomright =
            mb.vertex().position({right, bottom, 0}).texcoord({uvright, uvbottom}).normal({0, 1, 0}).get();
        auto topright = mb.vertex().position({right, top, 0}).texcoord({uvright, uvtop}).normal({0, 1, 0}).get();

        mb.tri(bottomleft, topleft, topright);
        mb.tri(topright, bottomright, bottomleft);
    }

    for (int r = 1; r < 3; ++r) {
        auto left = width/2.f - 1;
        auto right = left + 1;
        auto top = r + 1.f;
        auto bottom = top - 1;

        auto uvleft = 0.25f;
        auto uvright = 0.125f;
        auto uvtop = 0.f;
        auto uvbottom = 1.f;

        auto bottomleft = mb.vertex().position({left, bottom, 0}).texcoord({uvleft, uvbottom}).normal({0, 1, 0}).get();
        auto topleft = mb.vertex().position({left, top, 0}).texcoord({uvleft, uvtop}).normal({0, 1, 0}).get();
        auto bottomright =
            mb.vertex().position({right, bottom, 0}).texcoord({uvright, uvbottom}).normal({0, 1, 0}).get();
        auto topright = mb.vertex().position({right, top, 0}).texcoord({uvright, uvtop}).normal({0, 1, 0}).get();

        mb.tri(bottomleft, topleft, topright);
        mb.tri(topright, bottomright, bottomleft);
    }

    return mb.get();
}