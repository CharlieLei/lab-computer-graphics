cmake_minimum_required(VERSION 3.17)
project(6_terrainengine)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

link_directories(lib/glfw)

add_executable(6_terrainengine
        glad.c
        main.cpp
        tool/stb_image.h
        tool/stb_image.cpp
        tool/Shader.h
        tool/Shader.cpp
        tool/Camera.cpp
        tool/Camera.h tool/Skybox.h tool/Terrain.h tool/Timer.cpp tool/Timer.h tool/Skybox.cpp tool/Terrain.cpp tool/Texture.cpp tool/Texture.h)

target_link_libraries(6_terrainengine opengl32 glfw3)