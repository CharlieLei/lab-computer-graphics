cmake_minimum_required(VERSION 3.17)
project(C2_pyramid)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

link_directories(lib/glfw)

add_executable(C2_pyramid
        glad.c
        main.cpp

        tool/Shader.h
        tool/Camera.h
        tool/Timer.h

        tool/Shader.cpp
        tool/Camera.cpp
        tool/Timer.cpp)

target_link_libraries(C2_pyramid opengl32 glfw3)