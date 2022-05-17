#include "scene.hpp"

using namespace cgp;

void scene_structure::initialize()
{
	// Basic set-up
	// ***************************************** //

	global_frame.initialize(mesh_primitive_frame(), "Frame");
	environment.camera.axis = camera_spherical_coordinates_axis::z;
	environment.camera.look_at({ -50.0f,0.0f,0.0f }, { 10,0,10 });

	

	ship.create_ship();

	fps_record.start();
	total_time = 0;

	ocean.init();

}



void scene_structure::display() {

	total_time += timer.t / 10;
	ocean.render(1920, 1080, total_time, environment);
	ocean.update(timer.t / 10, environment);

	environment.camera.center_of_rotation = { 4, 4, 5 };

	// Prevent camera from going underwater
	if (environment.camera.theta < -0.1) environment.camera.theta = -0.1;
	if (environment.camera.theta > Pi / 2) environment.camera.theta = Pi / 2;

	if (gui.firstPersonCamera) {
		environment.camera.distance_to_center = 3;
	}
	else {
		environment.camera.distance_to_center = gui.zoomLevel;
	}


	// Update the current time
	timer.update();

	// Basic elements of the scene
	environment.light = environment.camera.position();

	rotation_transform rotation = rotation_transform::from_axis_angle({ 0,1,0 }, 0);
	vec3 translation = { 0,0,-1 };
	ship.display_ship(environment, rotation, translation);

	

}


void scene_structure::display_gui()
{
	ImGui::Checkbox("First Person View", &gui.firstPersonCamera);
}

void scene_structure::set_window_title(GLFWwindow* window) {
	fps_record.update();
	std::string title = "INF443: Black Flag - Louis Caubet & Firas Ben Jedidia - " + str(fps_record.fps) + " fps";
	glfwSetWindowTitle(window, title.c_str());
}


