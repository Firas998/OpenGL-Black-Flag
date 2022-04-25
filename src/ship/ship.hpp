#pragma once

#include "cgp/cgp.hpp"

#include "ship_loader.hpp"

struct ship {
	ShipLoader loader;
	void create_ship();
	void display_ship(cgp::scene_environment_basic_camera_spherical_coords environment);
};

