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

	ocean.init();

}



void scene_structure::display() {

	// Update the current time
	float dt = timer.update();

	if (inputs.keyboard.up) {
		speed += dt;
		speed = std::min(speed, 10.0f);
	}
	else {
		speed -= dt;
		speed = std::max(speed, 0.0f);
	}

	if (inputs.keyboard.right) {
		angle -= 0.05*dt*speed;
	}
	else if (inputs.keyboard.left) {
		angle += 0.05*dt*speed;
	}

	position = {
		position.x + dt * speed * std::cos(angle),
		position.y + dt * speed * std::sin(angle),
		position.z
	};

	environment.camera.center_of_rotation = position + vec3(4, 4, 5);

	// Prevent camera from going underwater
	if (environment.camera.theta < -0.1) environment.camera.theta = -0.1;
	if (environment.camera.theta > Pi / 2) environment.camera.theta = Pi / 2;

	if (gui.firstPersonCamera) {
		environment.camera.distance_to_center = 3;
	}
	else {
		environment.camera.distance_to_center = gui.zoomLevel;
	}

	ocean.render(1920, 1080, timer.t, environment);
	ocean.update(dt, environment);

	
	// Basic elements of the scene
	environment.light = environment.camera.position();

	vec3 ship_back = position + vec3(0, 5.5f, 0);
	vec3 ship_front = position + vec3(12, 5.5f, 0);
	vec3 cam_pos = environment.camera.position();

	vec3 relative_ship_back = ship_back - cam_pos;
	vec3 relative_ship_front = ship_front - cam_pos;
	

	ship_back.z = -ocean.getHeight(relative_ship_back.x, relative_ship_back.y) / 5;
	ship_front.z = -ocean.getHeight(relative_ship_front.x, relative_ship_front.y) / 5;

	debug.transform.translation = ship_back;
	debug2.transform.translation = ship_front;

	float pitch = std::atan((ship_front.z - ship_back.z) / 12);
	// float z = std::min(std::max(0.25f*(ship_back.z+ship_front.z) - 1, -1.5f), -1.0f);
	float z = -ocean.getHeight(
		(relative_ship_front.x+relative_ship_back.x) / 2, 
		(relative_ship_front.y+relative_ship_back.y) / 2
	) / 10 - 1.0f;

	z = std::min(std::max(z, -1.5f), -1.0f);
	
	rotation_transform rotation = rotation_transform::from_axis_angle({ 0,1,0 }, 0.5f * pitch)
		* rotation_transform::from_axis_angle({ 0,0,1 }, angle);
	vec3 translation = position + vec3(0,0, z);
	ship.display_ship(environment, rotation, translation);

	cgp::draw(debug, environment);
	cgp::draw(debug2, environment);

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