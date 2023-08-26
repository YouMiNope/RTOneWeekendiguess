# RTOneWeekendiguess
A Ray Tracing in One Weekend Implementation with Imgui and OpenGL

![image](https://github.com/YouMiNope/RTOneWeekendiguess/blob/master/images/rtweek.png)

## Build

Require CMake and GLFW

also make sure imgui submodule is not empty.

### GLFW

Build your own GLFW, you'll need glfw3.lib and headfiles.

### CMake

Config your CMake variable, whether by edit CMakePresets.json or use cmake-gui.
or you could just cmake with param -DLIBRARY_DIR=<your_path> -DINCLUDE_DIR=<path_again>

set LIBRARY_DIR to where your glfw3.lib is at, and INCLUDE_DIR to the path to headfiles