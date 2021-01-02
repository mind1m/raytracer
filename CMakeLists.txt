cmake_minimum_required (VERSION 3.0.0)
project (raytracer)

set ( CMAKE_CXX_STANDARD 11 )


set ( SOURCE_RAYTRACER
    src/utils.h
    src/vec3.h
    src/color.h
    src/ray.h
    src/hittable.h
    src/hittable_list.h
    src/sphere.h
    src/camera.h
    src/main.cpp
)

add_executable(raytracer ${SOURCE_RAYTRACER})