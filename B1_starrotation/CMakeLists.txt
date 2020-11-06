cmake_minimum_required(VERSION 3.17)
project(1_starrotation)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

link_directories(lib/glfw)

add_executable(1_starrotation glad.c main.cpp Shader.cpp Shader.h Texture.cpp Texture.h stb_image.cpp stb_image.h Star.cpp Star.h)

target_link_libraries(1_starrotation opengl32 glfw3)