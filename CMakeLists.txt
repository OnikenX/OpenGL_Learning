cmake_minimum_required(VERSION 3.19)
set(CMAKE_CXX_STANDARD 20)

set(OpenGL_GL_PREFERENCE GLVND)
add_compile_definitions(DEBUG)

project(OpenGL_Learning)

add_executable(OpenGL_Learning src/main.cpp)
add_executable(tests src/test.cpp)
#hunter_add_package(glfw)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

#pkg_check_modules (PIPEWIRE REQUIRED libpipewire-0.3)

include_directories(dependecies)
target_link_libraries(OpenGL_Learning glfw OpenGL::GL GLEW)
