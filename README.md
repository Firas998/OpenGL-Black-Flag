# INF443 project: Black Pearl at sea

Louis Caubet & Firas Ben Jedidia

## Requirements

-   CGP, as installed in INF443. You should clone this repo in /scenes/inf443 for it to work seamlessly.

-   OpenGL 4.6. You need to update the glad version used by CGP. To do so, download a newer version of glad [here](https://stratus.binets.fr/s/iBSsXigDxtsJMjS).
    Replace the glad folder in `library/third_party` with the downloaded version.

-   Assimp: install using vcpkg:

    > ```
    > git clone https://github.com/Microsoft/vcpkg.git
    > cd vcpkg
    > ./bootstrap-vcpkg.sh
    > ./vcpkg integrate install
    > vcpkg install assimp
    > ```
    >
    > Download a sample `CMakePresets.json` [here](https://stratus.binets.fr/s/acZQrmKAqEAprDJ) and put in in the root directory. <br>
    > Set the path to `vcpkg.cmake` in `CMakePresets.json`.

-   glslang: install using vcpkg : `./vcpkg install glslang --triplet x64-windows`
-   rapidjson: install using vcpkg : `./vcpkg install rapidjson --triplet x64-windows`

## Third-party libraries

-   `stb_image.h` ([GitHub](https://github.com/nothings/stb)) is a single-file public domain C/C++ image loader. We use it to load the window icon.

-   `GLFFT` ([GitHub](https://github.com/Themaister/GLFFT)) is a C++/OpenGL library to run FFT on the GPU using compute shaders. We use the version shipped with the `opengl-es-sdk-for-android` due to compatibility issues with the latest version.

-   `FFTWater` is based on the FFTOceanWater advanced sample from `opengl-es-sdk-for-android` ([GitHub](https://github.com/ARM-software/opengl-es-sdk-for-android/tree/master/samples/advanced_samples/FFTOceanWater)). We made numerous changes to this code to make it compatible with OpenGL 4.6 instead of OpenGL ES and to adapt the result to our project.
