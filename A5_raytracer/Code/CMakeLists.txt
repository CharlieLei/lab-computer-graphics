cmake_minimum_required(VERSION 3.5)
project(A5_raytracer)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS -pthread)

include_directories(Eigen)

add_executable(A5_raytracer
        main.cpp
        bmp.cpp
        bmp.h
        camera.cpp
        camera.h
        color.cpp
        color.h
        light.cpp
        light.h
        primitive.cpp
        primitive.h
        raytracer.cpp
        raytracer.h
        scene.cpp
        scene.h
        vector3.cpp
        vector3.h
        )