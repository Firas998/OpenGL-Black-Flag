#include "Smokeparticle.hpp"

using namespace cgp;

ParticleGenerator::ParticleGenerator(GLuint const shaderprogram, unsigned int amount,vec3 generatorposition,vec3 Velocity)
    : amount(amount),shaderprogram(shaderprogram)
{
    this->init(generatorposition,Velocity);
}

void ParticleGenerator::Update(float dt, unsigned int newParticles, vec3 generatorposition,vec3 Velocity)
{

    // Delete dead particles
    while (this->particles.front().Life < 0) {
        this->particles.pop();
    }

    // If there are too many particles, delete some
    while (this->particles.size() > 2000 - newParticles) {
        this->particles.pop();
    }

    // add new particles
    for (unsigned int i = 0; i < newParticles; ++i)
    {
        this->particles.push(Particle(generatorposition, Velocity, 1.0f));
    }
    
    
    // update all particles
    for (auto it = this->particles._Get_container().begin(); it != this->particles._Get_container().end(); ++it) {
        Particle p = *it;
        if (p.Life > 0.0f)
        {	
            // particle is alive, thus update
            p.Position += p.Velocity * dt;
            p.textureindex = (int)((1 - ((1.0f * p.Life) / TotalDuration)) * 15);

        }
    }
}

// render all particles
void ParticleGenerator::Draw(cgp::scene_environment_basic_camera_spherical_coords &environment)
{
    // use additive blending to give it a 'glow' effect
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(false);
    for (Particle const& particle : this->particles._Get_container())
    {
        if (particle.Life > 0.0f)
        {   
            drawable_quad.transform.translation = particle.Position;
            drawable_quad.transform.rotation = rotation_transform::between_vector(cgp::vec3(0, 1, 0), environment.camera.front());
            glUseProgram(shaderprogram);
            cgp::vec3 index{ particle.textureindex % 5 , particle.textureindex / 5,0.0f };
            cgp::opengl_uniform(shaderprogram, "textureindex", index);
            glUseProgram(0);
            cgp::draw(drawable_quad, environment);
        }
    }
    // don't forget to reset to default blending mode
    glDepthMask(true);
    glDisable(GL_BLEND);
}

void ParticleGenerator::init(vec3 generatorposition,vec3 Velocity)
{
    drawable_quad.initialize(mesh_primitive_quadrangle({ -1, 0, -1 }, { 1,0,-1 }, { 1,0,1 }, { -1,0,1 }), "Particle_mesh");
    drawable_quad.shader = shaderprogram;
    drawable_quad.texture = texture_image_id;
    for (unsigned int i = 0; i < this->amount; ++i)
        this->particles.push(Particle(generatorposition,Velocity,TotalDuration));
}

void ParticleGenerator::respawnParticle(Particle& particle, vec3 generatorposition,vec3 Velocity)
{
    Particle particule { generatorposition,Velocity, TotalDuration };
    particle = particule;
}