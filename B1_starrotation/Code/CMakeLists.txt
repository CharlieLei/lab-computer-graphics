cmake_minimum_required(VERSION 3.17)
project(B1_starrotation)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

link_directories(lib/glfw)

add_executable(B1_starrotation glad.c main.cpp tool/Shader.cpp tool/Shader.h tool/Texture.cpp tool/Texture.h tool/stb_image.cpp tool/stb_image.h Star.cpp Star.h)

target_link_libraries(B1_starrotation opengl32 glfw3)