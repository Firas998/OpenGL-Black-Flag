#include "cannon.hpp"
#include <iostream> 
#include <vector>

using namespace cgp;

const cgp::vec3 cannonball::g = cgp::vec3(0, 0, -9.81f);

cannonballgenerator::cannonballgenerator() {
	cannon_drawable.initialize(mesh_primitive_sphere(0.1f), "sphere");
	cannontimers = std::vector<float>({ 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,1.0f });
	cannonpositions.push_back((vec3(23.264f, 4.6946f, -7.8289f) + vec3(23.264f, 4.6025f, -7.8289f)) / 2);
	cannonpositions.push_back((vec3(20.218f, 4.45f, -7.1893f) + vec3(20.218f, 4.35865f, -7.1893f)) / 2);
	cannonpositions.push_back((vec3(17.343f, 4.27f, -6.8868f) + vec3(17.343f, 4.1786f, -6.8868f)) / 2);
	cannonpositions.push_back((vec3(14.435f, 4.1542, -6.8471) + vec3(14.435f, 4.621f, -6.8471)) / 2);
	cannonpositions.push_back((vec3(11.505f, 4.1542f, -6.9161) + vec3(11.505f, 4.0621, -6.9161)) / 2);
	cannonpositions.push_back((vec3(8.6325f, 4.3022f, -7.1158f) + vec3(8.6325f, 4.21f, -7.1158f)) / 2);
	cannonpositions.push_back((vec3(5.7567, 4.5144f, -7.3844f) + vec3(5.7567, 4.4223, -7.3844f)) / 2);
	cannonpositions.push_back((vec3(2.9597f, 4.8406f, -7.8051f) + vec3(2.9597f, 4.7485f, -7.8051f)) / 2);
	cannonpositions.push_back((vec3(0.051582f, 5.2053f, -8.166f) + vec3(0.051582f, 5.1132f, -8.166f)) / 2);
}


void cannonball::updateball(float dt) {
	speed = speed + dt * g;
	position=position + dt * speed;

	
}
void cannonballgenerator::drawballs(float dt, cgp::scene_environment_basic_camera_spherical_coords& environment,cgp::affine_rts &transformation) {
	for (int i = 0; i < 9; i++) {
		cannontimers[i] -= dt;
		if (cannontimers[i] < 0) {
			cannontimers[i] = 3 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (7 - 3)));
			cannonballs.push_back({ i,transformation });
		}
	}
	for (auto it = cannonballs.begin(); it != cannonballs.end(); ) {
		if (it->position.z < 0)
			it = cannonballs.erase(it);
		if (it != cannonballs.end())
			++it;
	}


	for (cannonball& ball : cannonballs)
	{
		ball.updateball(dt);
		cannon_drawable.transform.translation = ball.position;
		draw(cannon_drawable, environment);
	}
}