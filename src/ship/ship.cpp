#include "ship.hpp"
#include <iostream> 

using namespace cgp;

#define PI 3.14

void ship::create_ship() {
	loader.loadObj();
}
void ship::display_ship(cgp::scene_environment_basic_camera_spherical_coords environment, rotation_transform rotation, vec3 translation) {
	rotation_transform R = rotation * 
		rotation_transform::from_axis_angle({ 1,0,0 }, PI / 2) * 
		rotation_transform::from_axis_angle({0,0,1}, 0.05);
	vec3 t = {0,0,1};
	for (int i = 0; i < loader.meshes.size(); i++) {
		loader.meshes[i].transform.rotation = R;
		loader.meshes[i].transform.translation = translation + t;
		loader.meshes[i].transform.scaling = 0.5;
		draw(loader.meshes[i], environment);
	}
}