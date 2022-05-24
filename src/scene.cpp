#include "scene.hpp"
#include <algorithm>

using namespace cgp;

void scene_structure::initialize()
{
	// Basic set-up
	// ***************************************** //

	global_frame.initialize(mesh_primitive_frame(), "Frame");
	environment.camera.axis = camera_spherical_coordinates_axis::z;
	environment.camera.look_at({ -50.0f,0.0f,0.0f }, { 10,0,10 });

	mesh cube = mesh_primitive_cube();
	debug.initialize(cube);
	debug.shading.color = { 1,0,0 };
	debug2.initialize(cube);
	debug2.shading.color = { 1,0,0 };

	ship.create_ship();

	fps_record.start();
	total_time = 0;

	ocean.init();

}



void scene_structure::display() {

	// Update the current time
	float dt = timer.update();

	if (inputs.keyboard.up) {
		speed += timer.t;
		speed = std::min(speed, 20.0f);
	}
	else {
		speed -= timer.t;
		speed = std::max(speed, 0.0f);
	}

	if (inputs.keyboard.right) {
		angle += timer.t;
	}
	else if (inputs.keyboard.left) {
		angle -= timer.t;
	}

	total_time += timer.t / 10;
	ocean.render(1920, 1080, timer.t, environment);
	ocean.update(dt, environment);

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

	

	// Basic elements of the scene
	environment.light = environment.camera.position();

	vec3 ship_back = { 0, 5.5f, 0 };
	vec3 ship_front = { 12, 5.5f, 0 };
	vec3 cam_pos = environment.camera.position();

	vec3 relative_ship_back = ship_back - cam_pos;
	vec3 relative_ship_front = ship_front - cam_pos;
	

	ship_back.z = -ocean.getHeight(relative_ship_back.x, relative_ship_back.y) / 5;
	ship_front.z = -ocean.getHeight(relative_ship_front.x, relative_ship_front.y) / 5;

	debug.transform.translation = ship_back;
	debug2.transform.translation = ship_front;

	float angle = std::atan((ship_front.z - ship_back.z) / 12);
	float z = std::min(std::max(0.25f*(ship_back.z+ship_front.z) - 1, -1.5f), -1.0f);
	
	rotation_transform rotation = rotation_transform::from_axis_angle({ 0,1,0 }, 0.5f*angle);
	vec3 translation = { 0,0, z};
	ship.display_ship(environment, rotation, translation);

	draw(debug, environment);
	draw(debug2, environment);

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