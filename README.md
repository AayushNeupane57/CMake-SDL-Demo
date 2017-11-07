# CMake-SDL-Demo
A simple sample that sets up SDL for use with CMake for cross-platform development

# Dependencies:
- SDL2
- GLEW
- CMake

## Windows:
### Environment Variables:
This template expects the following environment variables to be set in order to find external dependencies:
- `SDL2`: Path to the root of the SDL2 development library directory (Get the development libraries from [HERE](https://www.libsdl.org/download-2.0.php))
- `GLEW`: Path to the root of the GLEW binaries directory (Get the binaries from [HERE](http://glew.sourceforge.net/))

## To Build:
1. Navigate to the root of the project (`cmake-sdl-demo/`)
2. `mkdir build`
3. `cd build`
4. Create project files:
    - VS2017: `cmake ../ -G "Visual Studio 15"`
    - Xcode: `cmake ../ -G "Xcode"`
