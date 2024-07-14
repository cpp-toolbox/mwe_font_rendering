cmake_minimum_required(VERSION 3.10)

project(mwe_opengl_text)

add_executable(mwe_opengl_text
        src/main.cpp
        src/shader_pipeline/shader_pipeline.cpp
        src/window/window.cpp
        src/font_rendering/text_renderer.cpp
        src/font_rendering/text_renderer.hpp
)

find_package(glfw3)
find_package(glad)
find_package(freetype)
target_link_libraries(${PROJECT_NAME} glfw glad::glad Freetype::Freetype)