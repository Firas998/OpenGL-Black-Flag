#include "tree.hpp"

using namespace cgp;


mesh create_cylinder_mesh(float radius, float height)
{
    mesh m; 
    // To do: fill this mesh ...
    // ...
    // To add a position: 
    //   m.position.push_back(vec3{x,y,z})
    // Or in pre-allocating the buffer:
    //   m.position.resize(maximalSize);
    //   m.position[index] = vec3{x,y,z}; (with 0<= index < maximalSize)
    // 
    // Similar with the triangle connectivity:
    //  m.connectivity.push_back(uint3{index_1, index_2, index_3});

    // Number of points on a circle
    const int N = 10;

    // Construct a circle of radius radius
    vec2 circle[N];
    float theta = 0;
    for (int i=0; i<N; i++) {
        circle[i] = { radius * cos(theta), radius * sin(theta) };
        theta += 2 * Pi / N;
    }

    // Place the bottom circle on the mesh
    m.position.resize(2 * N);
    for (int i = 0; i < N; i ++) {
        m.position[2 * i] = { circle[i].x, circle[i].y, 0 };
    }

    // Place the upper circle on the mesh
    for (int i = 0; i < N; i++) {
        m.position[2 * i + 1] = { circle[i].x, circle[i].y, height };
    }

    // The triangles are all possibilities between i,i+1,i+2
    for (int i = 0; i < 2 * N-2; i++) {
        if(i%2 == 0) m.connectivity.push_back(uint3{ i,i + 1,i + 2 });
        else m.connectivity.push_back(uint3{ i + 1,i, i + 2 });
    }
    m.connectivity.push_back(uint3{ 2*N - 2,2*N - 1,0 });
    m.connectivity.push_back(uint3{ 0, 2*N - 1,1 });


    // Need to call fill_empty_field() before returning the mesh 
    //  this function fill all empty buffer with default values (ex. normals, colors, etc).
    m.fill_empty_field();

    return m;
}

mesh create_cone_mesh(float radius, float height, float z_offset)
{
    mesh m; 
    // Number of points on a circle
    const int N = 10;

    m.position.resize(N+2);

    // Construct a circle of radius radius
    float theta = 0;
    for (int i = 0; i < N; i++) {
        m.position[i+2] = {radius * cos(theta), radius * sin(theta), z_offset};
        theta += 2 * Pi / N;
    }

    m.position[0] = { 0,0,z_offset + height };
    m.position[1] = { 0,0, z_offset };

    for (int i = 0; i < N + 1; i++) {
        m.connectivity.push_back(uint3{ i,i + 1,0 });
        m.connectivity.push_back(uint3{ i,i + 1,1 });
    }

    m.connectivity.push_back(uint3{ N+1,2,0 });
    m.connectivity.push_back(uint3{ N+1,2,1 });



    m.fill_empty_field();
    return m;
}

mesh create_tree()
{
    float h = 0.7f; // trunk height
    float r = 0.1f; // trunk radius

    // Create a brown trunk
    mesh trunk = create_cylinder_mesh(r, h);
    trunk.color.fill({ 0.4f, 0.3f, 0.3f });


    // Create a green foliage from 3 cones
    mesh foliage = create_cone_mesh(4 * r, 6 * r, 0.0f);      // base-cone
    foliage.push_back(create_cone_mesh(4 * r, 6 * r, 2 * r));   // middle-cone
    foliage.push_back(create_cone_mesh(4 * r, 6 * r, 4 * r));   // top-cone
    foliage.position += vec3(0, 0, h);                 // place foliage at the top of the trunk
    foliage.color.fill({ 0.4f, 0.6f, 0.3f });

    // The tree is composted of the trunk and the foliage
    mesh tree = trunk;
    tree.push_back(foliage);

    return tree;
}


