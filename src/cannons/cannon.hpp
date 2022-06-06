#pragma once

#include "cgp/cgp.hpp"
#include <vector>
#include "smokeparticle.hpp"
#include "ship/ship.hpp"

// Objets pour les boules tirées par les canons
class cannonball {
public:
	ParticleGenerator* cannongen;
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

//objets qui gérent les systèmes de particules de flash de museau
struct BlastParticleGenerator {
	ParticleGenerator* gen;
	cgp::vec3 cannon_local_position;
	cgp::vec3 Velocity;
	BlastParticleGenerator() {};
	BlastParticleGenerator(ParticleGenerator *gen, cgp::vec3 cannon_local_position, cgp::vec3 Velocity) :gen{ gen }, cannon_local_position{ cannon_local_position }, Velocity{ Velocity }{}
};

//Objet qui gère les canons du bateau
class cannonballgenerator {

private:
	GLuint texture_id;
	GLuint texture_id_smoke;
	int number_of_hits = 0;
	Ship* other_ship;
public:
	unsigned int shaderprogram;
	std::deque<cannonball*> cannonballs;
	std::deque<BlastParticleGenerator> particlegenerators;
	std::vector<cgp::vec3> cannonpositionsleft;
	std::vector<cgp::vec3> cannonpositionsright;
	std::vector<float> cannontimersleft;
	std::vector<float> cannontimersright;
	cgp::mesh_drawable cannon_drawable;
	cgp::mesh_drawable particle_drawable;
	cgp::mesh_drawable smoke_drawable;
	float particle_size_scale = 1.0f;
	void initialize(GLuint const shaderprogram, Ship &other_ship);
	void createblast(cgp::vec3 theposition, cgp::affine_rts& transformation, float angle,cgp::vec3 velocity);
	void drawballs(float dt, cgp::scene_environment_basic_camera_spherical_coords& environment, cgp::affine_rts& transformation, cgp::affine_rts& ship2_transform,float angle,bool left, bool right);
	void Draw_Update_Particles(float dt, cgp::affine_rts& transformation,float angle, cgp::scene_environment_basic_camera_spherical_coords& environment);

};
