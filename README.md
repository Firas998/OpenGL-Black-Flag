# OpenGL Black Flag

A project by Louis Caubet & Firas Ben Jedidia for the INF443 Computer Graphics course at Polytechnique.

## Intro

In this project, we wanted to demonstrate a pirate ship at sea, with functional cannons able to shoot down an ennemy ship. We also demonstrated a procedurally generated island that can be visited.

## Demo

Please go take a look at our demo clip [here](https://www.youtube.com/watch?v=TKyNyBxJt5c) !

Screenshots:

![Screen1](https://user-images.githubusercontent.com/59528773/173236023-3178ceaf-bd0f-4e0f-863c-b2983522e3b6.png)
![Screen2](https://user-images.githubusercontent.com/59528773/173236028-46f93e43-b152-420e-93dd-3af66be78523.png)
![Screen3](https://user-images.githubusercontent.com/59528773/173236034-a88e2c64-9c50-4021-935e-0951d7e3961f.png)
![Screen4](https://user-images.githubusercontent.com/59528773/173236036-c4a3ae1b-3622-4f46-bfdb-f6073a98ad0b.png)
![Screen5](https://user-images.githubusercontent.com/59528773/173236041-16ea99d3-6b67-458c-af94-3639f89ae6cb.png)


## Installing and running

If you download the source code from the release, everything is ready to run out of the box. Just run `setup.bat` to install vcpkg dependencies and you're good to go!

Otherwise, please follow the steps below to set everything up manually.

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
    > ./vcpkg install assimp --triplet x64-windows
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

-   `FFTW3` is required. Add the precompiled library into a `precompiled/fftw3` folder. You can download the library from https://www.fftw.org/

-   The ship is an asset named "Black Pearl" made by "Dazimbane" on RenderHub.
