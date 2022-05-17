#include "scene.hpp"
#include "ocean.hpp"

using namespace cgp;

void scene_structure::initialize()
{
	// Basic set-up
	// ***************************************** //

	global_frame.initialize(mesh_primitive_frame(), "Frame");
	environment.camera.axis = camera_spherical_coordinates_axis::z;
	environment.camera.look_at({ -50.0f,0.0f,0.0f }, { 10,0,10 });

	//cubemap.initialize();

	//ship.create_ship();
	GLuint const shader = opengl_load_shader("C:/Users/lenovo/Desktop/code_inf443/scenes/inf443/inf443/shaders/smokeparticle/vert.glsl", "C:/Users/lenovo/Desktop/code_inf443/scenes/inf443/inf443/shaders/smokeparticle/frag.glsl");

	Particles = new ParticleGenerator(shader,  500);

	fps_record.start();
	total_time = 0;

	//Ocean::app_init();

}



void scene_structure::display() {

	total_time += timer.t / 10;
	//Ocean::app_render(1920, 1080, total_time, 0);
	//Ocean::app_update(timer.t / 10, environment);

	Particles->Update(0.1f,30);
	Particles->Draw();


	environment.camera.center_of_rotation = { 8, 1.5f, 8 };

	// Prevent camera from going underwater
	if (environment.camera.theta < 0) environment.camera.theta = 0;
	if (environment.camera.theta > Pi / 2) environment.camera.theta = Pi / 2;

	if (gui.firstPersonCamera) {
		environment.camera.distance_to_center = 3;
	}
	else {
		environment.camera.distance_to_center = gui.zoomLevel;
	}
		
	// cubemap.drawCubemap(environment);


	// Update the current time
	timer.update();

	// Basic elements of the scene
	environment.light = environment.camera.position();

	rotation_transform rotation = rotation_transform::from_axis_angle({ 0,1,0 }, 0);
	vec3 translation = { 0,0,0 };
	//ship.display_ship(environment, rotation, translation);

	

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


