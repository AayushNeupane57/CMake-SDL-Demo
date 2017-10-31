# CMake-SDL-Demo
A simple sample that sets up SDL for use with CMake for cross-platform development

## Windows:
This template expects an environment variable named `SDL2`, which is set to the path to the root of the SDL2 development libraries (Get the dev libraries from [HERE](https://www.libsdl.org/download-2.0.php))

## To Build:
1. Navigate to the root of the project (`cmake-sdl-demo/`)
2. `mkdir build`
3. `cd build`

Create project files:
- VS2017: `cmake ../ -G "Visual Studio 15"`
- Xcode: `cmake ../ -G "Xcode"`
