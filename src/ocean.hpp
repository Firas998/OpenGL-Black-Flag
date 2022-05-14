#pragma once

#include <cgp/cgp.hpp>

struct Ocean {
	static void app_init();
	static void app_update(float delta_time, cgp::scene_environment_basic_camera_spherical_coords environment);
	static void app_render(unsigned width, unsigned height, float total_time, unsigned mesh_index);
	static void app_term();
};