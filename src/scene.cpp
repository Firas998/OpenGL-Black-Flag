#include "scene.hpp"

#include "terrain.hpp"
#include "tree.hpp"

#include "interpolation.hpp"


using namespace cgp;

void scene_structure::initialize()
{
	// Basic set-up
	// ***************************************** //

	global_frame.initialize(mesh_primitive_frame(), "Frame");
	environment.camera.axis = camera_spherical_coordinates_axis::z;
	environment.camera.look_at({ 15.0f,6.0f,6.0f }, { 0,0,0 });

	ship.create_ship();


	//int N_terrain_samples = 100;
	//float terrain_length = 20;
	//mesh const terrain_mesh = create_terrain_mesh(N_terrain_samples, terrain_length, perlin_noise_parameters());
	//terrain.initialize(terrain_mesh, "terrain");
	//terrain.shading.color = { 0.6f,0.85f,0.5f };
	//terrain.shading.phong.specular = 0.0f; // non-specular terrain material

	//mesh const tree_mesh = create_tree();
	//tree.initialize(tree_mesh, "tree");

	//mesh const quad = mesh_primitive_quadrangle({ -0.5f,0,0 }, { 0.5f,0,0 }, { 0.5f,0,1 }, { -0.5f,0,1 });
	//quad_drawable.initialize(quad, "Quad");
	//quad_drawable.texture = opengl_load_texture_image("assets/grass.png");
	//quad_drawable.transform.scaling = 0.5f;
	//quad_drawable.shading.phong = { 0.4f, 0.6f,0,1 };

	//tree_position = generate_positions_on_terrain(150, terrain_length);
	//billboard_position = generate_positions_on_terrain(90, terrain_length);

	//GLuint const texture_image_id = opengl_load_texture_image("assets/texture_grass.jpg",
	//	GL_REPEAT,
	//	GL_REPEAT);

	//terrain.texture = texture_image_id;

	// bird.create_bird();

	// Key 3D positions
	//buffer<vec3> key_positions =
	//{ {-1,1,3}, {0,1,3}, {1,1,3}, {1,2,3}, {2,2,3}, {2,2,4}, {2,0,4.5}, {1.5,-1,4}, {1.5,-1,3}, {1,-1,3}, {0,-0.5,3}, {-1,-0.5,3},{-1,1,3}, {0,1,3}, {1,1,3} };

	//// Key times (time at which the position must pass in the corresponding position)
	//buffer<float> key_times =
	//{ 0.0f, 1.0f, 2.0f, 2.5f, 3.0f, 3.5f, 3.75f, 4.5f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f };

	//// Initialize the helping structure to display/interact with these positions
	//keyframe.initialize(key_positions, key_times);

	//int N = key_times.size();
	//timer.t_min = key_times[1];
	//timer.t_max = key_times[N - 2];
	//timer.t = timer.t_min;

}





void scene_structure::display()
{

	// Update the current time
	timer.update();

	// Basic elements of the scene
	environment.light = environment.camera.position();
	if (gui.display_frame)
		draw(global_frame, environment);

	ship.display_ship(environment);

	//draw(terrain, environment);
	//if (gui.display_wireframe) {
	//	draw_wireframe(terrain, environment);
	//}
	//	
	//for (int i = 0; i < 150; i++) {
	//	tree.transform.translation = tree_position[i];
	//	draw(tree, environment);
	//	if(gui.display_wireframe) draw_wireframe(tree, environment);
	//}

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glDepthMask(false);

	//// Re-orient the grass shape to always face the camera direction
	//vec3 const front = normalize(environment.camera.front() * vec3 { 1, 1, 0 }); // front-vector of the camera without z-component
	//vec3 const right = environment.camera.right();
	//rotation_transform R = rotation_transform::between_vector({ 1,0,0 }, { 0,1,0 }, right, front);

	//for (int i = 0; i < 90; i++) {
	//	quad_drawable.transform.translation = billboard_position[i];
	//	quad_drawable.transform.rotation = R;
	//	draw(quad_drawable, environment);
	//}

	//// Don't forget to re-activate the depth-buffer write
	//glDepthMask(true);
	//glDisable(GL_BLEND);

	//vec3 p = interpolation(timer.t, keyframe.key_positions, keyframe.key_times);
	//vec3 direction = interpolation_direction(timer.t, keyframe.key_positions, keyframe.key_times);

	//keyframe.display_key_positions(environment);

	//bird.display_bird(environment, timer.t, p, direction);

	

}





void scene_structure::display_gui()
{
	ImGui::Checkbox("Frame", &gui.display_frame);
	ImGui::Checkbox("Wireframe", &gui.display_wireframe);
}


