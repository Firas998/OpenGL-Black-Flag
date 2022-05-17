#pragma once

#include "cgp/cgp.hpp"
#include "ship/ship.hpp"
#include "ocean.hpp"


// The element of the GUI that are not already stored in other structures
struct gui_parameters {
	bool firstPersonCamera = false;
	float zoomLevel = 50;
	float defaultScroll = 0;
};



// The structure of the custom scene
struct scene_structure {
	
	// ****************************** //
	// Elements and shapes of the scene
	// ****************************** //

	cgp::mesh_drawable global_frame;          // The standard global frame
	cgp::scene_environment_basic_camera_spherical_coords environment; // Standard environment controler
	cgp::inputs_interaction_parameters inputs; // Storage for inputs status (mouse, keyboard, window dimension)

	cgp::timer_fps fps_record;

	gui_parameters gui;                       // Standard GUI element storage

	struct Ocean ocean;
	ship ship;
	
	/*cgp::mesh_drawable terrain;
	cgp::mesh_drawable tree;
	cgp::mesh_drawable quad_drawable;

	std::vector<cgp::vec3> tree_position;
	std::vector<cgp::vec3> billboard_position;*/

	// Timer used for the animation
	cgp::timer_interval timer;
	float total_time;


	// ****************************** //
	// Functions
	// ****************************** //

	void initialize();  // Standard initialization to be called before the animation loop
	void display();     // The frame display to be called within the animation loop
	void display_gui(); // The display of the GUI, also called within the animation loop
	void set_window_title(GLFWwindow* window);


};





