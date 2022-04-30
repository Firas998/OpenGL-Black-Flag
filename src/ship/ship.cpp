#include "ship.hpp"
#include <iostream> 

using namespace cgp;

#define PI 3.14

void ship::create_ship() {
	loader.loadObj();
}
void ship::display_ship(cgp::scene_environment_basic_camera_spherical_coords environment) {
	rotation_transform R = rotation_transform::from_axis_angle({ 1,0,0 }, PI / 2);
	vec3 translation = {0,-10,0};
	for (int i = 0; i < loader.meshes.size(); i++) {
		loader.meshes[i].transform.rotation = R;
		loader.meshes[i].transform.translation = translation;
		draw(loader.meshes[i], environment);
	}
}