#include "interpolation.hpp"
#include <cmath>


using namespace cgp;

//float cardinal_spline_interpolation(float t, float t0, float t1, float t2, float t3, float const& p0, float const& p1, float const& p2, float const& p3, float K);

//int find_index_of_interval(float t, buffer<float> const& intervals);


float interpolation(float t)
{
    /*
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

    */
    

    return 0.5f;
}


/*
float cardinal_spline_interpolation(float t, float t0, float t1, float t2, float t3, float const& p0, float const& p1, float const& p2, float const& p3, float K)
{
    float const di = (p2 - p0) / (t2 - t0);
    float const di1 = (p3 - p1) / (t3 - t1);
    float const s = (t - t1) / (t2 - t1);
    float const p = p1 * (2 * pow(s, 3) - 3 * pow(s, 2) + 1) + di * (pow(s, 3) - 2 * pow(s, 2) + s) + di1 * (pow(s, 3) - pow(s, 2)) + p2 * (3 * pow(s, 2) - 2 * pow(s, 3));

    return p;
}
*/

/*
* int find_index_of_interval(float t, buffer<float> const& intervals)
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
*/