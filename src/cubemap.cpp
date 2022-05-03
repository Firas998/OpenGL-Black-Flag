#include "cubemap.hpp"

#define STB_IMAGE_IMPLEMENTATION

#include "third_party/src/glad/include/glad/glad.hpp"
#include "third_party/src/stb/stb_image.h"
#include "helper_opengl.hpp"


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
    std::vector<std::string> faces = 
    {
        "assets/cubemap/right.jpg",
        "assets/cubemap/left.jpg",
        "assets/cubemap/top.jpg",
        "assets/cubemap/bottom.jpg",
        "assets/cubemap/front.jpg",
        "assets/cubemap/back.jpg"
    };
    this->cubemap = loadCubemap(faces);
    this->shader = opengl_load_shader("shaders/skybox/vert.glsl", "shaders/skybox/frag.glsl");
}

void Cubemap::draw() {
    glDepthMask(GL_FALSE);
    glUseProgram(this->shader);
    // ... set view and projection matrix
    GLuint vao = 0;
    // Create an empty VAO identifiant
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->cubemap);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthMask(GL_TRUE);
    glUseProgram(0);
}