#pragma once

#include <cgp/cgp.hpp>

typedef cgp::scene_environment_basic_camera_spherical_coords Environment;

struct Ocean {

	/// <summary>
	/// Initialize the ocean. Call at scene initialization.
	/// </summary>
	void init();

	/// <summary>
	/// Updates the camera position and direction. Call this before rerendering. 
	/// </summary>
	/// <param name="delta_time">time since last call</param>
	/// <param name="environment">the CGP environment object</param>
	void update(float delta_time, Environment environment);

	/// <summary>
	/// Renders the ocean. 
	/// </summary>
	/// <param name="width">Window width</param>
	/// <param name="height">Window height</param>
	/// <param name="total_time">Total time since simulation start.</param>
	/// <param name="environment">the CGP environment object</param>
	/// <returns>The heightmap of the current state of the ocean.</returns>
	float* render(unsigned width, unsigned height, float total_time, Environment environment);

	/// <summary>
	/// Cleanup
	/// </summary>
	void term();
};