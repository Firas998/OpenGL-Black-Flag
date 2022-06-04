#pragma once

#include "cgp/cgp.hpp"
#include <vector>

class cannonball {
public:
	int cannonposition;
	const static cgp::vec3 g;
	cgp::vec3 speed;
	cgp::vec3 position;
	cgp::affine_rts transform;
	cannonball(int pos, cgp::affine_rts &transformation,cgp::vec3 speed=cgp::vec3(0,0,1)) : cannonposition{ pos },speed{speed},transform{transformation}
	{
		position = transform * position;
	};
	void updateball(float dt);
};

class cannonballgenerator {
public:
	cannonballgenerator();
	cgp::scene_environment_basic_camera_spherical_coords* environment;
	std::vector<cannonball> cannonballs;
	std::vector<cgp::vec3> cannonpositions;
	std::vector<float> cannontimers;
	cgp::mesh_drawable cannon_drawable;
	void drawballs(float dt, cgp::scene_environment_basic_camera_spherical_coords& environment,cgp::affine_rts &transformation);

};