/* Copyright (c) 2015-2017, ARM Limited and Contributors
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge,
 * to any person obtaining a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*
Original code: https://github.com/ARM-software/opengl-es-sdk-for-android/blob/master/samples/advanced_samples/FFTOceanWater/jni/ocean.cpp
MODIFIED BY LOUIS CAUBET
Draw ocean in this project using CGP lib.
*/

#include "ocean.hpp"

#include <complex>
#include <cstdio>
#include <cmath>
#include <random>
#include <iostream>

#include "third_party/FFTWater/fftwater.hpp"
#include "third_party/FFTWater/scattering.hpp"
#include "third_party/GLFFT/glfft.hpp"
#include "third_party/FFTWater/mesh.hpp"
#include "third_party/FFTWater/common.hpp"

#include <cgp/cgp.hpp>

using namespace std;
using namespace GLFFT;

// Number of samples in heightmap
#define SIZE_X 256
#define SIZE_Z 256

// The size in world space for a heightmap block.
#define DIST_X 200.0f
#define DIST_Z 200.0f

// The high-frequency normal map is sampled for much finer waves.
// Make this non-integer so it does not contribute to making the heightmap tiling more apparent.
#define NORMALMAP_FREQ_MOD 10.3f
#define AMPLITUDE 2.0f

// Stormy
#define WIND_SPEED_X +100.0f
#define WIND_SPEED_Z -50.0f

static FFTWater* water;
static Scattering* scatter;
static Mesh* mesh[2];

static GLuint prog_quad;
static GLuint prog_skydome;
static GLuint vao_quad;
static GLuint vbo_quad;

static vec3 cam_pos = vec3(0.0f, 15.0f, 0.0f);
static float cam_rot_y = -0.6f, cam_rot_x = -0.1f;
static vec3 cam_dir;

// Used for frustum culling in mesh.cpp.
static void compute_frustum(vec4* planes, const mat4& mvp)
{
    // Frustum planes are in world space.
    mat4 inv = mat_inverse(mvp);
    // Get world-space coordinates for clip-space bounds.
    vec4 lbn = inv * vec4(-1, -1, -1, 1);
    vec4 ltn = inv * vec4(-1, 1, -1, 1);
    vec4 lbf = inv * vec4(-1, -1, 1, 1);
    vec4 rbn = inv * vec4(1, -1, -1, 1);
    vec4 rtn = inv * vec4(1, 1, -1, 1);
    vec4 rbf = inv * vec4(1, -1, 1, 1);
    vec4 rtf = inv * vec4(1, 1, 1, 1);

    // Divide by w.
    vec3 lbn_pos = vec_project(lbn);
    vec3 ltn_pos = vec_project(ltn);
    vec3 lbf_pos = vec_project(lbf);
    vec3 rbn_pos = vec_project(rbn);
    vec3 rtn_pos = vec_project(rtn);
    vec3 rbf_pos = vec_project(rbf);
    vec3 rtf_pos = vec_project(rtf);

    // Get plane normals for all sides of frustum.
    vec3 left_normal = vec_normalize(vec_cross(lbf_pos - lbn_pos, ltn_pos - lbn_pos));
    vec3 right_normal = vec_normalize(vec_cross(rtn_pos - rbn_pos, rbf_pos - rbn_pos));
    vec3 top_normal = vec_normalize(vec_cross(ltn_pos - rtn_pos, rtf_pos - rtn_pos));
    vec3 bottom_normal = vec_normalize(vec_cross(rbf_pos - rbn_pos, lbn_pos - rbn_pos));
    vec3 near_normal = vec_normalize(vec_cross(ltn_pos - lbn_pos, rbn_pos - lbn_pos));
    vec3 far_normal = vec_normalize(vec_cross(rtf_pos - rbf_pos, lbf_pos - rbf_pos));

    // Plane equations compactly represent a plane in 3D space.
    // We want a way to compute the distance to the plane while preserving the sign to know which side we're on.
    // Let:
    //    O: an arbitrary point on the plane
    //    N: the normal vector for the plane, pointing in the direction
    //       we want to be "positive".
    //    X: Position we want to check.
    //
    // Distance D to the plane can now be expressed as a simple operation:
    // D = dot((X - O), N) = dot(X, N) - dot(O, N)
    //
    // We can reduce this to one dot product by assuming that X is four-dimensional (4th component = 1.0).
    // The normal can be extended to four dimensions as well:
    // X' = vec4(X, 1.0)
    // N' = vec4(N, -dot(O, N))
    //
    // The expression now reduces to: D = dot(X', N')
    planes[0] = vec4(near_normal, -vec_dot(near_normal, lbn_pos));   // Near
    planes[1] = vec4(far_normal, -vec_dot(far_normal, lbf_pos));    // Far
    planes[2] = vec4(left_normal, -vec_dot(left_normal, lbn_pos));   // Left
    planes[3] = vec4(right_normal, -vec_dot(right_normal, rbn_pos));  // Right
    planes[4] = vec4(top_normal, -vec_dot(top_normal, ltn_pos));    // Top
    planes[5] = vec4(bottom_normal, -vec_dot(bottom_normal, lbn_pos)); // Bottom
}

// Create a VAO for a simple quad.
static void init_vao()
{
    opengl_check(glGenVertexArrays(1, &vao_quad));
    opengl_check(glBindVertexArray(vao_quad));
    opengl_check(glGenBuffers(1, &vbo_quad));
    opengl_check(glBindBuffer(GL_ARRAY_BUFFER, vbo_quad));

    static const int8_t quad[] = {
        -1, -1,
         1, -1,
        -1,  1,
         1,  1,
    };
    opengl_check(glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW));
    opengl_check(glEnableVertexAttribArray(0));
    opengl_check(glVertexAttribPointer(0, 2, GL_BYTE, GL_FALSE, 0, 0));
    opengl_check(glBindVertexArray(0));
}

void Ocean::app_init()
{

    common_set_basedir("shaders/water");
    init_vao();

    mesh[0] = new MorphedGeoMipMapMesh;
    if (common_has_extension("GL_EXT_tessellation_shader"))
    {
        mesh[1] = new TessellatedMesh;
    }

    std::cout << "MorphedGeoMipMapMesh done." << std::endl;

    water = new FFTWater(AMPLITUDE, vec2(WIND_SPEED_X, WIND_SPEED_Z), uvec2(SIZE_X, SIZE_Z), vec2(DIST_X, DIST_Z), vec2(NORMALMAP_FREQ_MOD));
    prog_quad = common_compile_shader_from_file("quad.vs", "quad.fs");
    prog_skydome = common_compile_shader_from_file("skydome.vs", "skydome.fs");

    // Generate a simple skydome.
    scatter = new Scattering;
    vec3 light_dir = vec_normalize(vec3(100.0f, 20.0f, 100.0f));
    scatter->generate(64, light_dir);

    
}

// Move the camera while looking at the sun for a nice scene.
void Ocean::app_update(float delta_time, cgp::scene_environment_basic_camera_spherical_coords environment)
{
    vec3 base_cam_dir = vec3(0.0f, 0.0f, -1.0f);

    const float factor = 8.0f;

    cgp::vec3 cam_pos_cgp = environment.camera.position();
    cam_pos = vec3(cam_pos_cgp.y* factor, cam_pos_cgp.z* factor, cam_pos_cgp.x * factor);
    cgp::vec3 cam_dir_cgp = environment.camera.front();
    cam_dir = vec3(cam_dir_cgp.y * factor, cam_dir_cgp.z* factor, cam_dir_cgp.x * factor);
}

void Ocean::app_render(unsigned width, unsigned height, float total_time, unsigned mesh_index)
{
    // Update the water textures with FFT.
    water->update(total_time);

    auto proj = mat_perspective_fov(60.0f, float(width) / height, 1.0f, 2000.0f);
    auto view = mat_look_at(cam_pos, cam_pos + cam_dir, vec3(0.0f, 1.0f, 0.0f));
    auto view_no_translate = mat_look_at(vec3(0.0f), cam_dir, vec3(0.0f, 1.0f, 0.0f));

    // Clear
    opengl_check(glViewport(0, 0, width, height));
    opengl_check(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
    opengl_check(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

    opengl_check(glEnable(GL_DEPTH_TEST));
    opengl_check(glDepthFunc(GL_LEQUAL));
    opengl_check(glEnable(GL_CULL_FACE));

    // Render water
    Mesh::RenderInfo info;
    info.mvp = proj * view;
    info.fft_size = uvec2(SIZE_X, SIZE_Z);
    info.tile_extent = vec2(DIST_X, DIST_Z);
    info.normal_scale = vec2(NORMALMAP_FREQ_MOD);
    info.displacement_downsample = water->get_displacement_downsample();
    info.cam_pos = cam_pos;
    info.height_displacement = water->get_height_displacement();
    info.gradient_jacobian = water->get_gradient_jacobian();
    info.normal = water->get_normal();
    info.skydome = scatter->get_texture();
    info.vp_width = width;
    info.vp_height = height;
    compute_frustum(info.frustum, info.mvp);

    mesh[mesh_index]->render(info);

    // Render skydome
    opengl_check(glUseProgram(prog_skydome));
    opengl_check(glActiveTexture(GL_TEXTURE0));
    opengl_check(glBindTexture(GL_TEXTURE_CUBE_MAP, scatter->get_texture()));
    opengl_check(glUniformMatrix4fv(0, 1, GL_FALSE, value_ptr(mat_inverse(proj * view_no_translate))));
    opengl_check(glBindVertexArray(vao_quad));
    opengl_check(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));

    opengl_check(glBindVertexArray(0));

    glUseProgram(0);
    glDisable(GL_CULL_FACE);

}

void Ocean::app_term()
{
    opengl_check(glDeleteBuffers(1, &vbo_quad));
    opengl_check(glDeleteVertexArrays(1, &vao_quad));

    delete water;
    water = nullptr;
    delete scatter;
    scatter = nullptr;
    delete mesh[0];
    delete mesh[1];
    mesh[0] = nullptr;
    mesh[1] = nullptr;
}