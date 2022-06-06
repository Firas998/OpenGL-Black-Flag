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

	player_ship.create_ship();
	other_ship.create_ship();

	GLuint const shader = opengl_load_shader("shaders/smokeparticle/vert.glsl", "shaders/smokeparticle/frag.glsl");
	Cannons.initialize(shader);

	
	//terrain_mesh = island.create_terrain_mesh();
	//terrain_drawable.initialize(terrain_mesh, "terrain");
	//island.update_terrain(terrain_mesh, terrain_drawable, parameters);



	fps_record.start();
	ocean.init();
	// other_ship.sink();
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
		angle -= 0.05 * dt * speed;
	}
	else if (inputs.keyboard.left) {
		angle += 0.05 * dt * speed;
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
	player_ship.display_ship(environment,ship1_transform);

	other_ship.update_position(environment, vec3(0, 50, 0), Pi, ocean);
	other_ship.display_ship(environment,ship2_transform);

	draw(terrain_drawable, environment);
	//cannonballs
	float cannons_dt = 0.1;
	Cannons.drawballs(cannons_dt, environment, ship1_transform,ship2_transform,angle,left,right);




}


void scene_structure::display_gui()
{
	

	bool update = false;
	update |= ImGui::SliderFloat("Persistance", &parameters.persistency, 0.1f, 1.5f);
	update |= ImGui::SliderFloat("Frequency gain", &parameters.frequency_gain, 1.5f, 2.5f);
	update |= ImGui::SliderInt("Octave", &parameters.octave, 1, 8);
	update |= ImGui::SliderFloat("Height", &parameters.terrain_height, 0.1f, 20.0f);
	update |= ImGui::SliderFloat("Noise Scale", &parameters.noisescale, 0.0001f, 3.0f);

	if (update)// if any slider has been changed - then update the terrain
		island.update_terrain(terrain_mesh, terrain_drawable, parameters);

	

}

void scene_structure::set_window_title(GLFWwindow* window) {
	fps_record.update();
	std::string title = "INF443: Black Flag - Louis Caubet & Firas Ben Jedidia - " + str(fps_record.fps) + " fps";
	glfwSetWindowTitle(window, title.c_str());
}

void scene_structure::update_from_key(int key, int action) {
	if (key == GLFW_KEY_K) {
		if (action == GLFW_PRESS) left = true;
		if (action == GLFW_RELEASE) left = false;
	}

	if (key == GLFW_KEY_L) {
		if (action == GLFW_PRESS) right = true;
		if (action == GLFW_RELEASE) right = false;
	}
}

