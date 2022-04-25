#include "ship.hpp"
#include <iostream> 

using namespace cgp;

void ship::create_ship() {
	loader.loadObj();
}
void ship::display_ship(cgp::scene_environment_basic_camera_spherical_coords environment) {
	for (int i = 0; i < loader.meshes.size(); i++) {
		draw(loader.meshes[i], environment);
	}
}