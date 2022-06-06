
#include "island.hpp"

using namespace cgp;

float perlin_noise_parameters::height(float noise) {
    return interpolation(noise, x_axis, y_axis);
}

float island::evaluate(float height) {
    float a = 3;
    float b = 2.2;
    return std::pow(height, a) / (std::pow(height, a) + std::pow(b - b * height, a));
}

mesh island::create_terrain_mesh()
{

	Terraintype WaterDeep = { "water_deep",0.2f,cgp::vec3(0.007f,0.294f,0.525f) };
    Terraintype WaterShallow = { "water_shallow",0.5f,cgp::vec3(0.7764f,0.9f,0.925f) };
    Terraintype Sand = { "sand" ,1.2f,cgp::vec3(0.761f,0.968f,0.5f) };
    Terraintype Grass = { "grass",1.6f,cgp::vec3(0.219f,0.84f,0.141f) };
    Terraintype Rock2 = { "rock2",1.9f,cgp::vec3(0.52f, 0.338f, 0.325f) };
    Terraintype Snow = { "snow",2.0f,cgp::vec3(1, 1, 1) };
	regions.push_back(WaterDeep);
	regions.push_back(WaterShallow);
    regions.push_back(Sand);
    regions.push_back(Grass);
    regions.push_back(Rock2);
    regions.push_back(Snow);
	int const terrain_sample = 100;
    vec3 offset = { 250,250,0 };
	mesh terrain = mesh_primitive_grid(offset+vec3(- 100, -100, 0), offset + vec3(100, -100, 0), offset + vec3(100, 100, 0 ), offset +vec3( - 100, 100, 0 ), terrain_sample, terrain_sample);
    
    
    int const N = std::sqrt(terrain.position.size());
    for (int ku = 0; ku < N; ++ku) {
        for (int kv = 0; kv < N; ++kv) {

            // Compute local parametric coordinates (u,v) \in [0,1]
            float u = ku / ((N - 1.0f));
            float v = kv / ((N - 1.0f));
            u = u * 2 - 1;
            v = v * 2 - 1;
            falloffmap.push_back(evaluate(std::max(std::abs(u), std::abs(v))));
        }
    }
    return terrain;
}

void island::update_terrain(mesh& terrain, mesh_drawable& terrain_visual, perlin_noise_parameters & parameters)
{
	// Number of samples in each direction (assuming a square grid)
	int const N = std::sqrt(terrain.position.size());

	// Recompute the new vertices
	for (int ku = 0; ku < N; ++ku) {
		for (int kv = 0; kv < N; ++kv) {

			// Compute local parametric coordinates (u,v) \in [0,1]
			const float u = ku / ((N - 1.0f)*parameters.noisescale);
			const float v = kv / ((N - 1.0f)*parameters.noisescale);

			int const idx = ku * N + kv;

			// Compute the Perlin noise
            float noise = noise_perlin({ u, v }, parameters.octave, parameters.persistency, parameters.frequency_gain)-falloffmap[idx];
            noise = parameters.height(noise);
            if (noise < 0)
                noise = 0;

			// use the noise as height value
			terrain.position[idx].z = parameters.terrain_height*noise;

			// use also the noise as color value
			
			for (int i = 0; i < regions.size(); i++) {
				if (noise<= regions[i].heightvalue) {
					terrain.color[idx] = regions[i].color;
					break;
				}
			}
		}
	}

	// Update the normal of the mesh structure
	terrain.compute_normal();

	// Update step: Allows to update a mesh_drawable without creating a new one
	terrain_visual.update_position(terrain.position);
	terrain_visual.update_normal(terrain.normal);
	terrain_visual.update_color(terrain.color);

}

float cardinal_spline_interpolation(float t, float t0, float t1, float t2, float t3, float const& p0, float const& p1, float const& p2, float const& p3, float K);

int find_index_of_interval(float t, buffer<float> const& intervals);


float interpolation(float t,buffer<float> x_axis,buffer<float> y_axis)
{
    
    int idx = 1;


    float t0 = x_axis[idx-1]; // = t_i
    float t1= x_axis[idx]; // = t_{i+1}
    float t2 = x_axis[idx + 1];
    float t3 = x_axis[idx + 2];


    float const& p0 = y_axis[idx-1]; // = p_i
    float const& p1 = y_axis[idx]; // = p_{i+1}
    float const& p2 = y_axis[idx+1]; // = p_{i+1}
    float const& p3 = y_axis[idx+2]; // = p_{i+1}


    float p = cardinal_spline_interpolation(t,t0,t1,t2,t3,p0,p1,p2,p3,1);
    return p;
}



float cardinal_spline_interpolation(float t, float t0, float t1, float t2, float t3, float const& p0, float const& p1, float const& p2, float const& p3, float K)
{
    float const di = (p2 - p0) / (t2 - t0);
    float const di1 = (p3 - p1) / (t3 - t1);
    float const s = (t - t1) / (t2 - t1);
    float const p = p1 * (2 * pow(s, 3) - 3 * pow(s, 2) + 1) + di * (pow(s, 3) - 2 * pow(s, 2) + s) + di1 * (pow(s, 3) - pow(s, 2)) + p2 * (3 * pow(s, 2) - 2 * pow(s, 3));

    return p;
}


int find_index_of_interval(float t, buffer<float> const& intervals)
{
    int const N = intervals.size();
    bool error = false;

    if (intervals.size() < 2) {
        std::cout<<"Error: Intervals should have at least two values; current size="<<intervals.size()<<std::endl;
        error = true;
    }
    if (N>0 && t < intervals[0]) {
        std::cout<<"Error: current time t is smaller than the first time of the interval"<<std::endl;
        error = true;
    }
    if(N>0 && t > intervals[N-1]) {
        std::cout<<"Error: current time t is greater than the last time of the interval"<<std::endl;
        error = true;
    }
    if (error == true) {
        std::string const error_str = "Error trying to find interval for t="+str(t)+" within values: ["+str(intervals)+"]";
        error_cgp( error_str );
    }


    int k=0;
    while( intervals[k+1]<t )
        ++k;
    return k;
}

