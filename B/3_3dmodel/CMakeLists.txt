cmake_minimum_required(VERSION 3.17)
project(3_3dmodel)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXE_LINKER_FLAGS /NODEFAULTLIB:\"MSVCRT\")

include_directories(include)

link_directories(lib/glfw)

add_executable(3_3dmodel glad.c main.cpp ObjLoader.cpp ObjLoader.h Model.cpp Model.h Camera.cpp Camera.h Shader.cpp Shader.h stb_image.cpp stb_image.h)

target_link_libraries(3_3dmodel opengl32 glfw3)