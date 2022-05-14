#include "cubemap.hpp"

using namespace cgp;

void Cubemap::initialize() {
    skybox.initialize("assets/cubemap/");
}

void Cubemap::drawCubemap(scene_environment_basic_camera_spherical_coords environment) {
    glDisable(GL_CULL_FACE);
    draw(skybox, environment);
    glEnable(GL_CULL_FACE);
}