#include "cubemap.hpp"

#define STB_IMAGE_IMPLEMENTATION

#include "third_party/src/glad/include/glad/glad.hpp"
#include "third_party/src/stb/stb_image.h"

using namespace cgp;


unsigned int Cubemap::loadCubemap(std::vector<std::string> faces){
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

void Cubemap::initialize() {
    const float size = 200;
    /*std::vector<std::string> faces = 
    {
        "assets/cubemap/right.jpg",
        "assets/cubemap/left.jpg",
        "assets/cubemap/top.jpg",
        "assets/cubemap/bottom.jpg",
        "assets/cubemap/front.jpg",
        "assets/cubemap/back.jpg"
    };
    this->cubemap = loadCubemap(faces);*/
    this->shader = opengl_load_shader("shaders/skybox/vert.glsl", "shaders/skybox/frag.glsl");
    mesh skybox_top = mesh_primitive_quadrangle(
        { -size, -size, size },
        { -size, size, size },
        { size, size, size },
        { size, -size, size }
    );
    skybox_top.uv = {
        {0,0},
        {0,1},
        {1,1},
        {1,0}
    };
    
    skybox_top_drawable.initialize(skybox_top);
    skybox_top_drawable.texture = opengl_load_texture_image("assets/cubemap/top.jpg", GL_REPEAT, GL_REPEAT);
    
    mesh skybox_bottom = mesh_primitive_quadrangle(
        { -size, -size, -size },
        { -size, size, -size },
        { size, size, -size },
        { size, -size, -size }
    );
    skybox_bottom.uv = {
        {0,0},
        {0,1},
        {1,1},
        {1,0}
    };
    skybox_bottom_drawable.initialize(skybox_bottom);
    skybox_bottom_drawable.texture = opengl_load_texture_image("assets/cubemap/bottom.jpg", GL_REPEAT, GL_REPEAT);

    mesh skybox_left = mesh_primitive_quadrangle(
        { -size, size, -size },
        { -size, size, size },
        { size, size, size },
        { size, size, -size }
    );
    skybox_left.uv = {
        {0,0},
        {0,1},
        {1,1},
        {1,0}
    };
    skybox_left_drawable.initialize(skybox_left);
    skybox_left_drawable.texture = opengl_load_texture_image("assets/cubemap/left.jpg", GL_REPEAT, GL_REPEAT);

    mesh skybox_right = mesh_primitive_quadrangle(
        { -size, -size, -size },
        { -size, -size, size },
        { size, -size, size },
        { size, -size, -size }
    );
    skybox_right.uv = {
        {1,0},
        {1,1},
        {0,1},
        {0,0},
    };
    skybox_right_drawable.initialize(skybox_right);
    skybox_right_drawable.texture = opengl_load_texture_image("assets/cubemap/right.jpg", GL_REPEAT, GL_REPEAT);

    mesh skybox_front = mesh_primitive_quadrangle(
        { size, -size, -size },
        { size, -size, size },
        { size, size, size },
        { size, size, -size }
    );
    skybox_front.uv = {
        {1,0},
        {1,1},
        {0,1},
        {0,0},
    };
    skybox_front_drawable.initialize(skybox_front);
    skybox_front_drawable.texture = opengl_load_texture_image("assets/cubemap/front.jpg", GL_REPEAT, GL_REPEAT);

    mesh skybox_back = mesh_primitive_quadrangle(
        { -size, -size, -size },
        { -size, -size, size },
        { -size, size, size },
        { -size, size, -size }
    );
    skybox_back.uv = {
        {0,0},
        {0,1},
        {1,1},
        {1,0}
    };
    skybox_back_drawable.initialize(skybox_back);
    skybox_back_drawable.texture = opengl_load_texture_image("assets/cubemap/back.jpg", GL_REPEAT, GL_REPEAT);
}

void Cubemap::drawCubemap(scene_environment_basic_camera_spherical_coords environment) {
    glDepthMask(GL_FALSE);
    glUseProgram(this->shader);
    //// ... set view and projection matrix
    //GLuint vao = 0;
    //// Create an empty VAO identifiant
    //glGenVertexArrays(1, &vao);
    //glBindVertexArray(vao);
    //glBindTexture(GL_TEXTURE_CUBE_MAP, this->cubemap);
    //glDrawArrays(GL_TRIANGLES, 0, 36);
    
    cgp_draw_custom(skybox_top_drawable, environment);
    cgp_draw_custom(skybox_bottom_drawable, environment);
    cgp_draw_custom(skybox_left_drawable, environment);
    cgp_draw_custom(skybox_right_drawable, environment);
    cgp_draw_custom(skybox_front_drawable, environment);
    cgp_draw_custom(skybox_back_drawable, environment);

    glDepthMask(GL_TRUE);
    glUseProgram(0);

}

void Cubemap::cgp_draw_custom(mesh_drawable drawable, scene_environment_basic_camera_spherical_coords environment) {
    // Source: CGP source code
    // Replace GL_TEXTURE_2D with GL_TEXTURE_CUBEMAP

    if (drawable.number_triangles == 0) return;

    // Setup shader
    assert_cgp(drawable.shader != 0, "Try to draw mesh_drawable without shader [name:" + drawable.name + "]");
    assert_cgp(drawable.texture != 0, "Try to draw mesh_drawable without texture [name:" + drawable.name + "]");
    glUseProgram(drawable.shader); opengl_check;

    // Send uniforms for this shader
    opengl_uniform(drawable.shader, environment);
    opengl_uniform(drawable.shader, drawable.shading);
    opengl_uniform(drawable.shader, "model", drawable.model_matrix());

    // Set texture
    glActiveTexture(GL_TEXTURE0); opengl_check;
    glBindTexture(GL_TEXTURE_2D, drawable.texture); opengl_check;
    opengl_uniform(drawable.shader, "image_texture", 0);  opengl_check;

    // Call draw function
    assert_cgp(drawable.number_triangles > 0, "Try to draw mesh_drawable with 0 triangles [name:" + drawable.name + "]"); opengl_check;
    glBindVertexArray(drawable.vao);   opengl_check;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawable.vbo.at("index")); opengl_check;
    glDrawElements(GL_TRIANGLES, GLsizei(drawable.number_triangles * 3), GL_UNSIGNED_INT, nullptr); opengl_check;

    // Clean buffers
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}