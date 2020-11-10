cmake_minimum_required(VERSION 3.17)
project(B3_3dmodel)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXE_LINKER_FLAGS /NODEFAULTLIB:\"MSVCRT\")

include_directories(include)

link_directories(lib/glfw)

add_executable(B3_3dmodel
        glad.c
        main.cpp
        Model.cpp
        Model.h
        tool/Camera.cpp
        tool/Camera.h
        tool/Shader.cpp
        tool/Shader.h
        tool/Timer.cpp
        tool/Timer.h
        )

target_link_libraries(B3_3dmodel opengl32 glfw3)