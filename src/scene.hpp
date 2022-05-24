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

	Ocean ocean;
	ship ship;
	cgp::mesh_drawable debug;
	cgp::mesh_drawable debug2;

	cgp::timer_basic timer;

	float speed = 0.0f;
	float angle = 0.0f;
	cgp::vec3 position = { 0,0,0 };


	// ****************************** //
	// Functions
	// ****************************** //

	void initialize();  // Standard initialization to be called before the animation loop
	void display();     // The frame display to be called within the animation loop
	void display_gui(); // The display of the GUI, also called within the animation loop
	void set_window_title(GLFWwindow* window);

};





