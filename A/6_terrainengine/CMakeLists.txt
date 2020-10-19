cmake_minimum_required(VERSION 3.17)
project(6_terrainengine)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

link_directories(lib/glfw)

add_executable(6_terrainengine
        glad.c
        main.cpp
        stb_image.h
        stb_image.cpp
        Shader.h
        Shader.cpp
        Camera.cpp
        Camera.h SkyBoxTexture.h)

target_link_libraries(6_terrainengine opengl32 glfw3)