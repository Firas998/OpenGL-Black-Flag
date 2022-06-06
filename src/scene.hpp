#pragma once

#include "cgp/cgp.hpp"
#include "ship/ship.hpp"
#include "Smokeparticle.hpp"
#include "island.hpp"
#include "cannon.hpp"
#include "ocean.hpp"
#include <GLFW/glfw3.h>

// The element of the GUI that are not already stored in other structures
struct gui_parameters {
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

	cgp::mesh terrain_mesh;
	cgp::mesh_drawable terrain_drawable;
	perlin_noise_parameters parameters;
	island island;
	
	//Cannonballs
	cannonballgenerator Cannons; 
	bool left;
	bool right;
	ParticleGenerator* Particles;

	mesh_drawable hahaha;
	cgp::affine_rts ship1_transform;
	cgp::affine_rts ship2_transform;
	Ship player_ship;
	Ship other_ship;
	Ocean ocean;


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
	void update_from_key(int key,int action);


};





