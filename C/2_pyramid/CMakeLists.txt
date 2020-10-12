cmake_minimum_required(VERSION 3.17)
project(2_pyramid)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

link_directories(lib/glfw)

add_executable(2_pyramid
        glad.c
        main.cpp

        Shader.h
        Camera.h

        Shader.cpp
        Camera.cpp)

target_link_libraries(2_pyramid opengl32 glfw3)