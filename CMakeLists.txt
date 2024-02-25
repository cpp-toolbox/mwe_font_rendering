cmake_minimum_required(VERSION 3.10)

project(mwe_opengl_text)

add_executable(mwe_opengl_text main.cpp
        shader_pipeline/shader_pipeline.cpp
        external_libraries/glad_opengl_3.3_core/src/gl.c
        text_rendering.cpp
        text_rendering.hpp
        graphics.cpp
        graphics.hpp
)

# GLAD: opengl function loader

include_directories(external_libraries/glad_opengl_3.3_core/include)
add_subdirectory(external_libraries/glad_opengl_3.3_core)

# GLM: opengl mathematics

include_directories(external_libraries/glm)
add_subdirectory(external_libraries/glm)

# GLFW: opengl framework

# disable unnessary steps in build
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(external_libraries/glfw)

# FreeType: font rendering

add_subdirectory(external_libraries/freetype)
include_directories(external_libraries/freetype/include)

target_link_libraries(mwe_opengl_text glad glfw freetype)
