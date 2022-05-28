#include "ship.hpp"
#include <iostream> 

using namespace cgp;

#define PI 3.14

void Ship::create_ship() {
	loader.loadObj();
}

void Ship::update_position(Environment env, vec3 base_position, float angle, Ocean ocean) {
	this->position = base_position;
	auto yaw = rotation_transform::from_axis_angle({ 0,0,1 }, angle);

	vec3 ship_back = position + vec3(0, 5.5f, 0);
	vec3 ship_front = position + yaw*vec3(12, 5.5f, 0);
	vec3 cam_pos = env.camera.position();

	vec3 relative_ship_back = ship_back - cam_pos;
	vec3 relative_ship_front = ship_front - cam_pos;

	ship_back.z = -ocean.getHeight(relative_ship_back.x, relative_ship_back.y) / 5;
	ship_front.z = -ocean.getHeight(relative_ship_front.x, relative_ship_front.y) / 5;

	float pitch = std::atan((ship_front.z - ship_back.z) / 12);

	float z = -ocean.getHeight(
		(relative_ship_front.x + relative_ship_back.x) / 2,
		(relative_ship_front.y + relative_ship_back.y) / 2
	) / 10 - 1.0f;

	z = std::min(std::max(z, -1.5f), -1.0f);

	rotation = rotation_transform::from_axis_angle({ 0,1,0 }, 0.5f * pitch) * yaw;
	position = position + vec3(0, 0, z);
}

void Ship::display_ship(Environment env) {
	rotation_transform R = rotation * 
		rotation_transform::from_axis_angle({ 1,0,0 }, PI / 2) * 
		rotation_transform::from_axis_angle({0,0,1}, 0.05);
	vec3 t = {0,0,1};
	for (int i = 0; i < loader.meshes.size(); i++) {
		loader.meshes[i].transform.rotation = R;
		loader.meshes[i].transform.translation = position + t;
		loader.meshes[i].transform.scaling = 0.5;
		draw(loader.meshes[i], env);
	}
	
}