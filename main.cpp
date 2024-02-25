#include <iostream>
#include <map>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "text_rendering.hpp"
#include "graphics.hpp"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main() {

    auto [able_to_initialize, window] = initialize_glfw_and_return_window(SCR_WIDTH, SCR_HEIGHT);
    if (!able_to_initialize) return -1;

    auto [able_to_prepare_font, char_to_drawing_data] = prepare_freetype_font();
    if (!able_to_prepare_font) return -1;

    auto [shader_pipeline, vao_name, vbo_name] = configure_opengl(SCR_WIDTH, SCR_HEIGHT);

    while (!glfwWindowShouldClose(window))
    {
        process_input(window);

        render(shader_pipeline, char_to_drawing_data, vao_name, vbo_name);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

