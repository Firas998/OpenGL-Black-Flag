#include "bird.hpp"

using namespace cgp;

void bird::create_bird() {

	mesh body_mesh = mesh_primitive_ellipsoid({ 0.2f,0.1f,0.1f });
	body.initialize(body_mesh, "Body");

	mesh head_mesh = mesh_primitive_sphere(0.07f);
	head.initialize(head_mesh, "Head");

	mesh eye_mesh = mesh_primitive_sphere(0.01f);
	eye1.initialize(eye_mesh, "Eye1");
	eye2.initialize(eye_mesh, "Eye2");
	eye1.shading.color = { 0,0,0 };
	eye2.shading.color = { 0,0,0 };

	mesh nose_mesh = mesh_primitive_cone(0.02f, 0.05f, { 0,0,0 }, { 1,0,0 });
	nose.initialize(nose_mesh, "Nose");
	nose.shading.color = { 1, 1, 0 };

	mesh wing_inner_mesh = mesh_primitive_quadrangle({ 0,0,0 }, { 0.2,0,0 }, { 0.2,0.2,0 }, { 0,0.2,0 });
	mesh wing_inner_mesh_reverse = mesh_primitive_quadrangle({ 0,0,0 }, { 0.2,0,0 }, { 0.2,-0.2,0 }, { 0,-0.2f,0 });
	mesh wing_outer_mesh = mesh_primitive_quadrangle({ 0,0,0 }, { 0.2,0,0 }, { 0.125f, 0.1, 0 }, { 0.05f, 0.1, 0 });
	mesh wing_outer_mesh_reverse = mesh_primitive_quadrangle({ 0,0,0 }, { 0.2,0,0 }, { 0.125f, -0.1, 0 }, { 0.05f, -0.1, 0 });

	wing1_inner.initialize(wing_inner_mesh, "Wing1 inner");
	wing2_inner.initialize(wing_inner_mesh_reverse, "Wing2 inner");

	wing1_outer.initialize(wing_outer_mesh, "Wing1 outer");
	wing2_outer.initialize(wing_outer_mesh_reverse, "Wing2 outer");

	bird.add(body);

	bird.add(head, "Body", { 0.15, 0, 0.05f });
	bird.add(eye1, "Head", { 0.06f,0.02f,0.02f });
	bird.add(eye2, "Head", { 0.06f,-0.02f,0.02f });
	bird.add(nose, "Head", { 0.06f, 0, 0 });

	bird.add(wing1_inner, "Body", { -0.1f, 0, 0 });
	bird.add(wing2_inner, "Body", { -0.1f, 0, 0 });
	bird.add(wing1_outer, "Wing1 inner", { 0, 0.2f, 0 });
	bird.add(wing2_outer, "Wing2 inner", { 0, -0.2f, 0 });

}

void bird::display_bird(scene_environment_basic_camera_spherical_coords environment, float time, vec3 translation, vec3 direction) {
	bird.update_local_to_global_coordinates();

	bird["Body"].transform.translation = translation;
	bird["Body"].transform.scaling = 3;
	bird["Body"].transform.rotation = rotation_transform::between_vector({ 1,0,0 }, direction / norm(direction));

	bird["Wing1 inner"].transform.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, cos(10 * time) / 2.0f);
	bird["Wing2 inner"].transform.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, cos(10 * time + 3.14f) / 2.0f);

	bird["Wing1 outer"].transform.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, cos(10 * time));
	bird["Wing2 outer"].transform.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, cos(10 * time + 3.14f));

	bird["Head"].transform.rotation = rotation_transform::from_axis_angle({ 0,1,0 }, cos(4 * time) / 4.0f);

	draw(bird, environment);
}