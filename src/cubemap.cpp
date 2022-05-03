#include "cubemap.hpp"

using namespace cgp;

void Cubemap::initialize() {
    skybox.initialize("assets/cubemap/");
}

void Cubemap::drawCubemap(scene_environment_basic_camera_spherical_coords environment) {
    draw(skybox, environment);
}