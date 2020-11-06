cmake_minimum_required(VERSION 3.13)
project(1_triangle)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

link_directories(lib/glfw)

add_executable(1_triangle
        glad.c
        main.cpp
        Shader.cpp

        Shader.h

        triangleVertex.glsl
        triangleFrag.glsl
        quadrangleFrag.glsl)

target_link_libraries(1_triangle opengl32 glfw3)