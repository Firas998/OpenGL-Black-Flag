# INF443 project: Black Pearl at sea

Louis Caubet & Firas Ben Jedidia

## Requirements

-   CGP, as installed in INF443. You should clone this repo in /scenes/inf443 for it to work seamlessly.

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
