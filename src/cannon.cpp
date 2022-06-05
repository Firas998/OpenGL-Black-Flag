#include "cannon.hpp"
#include <iostream> 
#include <vector>

using namespace cgp;

const cgp::vec3 cannonball::g = cgp::vec3(0, 0, -9.81f);

void cannonballgenerator::initialize() {
	cannon_drawable.initialize(mesh_primitive_sphere(0.25f), "sphere");
	//cannon_drawable.shading.phong.diffuse = 0;
	//cannon_drawable.shading.phong.specular = 1;
	//cannon_drawable.shading.phong.specular_exponent = 128;
	GLuint const texture_image_id = opengl_load_texture_image("assets/iron.png",
		GL_REPEAT,
		GL_REPEAT);
	cannon_drawable.texture = texture_image_id;
	cannontimersleft = std::vector<float>({ 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,1.0f });
	cannontimersright = std::vector<float>({ 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,1.0f });

	cannonpositionsright.push_back((vec3(23.264f, 4.6946f, -7.8289f) + vec3(23.264f, 4.6025f, -7.8289f)) / 2);
	cannonpositionsright.push_back((vec3(20.218f, 4.45f, -7.1893f) + vec3(20.218f, 4.35865f, -7.1893f)) / 2);
	cannonpositionsright.push_back((vec3(17.343f, 4.27f, -6.8868f) + vec3(17.343f, 4.1786f, -6.8868f)) / 2);
	cannonpositionsright.push_back((vec3(14.435f, 4.1542, -6.8471) + vec3(14.435f, 4.621f, -6.8471)) / 2);
	cannonpositionsright.push_back((vec3(11.505f, 4.1542f, -6.9161) + vec3(11.505f, 4.0621, -6.9161)) / 2);
	cannonpositionsright.push_back((vec3(8.6325f, 4.3022f, -7.1158f) + vec3(8.6325f, 4.21f, -7.1158f)) / 2);
	cannonpositionsright.push_back((vec3(5.7567, 4.5144f, -7.3844f) + vec3(5.7567, 4.4223, -7.3844f)) / 2);
	cannonpositionsright.push_back((vec3(2.9597f, 4.8406f, -7.8051f) + vec3(2.9597f, 4.7485f, -7.8051f)) / 2);
	cannonpositionsright.push_back((vec3(0.051582f, 5.2053f, -8.166f) + vec3(0.051582f, 5.1132f, -8.166f)) / 2);

	
	cannonpositionsleft.push_back((vec3(23.264f, 4.6946f, -14.958) + vec3(23.264f, 4.6025f, -14.958)) / 2);
	cannonpositionsleft.push_back((vec3(20.218f, 4.45f, -15.573) + vec3(20.218f, 4.35865f, -15.573)) / 2);
	cannonpositionsleft.push_back((vec3(17.343f, 4.27f, -15.876) + vec3(17.343f, 4.1786f, -15.876)) / 2);
	cannonpositionsleft.push_back((vec3(14.435f, 4.1542, -15.915) + vec3(14.435f, 4.621f, -15.915)) / 2);
	cannonpositionsleft.push_back((vec3(11.505f, 4.1542f, -15.846) + vec3(11.505f, 4.0621, -15.846)) / 2);
	cannonpositionsleft.push_back((vec3(8.6325f, 4.3022f, -15.647) + vec3(8.6325f, 4.21f, -15.647)) / 2);
	cannonpositionsleft.push_back((vec3(5.7567, 4.5144f, -15.378) + vec3(5.7567, 4.4223, -15.378)) / 2);
	cannonpositionsleft.push_back((vec3(2.9597f, 4.8406f, -14.958) + vec3(2.9597f, 4.7485f, -14.958)) / 2);
	cannonpositionsleft.push_back((vec3(0.051582f, 5.2053f, -14.597) + vec3(0.051582f, 5.1132f, -14.597)) / 2);
}


void cannonball::updateball(float dt) {
	speed = speed + dt * g;
	position=position + dt * speed;

	
}
void cannonballgenerator::drawballs(float dt, cgp::scene_environment_basic_camera_spherical_coords& environment,cgp::affine_rts &transformation,float angle,bool left, bool right) {
	for (int i = 0; i < 9; i++) {
		cannontimersleft[i] -= dt;
		if (cannontimersleft[i] < 0 && left) {
			cannontimersleft[i] = 3 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (4 - 1)));
			cannonballs.push_back({ cannonpositionsleft[i],transformation,angle,cgp::vec3(0,25,15)});
		}
	}
	for (int i = 0; i < 9; i++) {
		cannontimersright[i] -= dt;
		if (cannontimersright[i] < 0 && right) {
			cannontimersright[i] = 3 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (4 - 1)));
			cannonballs.push_back({ cannonpositionsright[i],transformation,angle,cgp::vec3(0,-25,15) });
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