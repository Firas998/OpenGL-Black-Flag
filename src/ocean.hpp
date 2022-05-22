#pragma once

#include <cgp/cgp.hpp>

typedef cgp::scene_environment_basic_camera_spherical_coords Environment;

class Ocean {

public:
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
	void render(unsigned width, unsigned height, float total_time, Environment environment);

	/// <summary>
	/// Cleanup
	/// </summary>
	void term();

	// Getters
	int getHeightmapWidth() {
		return heightmapWidth;
	}
	int getHeightmapHeight() {
		return heightmapHeight;
	}
	float getHeight(int x, int y) {
		int i = 128 - y;
		int j = 128 - x;
		return heightmap[heightmapWidth * i + j];
	}

private:
	int heightmapWidth;
	int heightmapHeight;
	float* heightmap;

};