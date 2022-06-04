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

	GLuint const shader = opengl_load_shader("shaders/smokeparticle/vert.glsl", "shaders/smokeparticle/frag.glsl");

	Particles = new ParticleGenerator(shader,  500);
	player_ship.create_ship();
	other_ship.create_ship();

	fps_record.start();

	ocean.init();

	other_ship.sink();

}



void scene_structure::display() {

	total_time += timer.t / 10;

	Particles->Update(0.1f,30);
	Particles->Draw();

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

	environment.camera.distance_to_center = gui.zoomLevel;

	ocean.render(1920, 1080, timer.t, environment);
	ocean.update(dt, environment);

	
	// Basic elements of the scene
	environment.light = environment.camera.position();

	player_ship.update_position(environment, position, angle, ocean);
	player_ship.display_ship(environment);

	other_ship.update_position(environment, vec3(0, 50, 0), Pi, ocean);
	other_ship.display_ship(environment);
}


void scene_structure::display_gui()
{
	
}

void scene_structure::set_window_title(GLFWwindow* window) {
	fps_record.update();
	std::string title = "INF443: Black Flag - Louis Caubet & Firas Ben Jedidia - " + str(fps_record.fps) + " fps";
	glfwSetWindowTitle(window, title.c_str());
}