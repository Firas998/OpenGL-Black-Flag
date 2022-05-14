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

#include "common.hpp"
#include "vector_math.h"
#include "mesh.hpp"
#include <limits.h>

#include <vector>
#include <stdexcept>
#include <algorithm>

#include <iostream>

using namespace std;

constexpr float TessellatedMesh::patch_size;
constexpr float TessellatedMesh::lod0_distance;
constexpr unsigned TessellatedMesh::blocks_x;
constexpr unsigned TessellatedMesh::blocks_z;

constexpr unsigned MorphedGeoMipMapMesh::patch_size;
constexpr unsigned MorphedGeoMipMapMesh::max_instances;
constexpr unsigned MorphedGeoMipMapMesh::lods;
constexpr float MorphedGeoMipMapMesh::lod0_distance;
constexpr unsigned MorphedGeoMipMapMesh::blocks_x;
constexpr unsigned MorphedGeoMipMapMesh::blocks_z;

bool BoundingSphere::test_frustum(const vec4* frustum) const
{
    for (unsigned i = 0; i < 6; i++)
        if (vec_dot(center, frustum[i]) < -radius)
            return false;
    return true;
}

Mesh::Mesh(const char* vs_shader, const char* fs_shader)
{
    prog = common_compile_shader_from_file(vs_shader, fs_shader);
    if (!prog)
    {
        throw runtime_error("Failed to compile shader.");
    }

    opengl_check(glGenVertexArrays(1, &vao));
    opengl_check(glGenBuffers(1, &vbo));
    opengl_check(glGenBuffers(1, &ibo));

    std::cout << "End of Mesh constructor" << std::endl;
}

Mesh::Mesh(const char* vs_shader, const char* tc_shader, const char* te_shader,
    const char* geom_shader, const char* fs_shader)
{
    prog = common_compile_shader_from_file(vs_shader, tc_shader, te_shader,
        geom_shader, fs_shader);
    if (!prog)
    {
        throw runtime_error("Failed to compile shader.");
    }

    opengl_check(glGenVertexArrays(1, &vao));
    opengl_check(glGenBuffers(1, &vbo));
    opengl_check(glGenBuffers(1, &ibo));
}

Mesh::~Mesh()
{
    if (prog)
    {
        opengl_check(glDeleteProgram(prog));
    }

    if (vao)
    {
        opengl_check(glDeleteVertexArrays(1, &vao));
    }

    if (vbo)
    {
        opengl_check(glDeleteBuffers(1, &vbo));
    }

    if (ibo)
    {
        opengl_check(glDeleteBuffers(1, &ibo));
    }
}

static void generate_block_indices(vector<GLushort>& ibo, int vertex_buffer_offset,
    int width, int height, int stride)
{
    const GLushort restart_index = 0xffff;

    // Stamp out triangle strips back and forth.
    int strips = height - 1;

    // After even indices in a strip, always step to next strip.
    // After odd indices in a strip, step back again and one to the right or left.
    // Which direction we take depends on which strip we're generating.
    // This creates a zig-zag pattern.
    for (int z = 0; z < strips; z++)
    {
        int step_even = stride;
        int step_odd = 1 - step_even;
        int pos = z * stride + vertex_buffer_offset;

        for (int x = 0; x < 2 * width - 1; x++)
        {
            ibo.push_back(pos);
            pos += (x & 1) ? step_odd : step_even;
        }

        ibo.push_back(pos); // Complete strip

        if (z + 1 < strips)
            ibo.push_back(restart_index); // Restart
    }
}

void Mesh::bind_textures(const RenderInfo& info)
{
    opengl_check(glActiveTexture(GL_TEXTURE0 + 0));
    opengl_check(glBindTexture(GL_TEXTURE_2D, info.height_displacement));
    opengl_check(glActiveTexture(GL_TEXTURE0 + 1));
    opengl_check(glBindTexture(GL_TEXTURE_2D, info.gradient_jacobian));
    opengl_check(glActiveTexture(GL_TEXTURE0 + 2));
    opengl_check(glBindTexture(GL_TEXTURE_2D, info.normal));
    opengl_check(glActiveTexture(GL_TEXTURE0 + 3));
    opengl_check(glBindTexture(GL_TEXTURE_CUBE_MAP, info.skydome));
}

MorphedGeoMipMapMesh::MorphedGeoMipMapMesh()
    : Mesh("water.vs", "water.fs")
{
    init();
}

static inline float lod_factor(float max_lod, float distance_mod, vec3 dist)
{
    float level = log2((vec_length(dist) + 0.0001f) * distance_mod);
    return clamp(level, 0.0f, max_lod);
}

void MorphedGeoMipMapMesh::calculate_lods(const RenderInfo& info)
{
    vec2 patch_size_mod = vec2(patch_size) * info.tile_extent / vec2(info.fft_size);

    vec2 scale = info.tile_extent / vec2(info.fft_size);
    ivec2 block_off = ivec2(vec_round(vec2(info.cam_pos.x, info.cam_pos.z) / patch_size_mod));
    block_off -= ivec2(blocks_x >> 1, blocks_z >> 1);
    vec2 block_offset = vec2(patch_size) * vec2(block_off);

    float distance_mod = 1.0f / ((info.vp_width / 1920.0f) * lod0_distance);
    vec3 cam_pos = info.cam_pos;

    // Compute LOD and visibility.
    for (auto& patch : patches)
    {
        const vec2 half_block = scale * vec2(0.5f * patch_size);

        vec2 newpos = scale * (patch.pos + block_offset) + half_block;
        vec3 dist = cam_pos - vec3(newpos.x, 0.0f, newpos.y);
        patch.lod = lod_factor(lods - 1.0f, distance_mod, dist);

        BoundingSphere bs(vec3(newpos.x, 0.0f, newpos.y), vec3(10.0f + half_block.x, 20.0f, 10.0f + half_block.y));
        patch.visible = bs.test_frustum(info.frustum);
    }

    for (unsigned z = 0; z < blocks_z; z++)
    {
        for (unsigned x = 0; x < blocks_x; x++)
        {
            float lod = patches[z * blocks_x + x].lod;
            lod_buffer[z * blocks_x + x] = lod;
        }
    }

    // Upload the LOD texture to a PBO first.
    opengl_check(glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo));
    void* ptr = glMapBufferRange(GL_PIXEL_UNPACK_BUFFER, 0, blocks_x * blocks_z, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
    opengl_check(ptr);
    if (ptr)
    {
        // Quantize the LOD to a R8_UNORM value as well.
        transform(begin(lod_buffer), end(lod_buffer), static_cast<uint8_t*>(ptr), [](float v) -> GLubyte {
            return clamp(GLubyte(round(v * 32.0f)), GLubyte(0), GLubyte(255));
            });

        opengl_check(glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER));

        opengl_check(glBindTexture(GL_TEXTURE_2D, lod_tex));
        opengl_check(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
            blocks_x, blocks_z, GL_RED, GL_UNSIGNED_BYTE, nullptr));
    }
    opengl_check(glBindTexture(GL_TEXTURE_2D, 0));
    opengl_check(glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0));

    for (auto& lod : lod_meshes)
        lod.full.instances = 0;

    opengl_check(glBindBuffer(GL_UNIFORM_BUFFER, ubo)); 
    PatchData* ubo_data = static_cast<PatchData*>(glMapBufferRange(GL_UNIFORM_BUFFER,
        0, lods * blocks_x * blocks_z * sizeof(PatchData),
        GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));
    opengl_check(ubo_data);

    if (!ubo_data)
    {
        printf("Failed to map buffer!");
        return;
    }

    // Fill in instancing info for all patches.
    for (unsigned z = 0; z < blocks_z; z++)
    {
        for (unsigned x = 0; x < blocks_x; x++)
        {
            if (!patches[z * blocks_x + x].visible)
                continue;

            // Clamp-to-edge.
            unsigned px = x ? (x - 1) : 0;
            unsigned pz = z ? (z - 1) : 0;
            unsigned nx = min(x + 1, blocks_x - 1);
            unsigned nz = min(z + 1, blocks_z - 1);

            // Look at neighbors.
            float left = lod_buffer[z * blocks_x + px];
            float top = lod_buffer[nz * blocks_x + x];
            float right = lod_buffer[z * blocks_x + nx];
            float bottom = lod_buffer[pz * blocks_x + x];
            float center = lod_buffer[z * blocks_x + x];

            // .. and pick out the lowest LOD for edges.
            float left_lod = max(left, center);
            float top_lod = max(top, center);
            float right_lod = max(right, center);
            float bottom_lod = max(bottom, center);
            int center_lod = int(center);

            auto& lod = lod_meshes[center_lod];

            unsigned ubo_offset = center_lod * blocks_x * blocks_z;

            ubo_data[ubo_offset + lod.full.instances].Offsets = vec4(
                patches[z * blocks_x + x].pos + block_offset, // Offset to world space.
                patches[z * blocks_x + x].pos);
            ubo_data[ubo_offset + lod.full.instances].LODs = vec4(left_lod, top_lod, right_lod, bottom_lod);
            ubo_data[ubo_offset + lod.full.instances].InnerLOD = vec4(center);

            lod.full.instances++;
        }
    }

    opengl_check(glUnmapBuffer(GL_UNIFORM_BUFFER));
}

void MorphedGeoMipMapMesh::LODMesh::draw(GLuint ubo, unsigned ubo_offset)
{
    // Draw everything with instancing.
    for (unsigned i = 0; i < instances; i += max_instances)
    {
        unsigned to_draw = min(instances - i, max_instances);
        opengl_check(glBindBufferRange(GL_UNIFORM_BUFFER, 0, ubo, i * sizeof(PatchData) + ubo_offset, max_instances * sizeof(PatchData)));
        opengl_check(glDrawElementsInstanced(GL_TRIANGLE_STRIP, elems, GL_UNSIGNED_SHORT,
            reinterpret_cast<const GLvoid*>(uintptr_t(offset * sizeof(GLushort))),
            to_draw));
    }
}

void MorphedGeoMipMapMesh::render(const RenderInfo& info)
{
    opengl_check(glUseProgram(prog));
    opengl_check(glBindVertexArray(vao));

    calculate_lods(info);

    opengl_check(glUniformMatrix4fv(0, 1, GL_FALSE, value_ptr(info.mvp)));
    opengl_check(glUniform4fv(1, 1, value_ptr(vec4(info.tile_extent / vec2(info.fft_size),
        info.normal_scale.x, info.normal_scale.y))));
    opengl_check(glUniform2fv(3, 1, value_ptr(vec2(info.fft_size) / info.tile_extent)));

    // EDIT LOUIS CAUBET: weird bug throws INVALID_OPERATION here. 
    // By getting it we prevent it from triggering opengl_check and crashing the program.
    glGetError();
    
    opengl_check(glUniform3fv(4, 1, value_ptr(info.cam_pos)));

    opengl_check(glUniform2f(5,
        1.0f / (patch_size * blocks_x),
        1.0f / (patch_size * blocks_z)));

    opengl_check(glUniform2f(6,
        1.0f / info.fft_size.x,
        1.0f / info.fft_size.y));

    bind_textures(info);
    opengl_check(glActiveTexture(GL_TEXTURE0 + 4));
    opengl_check(glBindTexture(GL_TEXTURE_2D, lod_tex));

    opengl_check(glEnable(GL_PRIMITIVE_RESTART_FIXED_INDEX));
    for (unsigned i = 0; i < lods; i++)
        lod_meshes[i].full.draw(ubo, i * blocks_x * blocks_z * sizeof(PatchData));
    opengl_check(glDisable(GL_PRIMITIVE_RESTART_FIXED_INDEX));

    opengl_check(glBindVertexArray(0));
    opengl_check(glBindBufferBase(GL_UNIFORM_BUFFER, 0, 0));
}

void MorphedGeoMipMapMesh::build_lod(unsigned lod)
{
    int size = patch_size >> lod;
    int size_1 = size + 1;
    int mod = 1 << lod;

    LOD lodmesh;

    lodmesh.full_vbo = vertices.size();
    // Stamp out a simple grid with (N + 1)^2 vertices (N^2 * 2 quads).
    for (int y = 0; y < size_1; y++)
        for (int x = 0; x < size_1; x++)
            vertices.emplace_back(x * mod, y * mod);

    lodmesh.full.offset = indices.size();
    // Stamp out a tight strip representation of the mesh.
    generate_block_indices(indices, lodmesh.full_vbo, size_1, size_1, size_1);
    lodmesh.full.elems = indices.size() - lodmesh.full.offset;

    for (unsigned i = lodmesh.full_vbo; i < vertices.size(); i++)
    {
        auto& v = vertices[i];

        // When creating new vertices create them towards center of the patch
        // to avoid popping artifacts at edges.
        v.rounding_x = v.x < patch_size / 2;
        v.rounding_y = v.y < patch_size / 2;

        // We don't really care about the corners since they will never snap anywhere, so it doesn't matter
        // which vertex LOD they snap to.
        memset(v.lod_weight, 0, sizeof(v.lod_weight));
        if (v.x == 0)
            v.lod_weight[0] = 1;
        else if (v.y == patch_size)
            v.lod_weight[1] = 1;
        else if (v.x == patch_size)
            v.lod_weight[2] = 1;
        else if (v.y == 0)
            v.lod_weight[3] = 1;

        // For inner vertices, use weights (0, 0, 0, 0) which means use center LOD.
    }

    lod_meshes.push_back(move(lodmesh));
}

void MorphedGeoMipMapMesh::build_patches()
{
    for (unsigned z = 0; z < blocks_z; z++)
        for (unsigned x = 0; x < blocks_x; x++)
            patches.push_back({ vec2(ivec2(x, z) * ivec2(patch_size)), 0.0f, false });
}

void MorphedGeoMipMapMesh::init_lod_tex()
{
    opengl_check(glGenTextures(1, &lod_tex));
    opengl_check(glBindTexture(GL_TEXTURE_2D, lod_tex));
    opengl_check(glTexStorage2D(GL_TEXTURE_2D, 1, GL_R8, (GLsizei)blocks_x, (GLsizei)blocks_z));
    opengl_check(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    opengl_check(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    opengl_check(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    opengl_check(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    opengl_check(glBindTexture(GL_TEXTURE_2D, 0));

    lod_buffer.resize(blocks_x * blocks_z);
}

MorphedGeoMipMapMesh::~MorphedGeoMipMapMesh()
{
    opengl_check(glDeleteTextures(1, &lod_tex));
    opengl_check(glDeleteBuffers(1, &ubo));
    opengl_check(glDeleteBuffers(1, &pbo));
}

void MorphedGeoMipMapMesh::init()
{
    // Create VBOs and IBOs for every lod.
    for (unsigned i = 0; i < lods; i++)
        build_lod(i);

    // Initalize patch data.
    build_patches();

    // Create LOD texture.
    init_lod_tex();

    std::cout << "Initialized LOD tex" << std::endl;

    // Create an UBO large enough to hold PatchData for all LODs.
    opengl_check(glGenBuffers(1, &ubo));
    opengl_check(glBindBuffer(GL_UNIFORM_BUFFER, ubo));
    opengl_check(glBufferData(GL_UNIFORM_BUFFER, lods * blocks_x * blocks_z * sizeof(PatchData), nullptr, GL_STREAM_DRAW));
    opengl_check(glBindBuffer(GL_UNIFORM_BUFFER, 0));

    // Create a PBO for updating LOD texture.
    opengl_check(glGenBuffers(1, &pbo));
    opengl_check(glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo));
    opengl_check(glBufferData(GL_PIXEL_UNPACK_BUFFER, blocks_x * blocks_z, nullptr, GL_STREAM_DRAW));
    opengl_check(glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0));

    // Set up VAO.
    opengl_check(glBindVertexArray(vao));

    opengl_check(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    opengl_check(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW));
    opengl_check(glEnableVertexAttribArray(0));
    opengl_check(glEnableVertexAttribArray(1));
    // We take in uvec4 in vertex shader, so use IPointer.
    opengl_check(glVertexAttribIPointer(0, 4, GL_UNSIGNED_BYTE, sizeof(Vertex), 0));
    opengl_check(glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(Vertex), reinterpret_cast<const GLvoid*>(uintptr_t(4))));

    opengl_check(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    opengl_check(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), indices.data(), GL_STATIC_DRAW));

    opengl_check(glBindVertexArray(0));
    opengl_check(glBindBuffer(GL_ARRAY_BUFFER, 0));
    opengl_check(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

TessellatedMesh::TessellatedMesh()
    : Mesh("water_tess.vs", "water_tess.tesc", "water_tess.tese", nullptr, "water.fs")
{
    init_vao();
}

void TessellatedMesh::init_vao()
{
    opengl_check(glBindVertexArray(vao));

    vector<ubvec2> verts(blocks_x * blocks_z);

    // Place patches on a large grid.
    for (unsigned z = 0; z < blocks_z; z++)
    {
        for (unsigned x = 0; x < blocks_x; x++)
        {
            verts[z * blocks_x + x].x = x;
            verts[z * blocks_x + x].y = z;
        }
    }

    // Sort patches front-to-back. X/Z coordinates close to (blocks_x / 2, blocks_z / 2) are centered around camera.
    sort(begin(verts), end(verts), [](vec2 a, vec2 b) {
        a -= vec2(blocks_x / 2, blocks_z / 2);
        b -= vec2(blocks_x / 2, blocks_z / 2);
        return vec_dot(a, a) < vec_dot(b, b);
        });

    num_vertices = blocks_x * blocks_z;

    // Setup VAO.
    opengl_check(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    opengl_check(glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(ubvec2), verts.data(), GL_STATIC_DRAW));
    opengl_check(glEnableVertexAttribArray(0));
    opengl_check(glVertexAttribIPointer(0, 2, GL_UNSIGNED_BYTE, 0, 0));

    opengl_check(glBindVertexArray(0));
    opengl_check(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void TessellatedMesh::render(const RenderInfo& info)
{
    opengl_check(glUseProgram(prog));
    opengl_check(glBindVertexArray(vao));

    vec2 patch_size_mod = vec2(patch_size) * info.tile_extent / vec2(info.fft_size);

    ivec2 block_off = ivec2(vec_round(vec2(info.cam_pos.x, info.cam_pos.z) / patch_size_mod));
    block_off -= ivec2(blocks_x >> 1, blocks_z >> 1);

    opengl_check(glUniformMatrix4fv(0, 1, GL_FALSE, value_ptr(info.mvp)));
    opengl_check(glUniform4fv(1, 1, value_ptr(vec4(info.tile_extent / vec2(info.fft_size),
        info.normal_scale.x, info.normal_scale.y))));
    opengl_check(glUniform2iv(2, 1, value_ptr(block_off)));
    opengl_check(glUniform2fv(3, 1, value_ptr(vec2(info.fft_size) / info.tile_extent)));
    opengl_check(glUniform3fv(4, 1, value_ptr(info.cam_pos)));

    opengl_check(glUniform2f(5, patch_size, patch_size));
    opengl_check(glUniform2f(6, log2(float(patch_size)), patch_size));
    opengl_check(glUniform1f(7, 1.0f / ((info.vp_width / 1920.0f) * lod0_distance)));
    opengl_check(glUniform2f(8,
        1.0f / info.fft_size.x, 1.0f / info.fft_size.y));
    opengl_check(glUniform4fv(9, 6, value_ptr(info.frustum[0])));

    bind_textures(info);

    // Render patches with tessellation.
    opengl_check(glPatchParameteri(GL_PATCH_VERTICES, 1));
    opengl_check(glDrawArrays(GL_PATCHES, 0, num_vertices));
    opengl_check(glBindVertexArray(0));
}
