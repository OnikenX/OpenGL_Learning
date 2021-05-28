cmake_minimum_required(VERSION 3.19)
set(CMAKE_CXX_STANDARD 20)

set(OpenGL_GL_PREFERENCE GLVND)

project(OpenGL_Learning)

add_executable(OpenGL_Learning src/main.cpp)

#hunter_add_package(glfw)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(dependecies)
target_link_libraries(OpenGL_Learning glfw OpenGL::GL GLEW)
