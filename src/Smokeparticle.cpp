#include "Smokeparticle.hpp"

using namespace cgp;

ParticleGenerator::ParticleGenerator(GLuint const shaderprogram, unsigned int amount,vec3 generatorposition,vec3 Velocity)
    : amount(amount),shaderprogram(shaderprogram)
{
    this->init(generatorposition,Velocity);
}

void ParticleGenerator::Update(float dt, unsigned int newParticles, vec3 generatorposition,vec3 Velocity)
{
    // add new particles 
    
    
    for (unsigned int i = 0; i < newParticles; ++i)
    {
        /*
        int unusedParticle = this->firstUnusedParticle();
        if (unusedParticle == 0) {
            this->particles.push_back(Particle(generatorposition, Velocity));
        }
        else {
            this->respawnParticle(this->particles[unusedParticle], generatorposition, Velocity);
        }
        */
    }
    
    
    // update all particles
    for (unsigned int i = 0; i < this->amount; ++i)
    {
        Particle& p = this->particles[i];
        p.Life -= dt; // reduce life
        if (p.Life > 0.0f)
        {	// particle is alive, thus update
            p.Position += p.Velocity * dt;
            p.textureindex = (int)((1-((1.0f*p.Life) / TotalDuration)) * 15);
            
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
    for (Particle &particle : this->particles)
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
        this->particles.push_back(Particle(generatorposition,Velocity,TotalDuration));
}

// stores the index of the last particle used (for quick access to next dead particle)

unsigned int ParticleGenerator::firstUnusedParticle()
{
    // first search from last used particle, this will usually return almost instantly
    for (unsigned int i = lastUsedParticle; i < this->amount; ++i) {
        if (this->particles[i].Life <= 0.0f) {
            lastUsedParticle = i;
            return i;
        }
    }
    // otherwise, do a linear search
    for (unsigned int i = 0; i < lastUsedParticle; ++i) {
        if (this->particles[i].Life <= 0.0f) {
            lastUsedParticle = i;
            return i;
        }
    }
    // all particles are taken, override the first one (note that if it repeatedly hits this case, more particles should be reserved)
    lastUsedParticle = 200000;
    return 0;
}

void ParticleGenerator::respawnParticle(Particle& particle, vec3 generatorposition,vec3 Velocity)
{
    Particle particule { generatorposition,Velocity, TotalDuration };
    particle = particule;
}