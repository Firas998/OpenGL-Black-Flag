# INF443 project: Black Pearl at sea

Louis Caubet & Firas Ben Jedidia

## Requirements

-   CGP, as installed in INF443. You should clone this repo in /scenes/inf443 for it to work seamlessly.

-   OpenGL 4.3 or later. You need to update the glad version used by CGP. To do so, download glad from https://stratus.binets.fr/s/iBSsXigDxtsJMjS
    Replace the glad folder in `library/third_party` with the downloaded version.

-   Assimp: install using vspkg:

    > ```
    > git clone https://github.com/Microsoft/vcpkg.git
    > cd vcpkg
    > ./bootstrap-vcpkg.sh
    > ./vcpkg integrate install
    > vcpkg install assimp
    > ```
    >
    > Set the path to `vspkg.cmake` in `CMakePresets.json`.

-   glslang: install using vspkg : `./vcpkg install glslang --triplet x64-windows`
-   rapidjson: install using vspkg : `./vcpkg install rapidjson --triplet x64-windows`
