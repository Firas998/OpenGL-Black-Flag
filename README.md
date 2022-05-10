# INF443 project: Black Pearl at sea

Louis Caubet & Firas Ben Jedidia

## Requirements

-   CGP, as installed in INF443. You should clone this repo in /scenes/inf443 for it to work seamlessly.

-   OpenGL 4.3 or later. You need to update the glad version used by CGP. To do so, download a newer version of glad [here](https://stratus.binets.fr/s/iBSsXigDxtsJMjS).
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
