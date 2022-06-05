#pragma once

#include "cgp/cgp.hpp"
#include <vector>

class cannonball {
public:

	const static cgp::vec3 g;
	cgp::vec3 speed;
	cgp::vec3 position;
	cgp::affine_rts transform;
	cannonball(cgp::vec3 theposition, cgp::affine_rts &transformation,float angle) : transform{transformation}
	{
		cgp::rotation_transform RTest = cgp::rotation_transform::from_axis_angle({0,0,1},angle);
		speed = cgp::vec3(0 , -25, 5);
		speed = RTest * speed;
		position = transform * theposition;
	};
	void updateball(float dt);
};

class cannonballgenerator {
public:

	std::vector<cannonball> cannonballs;
	std::vector<cgp::vec3> cannonpositions;
	std::vector<float> cannontimers;
	cgp::mesh_drawable cannon_drawable;
	void initialize();
	void drawballs(float dt, cgp::scene_environment_basic_camera_spherical_coords& environment, cgp::affine_rts& transformation, float angle);

};