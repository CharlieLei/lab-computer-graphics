cmake_minimum_required(VERSION 3.17)
project(3_cubic)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXE_LINKER_FLAGS /NODEFAULTLIB:\"MSVCRT\")

include_directories(include)

link_directories(lib/glfw)

add_executable(3_cubic
        glad.c
        main.cpp
        Shader.cpp
        Camera.cpp
        stb_image.cpp

        Shader.h
        Camera.h
        stb_image.h)

target_link_libraries(3_cubic opengl32 glfw3)