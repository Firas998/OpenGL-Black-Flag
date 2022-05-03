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
	cgp::skybox_drawable skybox;
	unsigned int loadCubemap(std::vector<std::string> faces);
};