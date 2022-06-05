#pragma once
#include <vector>
#include "cgp/cgp.hpp"
#include <queue>

using namespace cgp;


class Particle {
public:
	vec3 Position;
	vec3 Velocity;
    int textureindex;
	float Life;
    float radius;
    Particle(vec3 generatorposition, vec3 Velocity,float duration): Position{ generatorposition }, Velocity{ Velocity }, radius{ 5 }, Life(duration)
    {
        float random = ((double)rand() / (RAND_MAX)) * radius;
        float random2 = ((double)rand() / (RAND_MAX)) * radius;
        Position = vec3(random, random2, 0);
    }
};

class ParticleGenerator
{
public:
    ParticleGenerator(GLuint const shader,unsigned int amount,vec3 generatorposition=vec3(0,0,0), vec3 Velocity=vec3(0,0,0));
    void Update(float dt, unsigned int newParticles, vec3 generatorposition,vec3 Velocity);
    void Draw(cgp::scene_environment_basic_camera_spherical_coords &environment);
private:
    unsigned int shaderprogram;
    std::queue<Particle, std::deque<Particle>> particles;
    cgp::mesh_drawable drawable_quad;
    unsigned int amount;
    unsigned int lastUsedParticle;
    cgp::vec3 generatorposition;
    float TotalDuration = 10.0f;
    GLuint const texture_image_id= cgp::opengl_load_texture_image("assets/Explosion02_5x5.png",
        GL_REPEAT,
        GL_REPEAT);
    void init(vec3 generatorposition,vec3 Velocity);
    void respawnParticle(Particle& particle, vec3 generatorposition, vec3 Velocity);
};

