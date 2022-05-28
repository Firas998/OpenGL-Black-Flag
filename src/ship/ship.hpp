#pragma once

#include "cgp/cgp.hpp"
#include "ship_loader.hpp"
#include "../ocean.hpp"

typedef cgp::scene_environment_basic_camera_spherical_coords Environment;

class Ship {
public:
	void create_ship();
	void update_position(Environment env, cgp::vec3 base_position, float angle, Ocean ocean);
	void display_ship(Environment env);

private:
	ShipLoader loader;
	cgp::vec3 position;
	cgp::rotation_transform rotation;
};

