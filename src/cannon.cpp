#include "cannon.hpp"
#include <iostream> 
#include <vector>

using namespace cgp;

const cgp::vec3 cannonball::g = cgp::vec3(0, 0, -9.81f);

void cannonballgenerator::initialize(GLuint const prog) {
	shaderprogram = prog;
	texture_id = cgp::opengl_load_texture_image("assets/Explosion02_5x5.png",
		GL_REPEAT,
		GL_REPEAT);

	particle_drawable.initialize(mesh_primitive_quadrangle(vec3(-1, 0, -1) * particle_size_scale, vec3(1, 0, -1) * particle_size_scale, vec3(1, 0, 1) * particle_size_scale, vec3(-1, 0, 1) * particle_size_scale), "Particle_mesh");
	particle_drawable.shader = shaderprogram;
	GLuint const texture_particles = cgp::opengl_load_texture_image("assets/Explosion02_5x5.png",
		GL_REPEAT,
		GL_REPEAT);
	particle_drawable.texture = texture_particles;

	cannon_drawable.initialize(mesh_primitive_sphere(0.25f), "sphere");
	GLuint const texture_image_id = opengl_load_texture_image("assets/iron.png",
		GL_REPEAT,
		GL_REPEAT);
	cannon_drawable.texture = texture_image_id;
	cannontimersleft = std::vector<float>({ 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,1.0f });
	cannontimersright = std::vector<float>({ 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,1.0f });


	cannonpositionsright.push_back((vec3(23.264f, 4.6946f, -7.8289f) + vec3(23.264f, 4.6025f, -7.8289f)) / 2);
	cannonpositionsright.push_back((vec3(20.218f, 4.45f, -7.1893f) + vec3(20.218f, 4.35865f, -7.1893f)) / 2);
	cannonpositionsright.push_back((vec3(17.343f, 4.27f, -6.8868f) + vec3(17.343f, 4.1786f, -6.8868f)) / 2);
	cannonpositionsright.push_back((vec3(14.435f, 4.1542, -6.8471) + vec3(14.435f, 4.621f, -6.8471)) / 2);
	cannonpositionsright.push_back((vec3(11.505f, 4.1542f, -6.9161) + vec3(11.505f, 4.0621, -6.9161)) / 2);
	cannonpositionsright.push_back((vec3(8.6325f, 4.3022f, -7.1158f) + vec3(8.6325f, 4.21f, -7.1158f)) / 2);
	cannonpositionsright.push_back((vec3(5.7567, 4.5144f, -7.3844f) + vec3(5.7567, 4.4223, -7.3844f)) / 2);
	cannonpositionsright.push_back((vec3(2.9597f, 4.8406f, -7.8051f) + vec3(2.9597f, 4.7485f, -7.8051f)) / 2);
	cannonpositionsright.push_back((vec3(0.051582f, 5.2053f, -8.166f) + vec3(0.051582f, 5.1132f, -8.166f)) / 2);

	
	cannonpositionsleft.push_back((vec3(23.264f, 4.6946f, -14.958) + vec3(23.264f, 4.6025f, -14.958)) / 2);
	cannonpositionsleft.push_back((vec3(20.218f, 4.45f, -15.573) + vec3(20.218f, 4.35865f, -15.573)) / 2);
	cannonpositionsleft.push_back((vec3(17.343f, 4.27f, -15.876) + vec3(17.343f, 4.1786f, -15.876)) / 2);
	cannonpositionsleft.push_back((vec3(14.435f, 4.1542, -15.915) + vec3(14.435f, 4.621f, -15.915)) / 2);
	cannonpositionsleft.push_back((vec3(11.505f, 4.1542f, -15.846) + vec3(11.505f, 4.0621, -15.846)) / 2);
	cannonpositionsleft.push_back((vec3(8.6325f, 4.3022f, -15.647) + vec3(8.6325f, 4.21f, -15.647)) / 2);
	cannonpositionsleft.push_back((vec3(5.7567, 4.5144f, -15.378) + vec3(5.7567, 4.4223, -15.378)) / 2);
	cannonpositionsleft.push_back((vec3(2.9597f, 4.8406f, -14.958) + vec3(2.9597f, 4.7485f, -14.958)) / 2);
	cannonpositionsleft.push_back((vec3(0.051582f, 5.2053f, -14.597) + vec3(0.051582f, 5.1132f, -14.597)) / 2);
}


void cannonball::updateball(float dt) {
	speed = speed + dt * g;
	position=position + dt * speed;
}
void cannonballgenerator::drawballs(float dt, cgp::scene_environment_basic_camera_spherical_coords& environment,cgp::affine_rts &transformation,float angle,bool left, bool right) {
	for (int i = 0; i < 9; i++) {
		cannontimersleft[i] -= dt;
		cannontimersright[i] -= dt;

		if (cannontimersleft[i] < 0 && left) {
			cannontimersleft[i] = 3 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2 - 1)));
			cannonball *ball = new cannonball(cannonpositionsleft[i], transformation, angle, cgp::vec3(0, 25, 15));
			cannonballs.push_back(ball);
			createblast(cannonpositionsleft[i], transformation, angle,cgp::vec3(0,1,0));
		}
		else if (cannontimersright[i] < 0 && right) {
			cannontimersright[i] = 3 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2 - 1)));
			cannonball* ball = new cannonball(cannonpositionsleft[i], transformation, angle, cgp::vec3(0, -25, 15));
			cannonballs.push_back(ball);
			createblast(cannonpositionsright[i], transformation, angle, cgp::vec3(0, -1, 0));
		}
	}

	while (cannonballs.size() > 0 && cannonballs.front()->position.z < 0) {
		delete cannonballs.front();
		cannonballs.pop_front();
	}


	for (cannonball *ball : cannonballs)
	{
		ball->updateball(dt);
		cannon_drawable.transform.translation = ball->position;
		draw(cannon_drawable, environment);
	}
	Draw_Update_Particles(dt, transformation, angle, environment);
}

void cannonballgenerator::createblast(cgp::vec3 theposition, cgp::affine_rts& transformation, float angle,cgp::vec3 Velocity) {

	cgp::rotation_transform RTest = cgp::rotation_transform::from_axis_angle({ 0,0,1 }, angle);
	vec3 speed = RTest * Velocity;
	vec3 position = transformation * theposition;
	
	ParticleGenerator *p = new ParticleGenerator( shaderprogram, texture_id, 10,position,speed,0 );
	BlastParticleGenerator b { p,theposition,Velocity };
	
	particlegenerators.push_back(b);

};

void cannonballgenerator::Draw_Update_Particles(float dt, cgp::affine_rts& transformation, float angle, cgp::scene_environment_basic_camera_spherical_coords& environment){
	while (particlegenerators.size() > 0 && particlegenerators.front().gen->Life < 0) {
		delete particlegenerators.front().gen;
		particlegenerators.pop_front();
	}

	for (int i = 0; i < particlegenerators.size(); i++) {
		ParticleGenerator* p = particlegenerators[i].gen;
		p->Life = p->Life - dt;
		if (p->Life > 0.0f)
		{
			cgp::rotation_transform RTest = cgp::rotation_transform::from_axis_angle({ 0,0,1 }, angle);
			cgp::vec3 speed = RTest * particlegenerators[i].Velocity;
			cgp::vec3 position = transformation * particlegenerators[i].cannon_local_position;
			particlegenerators[i].gen->Update(dt, 2, position, speed);
			particlegenerators[i].gen->Draw(environment, particle_drawable);
		}
		
	}
};