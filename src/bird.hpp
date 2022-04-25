#pragma once

#include "cgp/cgp.hpp"

struct bird {

	cgp::mesh_drawable body;

	cgp::mesh_drawable head;
	cgp::mesh_drawable eye1;
	cgp::mesh_drawable eye2;
	cgp::mesh_drawable nose;

	cgp::mesh_drawable wing1_inner;
	cgp::mesh_drawable wing1_outer;
	cgp::mesh_drawable wing2_inner;
	cgp::mesh_drawable wing2_outer;

	cgp::hierarchy_mesh_drawable bird;

	void create_bird();
	void display_bird(cgp::scene_environment_basic_camera_spherical_coords environment, float time, cgp::vec3 translation, cgp::vec3 direction);

};

