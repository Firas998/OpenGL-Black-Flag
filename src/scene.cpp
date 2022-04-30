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
	environment.camera.look_at({ -50.0f,0.0f,0.0f }, { 10,0,10 });

	ship.create_ship();

}





void scene_structure::display() {

	environment.camera.center_of_rotation = { 8, 1.5f, 8 };

	if (gui.firstPersonCamera) {
		environment.camera.distance_to_center = 3;
	}
	else {
		environment.camera.distance_to_center = 50;
	}
		

	// Update the current time
	timer.update();

	// Basic elements of the scene
	environment.light = environment.camera.position();

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
	ImGui::Checkbox("First Person View", &gui.firstPersonCamera);
}


