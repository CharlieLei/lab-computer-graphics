cmake_minimum_required(VERSION 3.17)
project(C3_cubic)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXE_LINKER_FLAGS /NODEFAULTLIB:\"MSVCRT\")

include_directories(include)

link_directories(lib/glfw)

add_executable(C3_cubic
        glad.c
        main.cpp
        tool/Shader.cpp
        tool/Camera.cpp
        tool/Timer.cpp
        tool/stb_image.cpp

        tool/Shader.h
        tool/Camera.h
        tool/Timer.h
        tool/stb_image.h)

target_link_libraries(C3_cubic opengl32 glfw3)