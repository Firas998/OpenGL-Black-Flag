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

#include "scattering.hpp"
#include <utility>

Scattering::Scattering()
{
    prog = common_compile_compute_shader_from_file("scattering.comp");
}

Scattering::~Scattering()
{
    if (tex != 0)
    {
        opengl_check(glDeleteTextures(1, &tex));
    }

    if (prog != 0)
    {
        opengl_check(glDeleteProgram(prog));
    }
}

void Scattering::generate(unsigned size, vec3 sun_dir)
{
    if (tex != 0)
    {
        opengl_check(glDeleteTextures(1, &tex));
    }

    bool mipmap_fp16 = common_has_extension("GL_EXT_color_buffer_half_float");

    // EDIT LOUIS CAUBET: override this
    mipmap_fp16 = true;

    opengl_check(glGenTextures(1, &tex));
    opengl_check(glBindTexture(GL_TEXTURE_CUBE_MAP, tex));
    opengl_check(glTexStorage2D(GL_TEXTURE_CUBE_MAP, mipmap_fp16 ? int(log2(float(size))) + 1 : 1, GL_RGBA16F, size, size));
    opengl_check(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    opengl_check(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, mipmap_fp16 ? GL_LINEAR_MIPMAP_NEAREST : GL_LINEAR));

    // EDIT LOUIS CAUBET - add this to prevent seeing the borders of the skybox cube.
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    opengl_check(glUseProgram(prog));
    opengl_check(glUniform3fv(0, 1, value_ptr(sun_dir)));

    int steps = 100;
    opengl_check(glUniform1i(1, steps));

    float start = 10000000.0f;
    float end = 15000000.0f;
    opengl_check(glUniform1f(2, start));
    opengl_check(glUniform1f(3, (end - start) / steps));

    opengl_check(glBindImageTexture(0, tex, 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA16F));
    opengl_check(glDispatchCompute(size / 8, size / 8, 6));
    opengl_check(glMemoryBarrier(GL_ALL_BARRIER_BITS));

    if (mipmap_fp16)
    {
        opengl_check(glBindTexture(GL_TEXTURE_CUBE_MAP, tex));
        opengl_check(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));
    }
}
