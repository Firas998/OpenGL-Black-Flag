#pragma once
#include <vector>
#include "cgp/cgp.hpp"
#include <queue>

using namespace cgp;


//Les paramètres du système de particule , on utilisera 2 instances distinctes de cet objet ( 1 pour les flash de museau , 1 pour la trainée de fumée ) 
struct genparams {
    float TotalDuration;
    float lowerradius;
    float higherradius;
    float scale;
    float texturelimit;
    float Life;
    int dimsize;
};

//Objet pour les particules, des systèmes de particules
class Particle {
public:
	vec3 Position;
	vec3 Velocity;
    int textureindex;
	float Life;

    //Les paramèetres du cone de vitesse
    float lowerradius;
    float higherradius;

    float scale;// à appliquer à la vitesse

    Particle(vec3 generatorposition, vec3 normal, float duration,float lowerradius, float higherradius,float scaler) : lowerradius{ lowerradius }, higherradius{ higherradius }, Life(duration) ,scale{scaler}
    {

        //On crée un cone de vitesse autour du vecteur normal, et on prend une position dans le plan normal à ce vecteur
        float random1 = (double)rand() / RAND_MAX;
        float random2 = (double)rand() / RAND_MAX;
        double theta = 2 * 3.14159265358979323846264;
        Position = vec3(random2 * lowerradius * cos(theta*random1), random2 * lowerradius * sin(theta*random1), 0);
        Position= generatorposition + rotation_transform::between_vector(cgp::vec3(0, 0, 1), normal) * Position;

        vec3 Position2 = vec3(random2 * higherradius * cos(theta * random1), random2 * higherradius * sin(theta * random1), 0);
        Position2 = generatorposition + rotation_transform::between_vector(cgp::vec3(0, 0, 1), normal) * Position2;
        Velocity = scale*(Position2-Position + vec3(0, 0, 1));
        Velocity= rotation_transform::between_vector(cgp::vec3(0, 0, 1), normal) *Velocity;
    }
};

//Classe qui gère un système entier de particules
class ParticleGenerator
{
public:
    ParticleGenerator(GLuint const shader, GLuint const texture_id, unsigned int amount,vec3 generatorposition, vec3 Velocity,int params);
    void Update(float dt, unsigned int newParticles, vec3 generatorposition,vec3 Velocity);
    void Draw(cgp::scene_environment_basic_camera_spherical_coords &environment,cgp::mesh_drawable& drawable);
    float Life;
private:
    float TotalDuration;
    float lowerradius;
    float higherradius;
    float scale;
    float texturelimit;
    int dimsize;
    static const std::vector<genparams> allparams; 
    GLuint const texture_image_id;
    unsigned int shaderprogram;
    std::deque<Particle> particles;
    cgp::mesh_drawable drawable_quad;
    void init(vec3 generatorposition,vec3 Velocity,int amount);
    void respawnParticle(Particle& particle, vec3 generatorposition, vec3 Velocity);
};



