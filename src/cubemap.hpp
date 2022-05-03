#pragma once
#include <iostream>
#include <vector>
#include <cgp/cgp.hpp>


class Cubemap {
public: 
	void initialize();
	void drawCubemap(cgp::scene_environment_basic_camera_spherical_coords environment);
private:
	unsigned int cubemap;
	unsigned int shader;
	cgp::mesh_drawable skybox_top_drawable;
	cgp::mesh_drawable skybox_bottom_drawable;
	cgp::mesh_drawable skybox_left_drawable;
	cgp::mesh_drawable skybox_right_drawable;
	cgp::mesh_drawable skybox_front_drawable;
	cgp::mesh_drawable skybox_back_drawable;
	unsigned int loadCubemap(std::vector<std::string> faces);
	void cgp_draw_custom(cgp::mesh_drawable drawable, cgp::scene_environment_basic_camera_spherical_coords environment);
};