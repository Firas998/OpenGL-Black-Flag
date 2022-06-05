#pragma once
#include <vector>
#include "cgp/cgp.hpp"

using namespace cgp;


struct Particle {
	vec3 Position;
	vec3 Velocity;
	vec4 Color;
    int textureindex;
	float Life;
    Particle() : Position{ 0,0,0 }, Velocity{ 0.0f,0,-1.0f }, Color{ 0,0,0,0 }, Life(5.0f) 
    {
        float random = ((rand() % 100) - 50) / 10.0f;
        Position.y = random;
    }
};

class ParticleGenerator
{
public:
    ParticleGenerator(GLuint const shader,unsigned int amount);
    void Update(float dt, unsigned int newParticles, vec3 offset = vec3(0.0f, 0.0f,0.0f));
    void Draw(cgp::scene_environment_basic_camera_spherical_coords &environment);
private:
    // states
    std::vector<Particle> particles;
    unsigned int shaderprogram; 
    cgp::mesh_drawable drawable_quad;
    unsigned int amount;
    cgp::scene_environment_basic_camera_spherical_coords *environment;
    float TotalDuration = 4.0f;
    GLuint const texture_image_id= cgp::opengl_load_texture_image("assets/Explosion02_5x5.png",
        GL_REPEAT,
        GL_REPEAT);
    unsigned int VAO;
    void init();
    unsigned int firstUnusedParticle();
    void respawnParticle(Particle& particle, vec3 offset = vec3(0.0f, 0.0f,0.0f));
};

