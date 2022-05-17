#pragma once

#include "cgp/cgp.hpp"

struct planestruct {
	int size;
	void createplane(int sizeinput);
	cgp::mesh_drawable plane_drawable;
	void displayplane(cgp::scene_environment_basic_camera_spherical_coords environment);
};