#include "Smokeparticle.hpp"

using namespace cgp;

const std::vector<genparams> ParticleGenerator::allparams({ { 0.25f,0.01f,0.1f,1.0f,5,1.0f },{ 0.25f,0.1f,0.1f,1.0f,5,1.0f } });

ParticleGenerator::ParticleGenerator(GLuint const shaderprogram, GLuint const texture_id, unsigned int amount,vec3 generatorposition,vec3 Velocity,int params)
    : shaderprogram(shaderprogram), texture_image_id(texture_id)
{
    Life=allparams[params].Life;
    TotalDuration=allparams[params].TotalDuration;
    lowerradius= allparams[params].lowerradius;
    higherradius= allparams[params].higherradius;
    scale= allparams[params].scale;
    texturelimit= allparams[params].texturelimit;
    this->init(generatorposition,Velocity,amount);
}

void ParticleGenerator::Update(float dt, unsigned int newParticles, vec3 generatorposition,vec3 Velocity)
{
    
    // Delete dead particles
    if (this->particles.size() > 0) {
        while (this->particles.front().Life < 0) {
            this->particles.pop_front();
        }
    }
    
    // add new particles
    for (unsigned int i = 0; i < newParticles; ++i) {
        this->particles.push_back(Particle(generatorposition, Velocity, TotalDuration,lowerradius,higherradius,scale));
    }
    
    // update all particles
    for (int i = 0; i < this->particles.size(); i++) {
        Particle* p = &(this->particles[i]);
        p->Life = p->Life - dt;
        if (p->Life > 0.0f)
        {	
            // particle is alive, thus update
            p->Position += p->Velocity * dt;
            p->textureindex = (int)((1 - ((1.0f * p->Life) / TotalDuration)) * texturelimit);

        }
    }
    
    
}

// render all particles
void ParticleGenerator::Draw(cgp::scene_environment_basic_camera_spherical_coords &environment,cgp::mesh_drawable& drawable)
{
    // use additive blending to give it a 'glow' effect
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(false);
    for (int i = 0; i < this->particles.size(); i++)
    {
        Particle* particle = &(this->particles[i]);
        if (particle->Life > 0.0f)
        {   
            drawable.transform.translation = particle->Position;
            drawable.transform.rotation = rotation_transform::between_vector(cgp::vec3(0, 1, 0), environment.camera.front());
            glUseProgram(shaderprogram);
            cgp::vec3 index{ particle->textureindex % 5 , particle->textureindex / 5,0.0f };
            cgp::opengl_uniform(shaderprogram, "textureindex", index);
            glUseProgram(0);
            cgp::draw(drawable, environment);
        }
    }
    // don't forget to reset to default blending mode
    glDepthMask(true);
    glDisable(GL_BLEND);
}

void ParticleGenerator::init(vec3 generatorposition,vec3 Velocity,int amount)
{
    for (unsigned int i = 0; i < amount; ++i)
        this->particles.push_back(Particle(generatorposition,Velocity,TotalDuration,lowerradius,higherradius,scale));
    

}

void ParticleGenerator::respawnParticle(Particle& particle, vec3 generatorposition,vec3 Velocity)
{
    Particle particule { generatorposition,Velocity, TotalDuration,lowerradius,higherradius,scale };
    particle = particule;
}