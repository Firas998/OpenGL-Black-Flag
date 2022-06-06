#pragma once

#include "cgp/cgp.hpp"
#include <vector>
#include <smokeparticle.hpp>

class cannonball {
public:

	const static cgp::vec3 g;
	cgp::vec3 speed;
	cgp::vec3 position;
	cgp::affine_rts transform;
	cannonball(cgp::vec3 theposition, cgp::affine_rts &transformation,float angle,cgp::vec3 speedy) : transform{transformation}
	{
		cgp::rotation_transform RTest = cgp::rotation_transform::from_axis_angle({0,0,1},angle);
		speed = RTest * speedy;
		position = transform * theposition;
	};
	void updateball(float dt);
};

struct BlastParticleGenerator {
	ParticleGenerator* gen;
	cgp::vec3 cannon_local_position;
	cgp::vec3 Velocity;
	BlastParticleGenerator() {};
	BlastParticleGenerator(ParticleGenerator *gen, cgp::vec3 cannon_local_position, cgp::vec3 Velocity) :gen{ gen }, cannon_local_position{ cannon_local_position }, Velocity{ Velocity }{}
};

class cannonballgenerator {
public:
	unsigned int shaderprogram;
	std::vector<cannonball*> cannonballs;
	std::deque<BlastParticleGenerator> particlegenerators;
	std::deque<BlastParticleGenerator> smokegenerators;
	std::vector<cgp::vec3> cannonpositionsleft;
	std::vector<cgp::vec3> cannonpositionsright;
	std::vector<float> cannontimersleft;
	std::vector<float> cannontimersright;
	cgp::mesh_drawable cannon_drawable;
	cgp::mesh_drawable particle_drawable;
	float particle_size_scale = 1.0f;
	void initialize(GLuint const shaderprogram);
	void createblast(cgp::vec3 theposition, cgp::affine_rts& transformation, float angle,cgp::vec3 velocity);
	void drawballs(float dt, cgp::scene_environment_basic_camera_spherical_coords& environment, cgp::affine_rts& transformation, float angle,bool left, bool right);
	void Draw_Update_Particles(float dt, cgp::affine_rts& transformation,float angle, cgp::scene_environment_basic_camera_spherical_coords& environment);
};
