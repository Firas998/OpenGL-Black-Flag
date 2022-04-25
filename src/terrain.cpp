
#include "terrain.hpp"


using namespace cgp;

// Evaluate 3D position of the terrain for any (u,v) \in [0,1]
float evaluate_terrain_height(float x, float y)
{
    vec2 p_0 = { 0, 0 };
    float h_0 = 2.0f;
    float sigma_0 = 3.0f;

    float d = norm(vec2(x, y) - p_0) / sigma_0;

    float z = h_0 * std::exp(-d * d);

    return z;
}

float evaluate_terrain_height(int n, float *h, vec2 *p, float *s, float x, float y) {
    float z = 0.0f;
    for (int i = 0; i < n; i++) {
        float d = norm(vec2(x, y) - p[i]) / s[i];
        z += h[i] * std::exp(-d * d);
    }
    return z;
}

mesh create_terrain_mesh(int N, float terrain_length, perlin_noise_parameters const& parameters)
{

    mesh terrain; // temporary terrain storage (CPU only)
    terrain.position.resize(N*N);
    terrain.uv.resize(N * N);

    vec2 p0 = { -10, -10 };
    vec2 p1 = { 5, 5 };
    vec2 p2 = { -3, 4 };
    vec2 p3 = { 6,4 };
    vec2 p[4] = { p0,p1,p2,p3 };

    float h[4] = { 3.f, -1.5f, 1.f, 2.f };
    float s[4] = { 10.f, 3.f, 4.f, 4.f };

    // Fill terrain geometry
    for(int ku=0; ku<N; ++ku)
    {
        for(int kv=0; kv<N; ++kv)
        {
            // Compute local parametric coordinates (u,v) \in [0,1]
            float u = ku/(N-1.0f);
            float v = kv/(N-1.0f);

            // Compute the real coordinates (x,y) of the terrain 
            float x = (u - 0.5f) * terrain_length;
            float y = (v - 0.5f) * terrain_length;

            // Compute the surface height function at the given sampled coordinate
            // float z = evaluate_terrain_height(x,y);

            // Add noise
            // Compute the Perlin noise
            float const noise = noise_perlin({ u, v }, parameters.octave, parameters.persistency, parameters.frequency_gain);

            // Custom terrain height
            float z = evaluate_terrain_height(4, h, p, s, x, y);

            // Store vertex coordinates
            terrain.position[kv+N*ku] = {x,y,z * noise };
            terrain.uv[kv + N * ku] = { u*10,v*10 };
        }
    }

    // Generate triangle organization
    //  Parametric surface with uniform grid sampling: generate 2 triangles for each grid cell
    for(int ku=0; ku<N-1; ++ku)
    {
        for(int kv=0; kv<N-1; ++kv)
        {
            unsigned int idx = kv + N*ku; // current vertex offset

            uint3 triangle_1 = {idx, idx+1+N, idx+1};
            uint3 triangle_2 = {idx, idx+N, idx+1+N};

            terrain.connectivity.push_back(triangle_1);
            terrain.connectivity.push_back(triangle_2);
        }
    }

    // need to call this function to fill the other buffer with default values (normal, color, etc)
	terrain.fill_empty_field(); 

    return terrain;
}

std::vector<vec3> generate_positions_on_terrain(int N, float terrain_length) {

    vec2 p0 = { -10, -10 };
    vec2 p1 = { 5, 5 };
    vec2 p2 = { -3, 4 };
    vec2 p3 = { 6,4 };
    vec2 p[4] = { p0,p1,p2,p3 };

    float h[4] = { 3.f, -1.5f, 1.f, 2.f };
    float s[4] = { 10.f, 3.f, 4.f, 4.f };

    std::vector<vec3> result;

    for (int i = 0; i < N; i++) {
        float x = rand_interval(-terrain_length/2., terrain_length/2.);
        float y = rand_interval(-terrain_length / 2., terrain_length / 2.);
        bool is_valid = true;
        for (int j = 0; j < i; j++) {
            vec3 t = result[j];
            if (pow(t.x - x, 2) + pow(t.y - y, 2) < 0.8f) {
                is_valid = false;
                break;
            }
        }
        if (!is_valid) {
            i--;
            continue;
        }
        float z = evaluate_terrain_height(4, h, p, s, x, y);
        result.push_back({ x,y,z });
    }

    return result;
}