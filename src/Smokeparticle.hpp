#pragma once
#include <vector>
#include "cgp/cgp.hpp"

using namespace cgp;


struct Particle {
	vec2 Position;
	vec2 Velocity;
	vec4 Color;
    int textureindex;
	float Life;
    Particle() : Position{ 0,0 }, Velocity{ 0,0 }, Color{ 1,1,1,1 }, Life(0.0f) { }
};

class ParticleGenerator
{
public:
    ParticleGenerator(GLuint const shader, unsigned int amount);
    void Update(float dt, unsigned int newParticles, vec2 offset = vec2(0.0f, 0.0f));
    void Draw();
private:
    // state
    std::vector<Particle> particles;
    unsigned int shaderprogram; 
    unsigned int amount;
    float TotalDuration = 4.0f;
    GLuint const texture_image_id= cgp::opengl_load_texture_image("../assets/Explosion02_5x5",
        GL_REPEAT,
        GL_REPEAT);
    unsigned int VAO;
    void init();
    unsigned int firstUnusedParticle();
    void respawnParticle(Particle& particle, vec2 offset = vec2(0.0f, 0.0f));
};

