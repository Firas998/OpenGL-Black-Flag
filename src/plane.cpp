#include "plane.hpp"
#include <iostream> 

using namespace cgp;

mesh createplaneout(int N, float terrain_length,double z)
{

    mesh terrain; // temporary terrain storage (CPU only)
    terrain.position.resize(N * N);

    // Fill terrain geometry
    for (int ku = 0; ku < N; ++ku)
    {
        for (int kv = 0; kv < N; ++kv)
        {
            // Compute local parametric coordinates (u,v) \in [0,1]
            float u = ku / (N - 1.0f);
            float v = kv / (N - 1.0f);

            // Compute the real coordinates (x,y) of the terrain 
            float x = (u - 0.5f) * terrain_length;
            float y = (v - 0.5f) * terrain_length;

            // Store vertex coordinates
            terrain.position[kv + N * ku] = { x,y,z };

            terrain.uv.resize(N * N);

            terrain.uv[kv + N * ku] = { 10 * u,10 * v };


            // use the noise as height value

        }
    }

    // Generate triangle organization
    //  Parametric surface with uniform grid sampling: generate 2 triangles for each grid cell
    for (int ku = 0; ku < N - 1; ++ku)
    {
        for (int kv = 0; kv < N - 1; ++kv)
        {
            unsigned int idx = kv + N * ku; // current vertex offset

            uint3 triangle_1 = { idx, idx + 1 + N, idx + 1 };
            uint3 triangle_2 = { idx, idx + N, idx + 1 + N };

            terrain.connectivity.push_back(triangle_1);
            terrain.connectivity.push_back(triangle_2);
        }
    }

    // need to call this function to fill the other buffer with default values (normal, color, etc)
    terrain.fill_empty_field();

    return terrain;
}

void planestruct::createplane(int sizeinput) {
	size = sizeinput;
	mesh m = createplaneout(size, size*10, 0);
	plane_drawable.initialize(m, "PLANE");
}

void planestruct::displayplane(cgp::scene_environment_basic_camera_spherical_coords environment)
{
	draw(plane_drawable, environment);
}